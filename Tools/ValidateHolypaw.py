#!/usr/bin/env python3
"""Static checks for Holypaw catalogs. Does not require Unreal."""
from __future__ import annotations

import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
TYPES = (ROOT / "Source/Holypaw/HolypawTypes.h").read_text()
VILLAINS = (ROOT / "Source/Holypaw/HolypawVillainCatalog.cpp").read_text()
WORLD = (ROOT / "Source/Holypaw/HolypawWorldBuilder.cpp").read_text()
CITIES = (ROOT / "Source/Holypaw/Cities/HolypawLivingCities.cpp").read_text() if (ROOT / "Source/Holypaw/Cities/HolypawLivingCities.cpp").exists() else ""
RIM = (ROOT / "Source/Holypaw/Cities/HolypawRimCities.cpp").read_text() if (ROOT / "Source/Holypaw/Cities/HolypawRimCities.cpp").exists() else ""
INTERIOR = (ROOT / "Source/Holypaw/Cities/HolypawInteriorKit.cpp").read_text() if (ROOT / "Source/Holypaw/Cities/HolypawInteriorKit.cpp").exists() else ""
STREAM = (ROOT / "Source/Holypaw/World/HolypawWorldStream.cpp").read_text() if (ROOT / "Source/Holypaw/World/HolypawWorldStream.cpp").exists() else ""
BOSS = (ROOT / "Source/Holypaw/Combat/HolypawBossScript.cpp").read_text() if (ROOT / "Source/Holypaw/Combat/HolypawBossScript.cpp").exists() else ""
SKILLS = (ROOT / "Source/Holypaw/HolypawSkillCatalog.cpp").read_text()
MISSIONS = (ROOT / "Source/Holypaw/HolypawMissionCatalog.cpp").read_text()
CHAR = (ROOT / "Source/Holypaw/Character/HolypawCharacter.cpp").read_text()
ATLAS_PATH = ROOT / "Source/Holypaw/HolypawAtlas.cpp"
ATLAS = ATLAS_PATH.read_text() if ATLAS_PATH.exists() else ""
errors: list[str] = []


def enum_names(src: str, enum: str) -> list[str]:
    m = re.search(rf"enum class {enum} : uint8\s*\{{(.*?)\}};", src, re.S)
    if not m:
        errors.append(f"missing enum {enum}")
        return []
    names = []
    for line in m.group(1).splitlines():
        line = line.strip()
        if not line or line.startswith("//"):
            continue
        name = re.split(r"\s", line)[0].rstrip(",")
        if name:
            names.append(name)
    return names


def find_ids(src: str, prefix: str) -> set[str]:
    return set(re.findall(rf"{prefix}::([A-Za-z0-9_]+)", src))


if not ATLAS_PATH.exists():
    errors.append("missing Source/Holypaw/HolypawAtlas.cpp")

villain_enum = enum_names(TYPES, "EHolypawVillain")
mission_enum = enum_names(TYPES, "EHolypawMission")
zone_enum = enum_names(TYPES, "EHolypawZone")
catalog_ids = find_ids(VILLAINS, "EHolypawVillain")
spawn_ids = find_ids(WORLD, "EHolypawVillain")
mission_ids = find_ids(MISSIONS, "EHolypawMission")

if "Sanrio" in CHAR or "Hello Kitty" in CHAR or "Sanrio" in MISSIONS or "Sanrio" in ATLAS:
    errors.append("copyright: Sanrio/Hello Kitty string found — keep Poly Mill original")

missing_cat = [n for n in villain_enum if n not in catalog_ids]
if missing_cat:
    errors.append(f"villain catalog missing {missing_cat}")

missing_spawn = [n for n in villain_enum if n not in spawn_ids]
if missing_spawn:
    errors.append(f"world spawn missing {missing_spawn}")

if len(villain_enum) != 40:
    errors.append(f"expected 40 villains, found {len(villain_enum)}")

if len(mission_enum) != 13:
    errors.append(f"expected 13 missions, found {len(mission_enum)}")

if "GlobeTrek" not in mission_enum:
    errors.append("mission enum missing GlobeTrek")

missing_mission = [n for n in mission_enum if n not in mission_ids]
if missing_mission:
    errors.append(f"mission catalog missing {missing_mission}")

hug = SKILLS.count("Add(EHolypawSkillTree::Hug")
miracle = SKILLS.count("Add(EHolypawSkillTree::Miracle")
party = SKILLS.count("Add(EHolypawSkillTree::Party")
if hug != 6 or miracle != 6 or party != 6:
    errors.append(f"expected 6 skills per tree, got hug={hug} miracle={miracle} party={party}")

required_hooks = [
    "NotifyRecruit",
    "NotifyKill",
    "NotifyConvert",
    "NotifyMiracle",
    "NotifyZone",
    "CompleteBearFaith",
    "BattleCommandId",
    "Kind == TEXT(\"guard\")",
    "Kind == TEXT(\"hymn\")",
]
for hook in required_hooks:
    if hook not in CHAR:
        errors.append(f"character missing hook {hook}")

if "BuildPolyMill" not in WORLD:
    errors.append("world missing BuildPolyMill")

for needle in ("BindKits", "GetRoads", "GetLandmasses", "LandHeightBias", "ResolveWilderness"):
    if needle not in WORLD and needle not in ATLAS:
        errors.append(f"globe architecture missing {needle}")

city_adds = re.findall(r"Add\(EHolypawZone::([A-Za-z]+)", ATLAS)
if len(city_adds) != 26:
    errors.append(f"expected 26 atlas cities, found {len(city_adds)}")

dup_cities = [n for n in city_adds if city_adds.count(n) > 1]
if dup_cities:
    errors.append(f"duplicate atlas cities {sorted(set(dup_cities))}")

missing_city_zones = [n for n in city_adds if n not in zone_enum]
if missing_city_zones:
    errors.append(f"atlas city not in EHolypawZone {missing_city_zones}")

for zone in city_adds:
    if f"case EHolypawZone::{zone}:" not in TYPES:
        errors.append(f"ZoneDisplayName missing {zone}")
    if f"case EHolypawZone::{zone}:" not in ATLAS:
        errors.append(f"ZoneTerrainColor missing {zone}")

road_ids = find_ids(ATLAS, "EHolypawZone")
# Roads should only name cities (plus whatever appears in wilderness helpers).
unknown_road = [n for n in find_ids(ATLAS.split("GetRoads")[-1].split("GetLandmasses")[0] if "GetRoads" in ATLAS else "", "EHolypawZone") if n not in city_adds]
if unknown_road:
    errors.append(f"roads reference non-city zones {unknown_road}")

landmasses = ATLAS.count("TEXT(\"")  # coarse; also require named continents
for continent in (
    "Home Sewn",
    "South Sewn",
    "Old World",
    "Sand Belt",
    "Silk East",
    "Cherry Isle",
    "Coral South",
    "Felt North",
    "Felt South",
):
    if continent not in ATLAS:
        errors.append(f"atlas missing continent {continent}")

coords = re.findall(r"FVector2D\(([-\d.]+)f,\s*([-\d.]+)f\)", ATLAS)
half = 167 * 1620 / 2.0
out_of_bounds = []
for x, y in coords:
    fx, fy = float(x), float(y)
    if abs(fx) > half or abs(fy) > half:
        out_of_bounds.append((fx, fy))
if out_of_bounds:
    errors.append(f"atlas points outside terrain half-extent {half:.0f}: {out_of_bounds[:6]}")

required_new = [
    "CarnivalBahia",
    "AndesLoom",
    "Clockhaven",
    "VelvetSeine",
    "MarbleForum",
    "SavannahBell",
    "SilkDelta",
    "SpiceHarbor",
    "CoralChoir",
    "FeltIceCamp",
]
missing_new = [n for n in required_new if n not in city_adds]
if missing_new:
    errors.append(f"atlas missing new cities {missing_new}")

if "CityVisitCount" not in (ROOT / "Source/Holypaw/Components/MissionComponent.cpp").read_text():
    errors.append("journal missing CityVisitCount")

UPROJECT = (ROOT / "Holypaw.uproject").read_text()
for plugin in ("PythonScriptPlugin", "ModelContextProtocol", "EnhancedInput"):
    if plugin not in UPROJECT:
        errors.append(f"uproject missing plugin {plugin}")

if not (ROOT / "legacy/web-prototype/index.html").exists():
    errors.append("missing archived web prototype")

INPUT = (ROOT / "Config/DefaultInput.ini").read_text()
if 'ActionName="Skill6"' not in INPUT:
    errors.append("DefaultInput missing Skill6 (Hymn)")

HUD = (ROOT / "Source/Holypaw/UI/HolypawHUD.cpp").read_text()
if "UHolypawBattleWidget" not in HUD:
    errors.append("HUD missing UMG battle overlay")
if not (ROOT / "Source/Holypaw/UI/HolypawBattleWidget.cpp").exists():
    errors.append("missing HolypawBattleWidget.cpp")

for needle in (
    "BuildRibbonDistricts",
    "BuildSkyRift",
    "AVolumetricCloud",
    "CottageBed",
    "Cloth Quarter",
    "Harbor Steps",
    "Quiet Rows",
):
    if needle not in WORLD:
        errors.append(f"world missing Phase 1 piece {needle}")

for skill in ("softFur", "buttonEyes", "haloStep", "miracleEcho", "partyBond", "fluffShield"):
    if skill not in SKILLS:
        errors.append(f"skill catalog missing {skill}")

PARTY = (ROOT / "Source/Holypaw/Components/PartyComponent.h").read_text()
if "MaxParty = 4" not in PARTY:
    errors.append("party max is not 4")
if "ReceiveHug" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("humans missing ReceiveHug juice")
if "HugLock" not in CHAR:
    errors.append("character missing hug lock (mash-protect)")
if "NewlyConvinced" not in CHAR:
    errors.append("miracle missing neighborhood sermon")

ENGINE = (ROOT / "Config/DefaultEngine.ini").read_text()
if "HolypawGameInstance" not in ENGINE:
    errors.append("DefaultEngine missing HolypawGameInstance")

for path in (
    "Source/Holypaw/Save/HolypawSaveGame.h",
    "Source/Holypaw/Save/HolypawSaveCodec.cpp",
    "Source/Holypaw/HolypawGameInstance.cpp",
    "Source/Holypaw/UI/HolypawTitleWidget.cpp",
    "Source/Holypaw/Actors/TravelLantern.cpp",
):
    if not (ROOT / path).exists():
        errors.append(f"missing {path}")

if "FHolypawHeartRecord" not in TYPES:
    errors.append("types missing FHolypawHeartRecord")
if "AddCityHeart" not in CHAR:
    errors.append("character missing city Hearts")
if "OpenFastTravel" not in CHAR:
    errors.append("character missing lantern fast travel")
if "ResetForNewGame" not in CHAR:
    errors.append("character missing new-game reset")
if "PlaceLantern" not in WORLD:
    errors.append("world missing PlaceLantern")
if "GetTravelLocation" not in WORLD:
    errors.append("world missing GetTravelLocation")
if "ResetFaith" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("humans missing ResetFaith for save/load")
if "bBeliever && !bKnelt" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("humans missing believer parade")
if 'ActionName="QuickSave"' not in INPUT:
    errors.append("DefaultInput missing QuickSave (F5)")
if 'ActionName="TitleConfirm"' not in INPUT:
    errors.append("DefaultInput missing TitleConfirm")
if "UHolypawTitleWidget" not in HUD:
    errors.append("HUD missing title overlay")

if not (ROOT / "Source/Holypaw/Audio/HolypawAudio.cpp").exists():
    errors.append("missing procedural audio")
if not (ROOT / "Source/Holypaw/Audio/HolypawAudioSubsystem.cpp").exists():
    errors.append("missing audio subsystem")
if not (ROOT / "Source/Holypaw/Audio/HolypawMusicScore.cpp").exists():
    errors.append("missing city music scores")
if not (ROOT / "Source/Holypaw/Audio/HolypawSfxBank.cpp").exists():
    errors.append("missing sfx bank")
if not (ROOT / "Source/Holypaw/Audio/HolypawAudioCueTable.cpp").exists():
    errors.append("missing audio cue table")

AUDIO_SCORE = (ROOT / "Source/Holypaw/Audio/HolypawMusicScore.cpp").read_text() if (ROOT / "Source/Holypaw/Audio/HolypawMusicScore.cpp").exists() else ""
AUDIO_CUES = (ROOT / "Source/Holypaw/Audio/HolypawAudioCueTable.cpp").read_text() if (ROOT / "Source/Holypaw/Audio/HolypawAudioCueTable.cpp").exists() else ""
AUDIO_SYS = (ROOT / "Source/Holypaw/Audio/HolypawAudioSubsystem.cpp").read_text() if (ROOT / "Source/Holypaw/Audio/HolypawAudioSubsystem.cpp").exists() else ""
AUDIO_SFX = (ROOT / "Source/Holypaw/Audio/HolypawSfxBank.cpp").read_text() if (ROOT / "Source/Holypaw/Audio/HolypawSfxBank.cpp").exists() else ""

for z in zone_enum:
    if f"EHolypawZone::{z}" not in AUDIO_SCORE:
        errors.append(f"music score missing zone {z}")

for cue in (
    "Hug", "Convert", "Miracle", "Save", "Shop", "Talk", "Title", "Door", "Travel",
    "BattleStart", "BattleHit", "BattleCrit", "BattleStagger", "BattleWin", "BattleLose",
    "Hurt", "Slap", "Beam", "Party", "Guard", "Hymn", "Lullaby", "Unstuff", "PolyRip",
    "Flee", "BossPhase", "Jump", "Land", "Pickup", "Chapel", "Fountain", "Inn", "Lantern",
    "UiOpen", "UiConfirm", "FootstepSoft",
):
    if f'TEXT("{cue}")' not in AUDIO_CUES:
        errors.append(f"cue table missing {cue}")

if "UHolypawAudioSubsystem" not in AUDIO_SYS:
    errors.append("audio subsystem missing class body")
if "RebuildCombat" not in AUDIO_SYS:
    errors.append("audio subsystem missing combat stem")
if "MakeSeamless" not in (ROOT / "Source/Holypaw/Audio/HolypawSynth.cpp").read_text():
    errors.append("synth missing seamless loop crossfade")
if "KeepStem" not in AUDIO_SYS:
    errors.append("audio subsystem missing stem retain")
if "NotifyVictory" not in AUDIO_SYS:
    errors.append("audio subsystem missing victory fade")
if "PlayCachedFootstep" not in AUDIO_SYS:
    errors.append("audio subsystem missing footstep pool")
if "FootSurface" not in AUDIO_SFX:
    errors.append("sfx bank missing foot surface ids")
if "RenderMill" not in AUDIO_SCORE:
    errors.append("music score missing mill drone")
if "PlayAbility" not in CHAR:
    errors.append("character missing PlayAbility combat audio")
if "SetCombat" not in CHAR:
    errors.append("character missing combat music hook")
if "MusicVolume" not in (ROOT / "Source/Holypaw/Save/HolypawSaveGame.h").read_text():
    errors.append("settings missing MusicVolume bus")
if "OscBell" not in AUDIO_SFX:
    errors.append("sfx bank missing layered bell hits")
if "RenderFootstep" not in AUDIO_SFX:
    errors.append("sfx bank missing footsteps")
if not (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").exists():
    errors.append("missing dialogue catalog")
if not (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").exists():
    errors.append("missing item catalog")
if "StartTalk" not in CHAR:
    errors.append("character missing StartTalk")
if "OpenShop" not in CHAR:
    errors.append("character missing OpenShop")
if "TickProcAnim" not in CHAR:
    errors.append("character missing teddy proc anim")
if "stuffingBun" not in (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text():
    errors.append("item catalog missing stuffingBun")
if "Park Ranger" not in (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text():
    errors.append("dialogue missing Park Ranger")
if "KeepWave" not in (ROOT / "Source/Holypaw/HolypawGameInstance.cpp").read_text():
    errors.append("game instance missing audio KeepWave")
if 'ActionName="Inventory"' not in INPUT:
    errors.append("DefaultInput missing Inventory")
if "OpenShop" not in (ROOT / "Source/Holypaw/Actors/FaithStall.cpp").read_text():
    errors.append("stall missing shop panel")
if "StartTalk" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("believers should talk, not re-hug")

if not (ROOT / "Source/Holypaw/Actors/HolypawPickup.cpp").exists():
    errors.append("missing HolypawPickup.cpp")
if not (ROOT / "Source/Holypaw/Actors/HolypawShrine.cpp").exists():
    errors.append("missing HolypawShrine.cpp")
if not (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").exists():
    errors.append("missing HolypawQuestCatalog.cpp")
if not (ROOT / "Source/Holypaw/Narrative/HolypawDialogueVM.cpp").exists():
    errors.append("missing HolypawDialogueVM.cpp")
if not (ROOT / "Source/Holypaw/Combat/HolypawBattleDirector.cpp").exists():
    errors.append("missing HolypawBattleDirector.cpp")
if not (ROOT / "Source/Holypaw/Combat/HolypawAbilityCatalog.cpp").exists():
    errors.append("missing HolypawAbilityCatalog.cpp")
if not (ROOT / "Source/Holypaw/Cities/HolypawLivingCities.cpp").exists():
    errors.append("missing HolypawLivingCities.cpp")
if not (ROOT / "Source/Holypaw/Cities/HolypawRimCities.cpp").exists():
    errors.append("missing HolypawRimCities.cpp")
for needle, blob, label in (
    ("PlaceShrine", WORLD, "world"),
    ("TickClockLighting", WORLD, "world"),
    ("UseShrine", CHAR, "character"),
    ("GetBattleStatusLine", CHAR, "character"),
    ("PulseHit", (ROOT / "Source/Holypaw/Actors/HostilePet.cpp").read_text(), "hostile"),
    ("Innkeep", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Choir Bear", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Seamstress", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Dock Poet", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Lamp Lighter", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Spire Guard", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Mill Whistleblower", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("ScaleForArmor", (ROOT / "Source/Holypaw/Combat/HolypawBattleMath.cpp").read_text(), "battle math"),
    ("BossPhaseLine", (ROOT / "Source/Holypaw/Combat/HolypawBattleMath.cpp").read_text(), "battle math"),
    ("TryEnterPhaseTwo", (ROOT / "Source/Holypaw/Actors/HostilePet.cpp").read_text(), "hostile"),
    ("TurnInErrand", CHAR, "character"),
    ("CottageLoft", WORLD, "world"),
    ("PlacePickup", WORLD, "world"),
    ("hymnSheet", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("Plaza Florist", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("AcceptQuest", CHAR, "character"),
    ("BuildTidewellDistricts", WORLD, "world"),
    ("saltPearl", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("cellarHymn", (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").read_text() if (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").exists() else "", "quests"),
    ("Ferry Clerk", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Salt Priest", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Lullaby", CHAR, "character"),
    ("BuildHearthfoldDistricts", WORLD, "world"),
    ("BuildEmberfenDistricts", WORLD, "world"),
    ("BuildSnowveilDistricts", WORLD, "world"),
    ("wheatEar", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("peatBead", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("auroraThread", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("Thresh Singer", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Frog Bell", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Scarf Knitter", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("wheatHymn", (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").read_text() if (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").exists() else "", "quests"),
    ("BuildLanternAngelesDistricts", WORLD, "world"),
    ("BuildMossgateDistricts", WORLD, "world"),
    ("BuildPalmaDuskDistricts", WORLD, "world"),
    ("BuildCherryLoomDistricts", WORLD, "world"),
    ("goldPoster", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("fogTea", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("palmClap", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("cherryThread", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("goldReel", (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").read_text() if (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").exists() else "", "quests"),
    ("Best Boy", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Tea Cart", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Clap Kid", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Gate Watch", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("VerbFromSlot", (ROOT / "Source/Holypaw/Narrative/HolypawDialogueVM.cpp").read_text() if (ROOT / "Source/Holypaw/Narrative/HolypawDialogueVM.cpp").exists() else "", "dialogue VM"),
    ("RollLullaby", (ROOT / "Source/Holypaw/Combat/HolypawBattleDirector.cpp").read_text() if (ROOT / "Source/Holypaw/Combat/HolypawBattleDirector.cpp").exists() else "", "battle director"),
    ("ApplyTalkVerb", CHAR, "character"),
    ("ApplyOutgoing", CHAR, "character"),
    ("ApplyIncoming", CHAR, "character"),
    ("BuildQuiltlandDistricts", CITIES, "living cities"),
    ("BuildDustMesaDistricts", CITIES, "living cities"),
    ("BuildClockhavenDistricts", CITIES, "living cities"),
    ("BuildVelvetSeineDistricts", CITIES, "living cities"),
    ("BuildMarbleForumDistricts", CITIES, "living cities"),
    ("BuildIvorySpireDistricts", CITIES, "living cities"),
    ("BuildSandHymnDistricts", CITIES, "living cities"),
    ("BuildCapePlushDistricts", CITIES, "living cities"),
    ("BuildSavannahBellDistricts", CITIES, "living cities"),
    ("TickWorldStream", WORLD, "world"),
    ("RequestDress", WORLD, "world"),
    ("pinePatch", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("clockCog", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("columnHat", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("acaciaBell", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("teaCog", (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").read_text() if (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").exists() else "", "quests"),
    ("Hat Mender", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Fog Usher", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("LineC", (ROOT / "Source/Holypaw/Narrative/HolypawDialogueVM.cpp").read_text() if (ROOT / "Source/Holypaw/Narrative/HolypawDialogueVM.cpp").exists() else "", "dialogue VM"),
    ("FindAbility", (ROOT / "Source/Holypaw/Combat/HolypawAbilityCatalog.cpp").read_text() if (ROOT / "Source/Holypaw/Combat/HolypawAbilityCatalog.cpp").exists() else "", "ability catalog"),
    ("FleeChance", (ROOT / "Source/Holypaw/Combat/HolypawBattleDirector.cpp").read_text() if (ROOT / "Source/Holypaw/Combat/HolypawBattleDirector.cpp").exists() else "", "battle director"),
    ("BuildCarnivalBahiaDistricts", RIM, "rim cities"),
    ("BuildAndesLoomDistricts", RIM, "rim cities"),
    ("BuildSilkDeltaDistricts", RIM, "rim cities"),
    ("BuildSpiceHarborDistricts", RIM, "rim cities"),
    ("BuildCoralChoirDistricts", RIM, "rim cities"),
    ("BuildAuroraBoroughDistricts", RIM, "rim cities"),
    ("BuildTundraParishDistricts", RIM, "rim cities"),
    ("BuildFeltIceCampDistricts", RIM, "rim cities"),
    ("confettiDrum", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("moonThread", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("spareScarf", (ROOT / "Source/Holypaw/HolypawItemCatalog.cpp").read_text(), "items"),
    ("drumGov", (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").read_text() if (ROOT / "Source/Holypaw/HolypawQuestCatalog.cpp").exists() else "", "quests"),
    ("Drum Kid", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Penguin Usher", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("Skein Mender", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("DressInterior", INTERIOR, "interior kit"),
    ("RoomShell", INTERIOR, "interior kit"),
    ("InnBed", INTERIOR, "interior kit"),
    ("ChapelPew", INTERIOR, "interior kit"),
    ("WorkshopLoom", INTERIOR, "interior kit"),
    ("DressCottageRooms", INTERIOR, "interior kit"),
    ("TickWorldStream", STREAM, "world stream"),
    ("DressCity", STREAM, "world stream"),
    ("RequestDress", STREAM, "world stream"),
    ("BuildCarnivalBahiaDistricts", STREAM, "world stream"),
    ("BuildFeltIceCampDistricts", STREAM, "world stream"),
    ("PlayTheme", (ROOT / "Source/Holypaw/Audio/HolypawAudio.cpp").read_text(), "audio"),
    ("SetCombat", (ROOT / "Source/Holypaw/Audio/HolypawAudio.cpp").read_text(), "audio"),
    ("PlayAbility", (ROOT / "Source/Holypaw/Audio/HolypawAudio.cpp").read_text(), "audio"),
    ("FindAbilityBySlot", (ROOT / "Source/Holypaw/Combat/HolypawAbilityCatalog.cpp").read_text(), "ability catalog"),
    ("SilkMagistrate", BOSS, "boss script"),
    ("MiracleEater", BOSS, "boss script"),
    ("BattlePage", CHAR, "character"),
    ("unstuff", CHAR, "character"),
    ("buttonBeam", CHAR, "character"),
):
    if needle not in blob:
        errors.append(f"{label} missing {needle}")

if errors:
    print("FAIL")
    for e in errors:
        print(" -", e)
    sys.exit(1)

print("OK")
print(
    f" villains={len(villain_enum)} spawned={len(spawn_ids)} missions={len(mission_enum)}"
    f" cities={len(city_adds)} skills hug/miracle/party={hug}/{miracle}/{party}"
)
sys.exit(0)
