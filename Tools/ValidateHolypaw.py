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
SCHEDULE = (ROOT / "Source/Holypaw/AI/HolypawSchedule.cpp").read_text() if (ROOT / "Source/Holypaw/AI/HolypawSchedule.cpp").exists() else ""
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
if "PulseLivingWorld" not in CHAR:
    errors.append("character missing living-world pulse")
if "GetFaithLine" not in CHAR:
    errors.append("character missing conversion HUD line")
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

UI_SUITE = (
    "Source/Holypaw/UI/HolypawUiTheme.cpp",
    "Source/Holypaw/UI/HolypawUiIcons.cpp",
    "Source/Holypaw/UI/HolypawUiCopy.cpp",
    "Source/Holypaw/UI/HolypawPauseWidget.cpp",
    "Source/Holypaw/UI/HolypawMapWidget.cpp",
    "Source/Holypaw/UI/HolypawJournalWidget.cpp",
    "Source/Holypaw/UI/HolypawTalkWidget.cpp",
    "Source/Holypaw/UI/HolypawShopWidget.cpp",
    "Source/Holypaw/UI/HolypawPlayHudWidget.cpp",
    "Source/Holypaw/UI/HolypawCodexWidget.cpp",
)
for path in UI_SUITE:
    if not (ROOT / path).exists():
        errors.append(f"missing {path}")

THEME = (ROOT / "Source/Holypaw/UI/HolypawUiTheme.cpp").read_text() if (ROOT / "Source/Holypaw/UI/HolypawUiTheme.cpp").exists() else ""
ICONS = (ROOT / "Source/Holypaw/UI/HolypawUiIcons.cpp").read_text() if (ROOT / "Source/Holypaw/UI/HolypawUiIcons.cpp").exists() else ""
COPY = (ROOT / "Source/Holypaw/UI/HolypawUiCopy.cpp").read_text() if (ROOT / "Source/Holypaw/UI/HolypawUiCopy.cpp").exists() else ""
MAPW = (ROOT / "Source/Holypaw/UI/HolypawMapWidget.cpp").read_text() if (ROOT / "Source/Holypaw/UI/HolypawMapWidget.cpp").exists() else ""
TITLEW = (ROOT / "Source/Holypaw/UI/HolypawTitleWidget.cpp").read_text()
BATTLEW = (ROOT / "Source/Holypaw/UI/HolypawBattleWidget.cpp").read_text()
PAUSEW = (ROOT / "Source/Holypaw/UI/HolypawPauseWidget.cpp").read_text() if (ROOT / "Source/Holypaw/UI/HolypawPauseWidget.cpp").exists() else ""
TALKW = (ROOT / "Source/Holypaw/UI/HolypawTalkWidget.cpp").read_text() if (ROOT / "Source/Holypaw/UI/HolypawTalkWidget.cpp").exists() else ""
SHOPW = (ROOT / "Source/Holypaw/UI/HolypawShopWidget.cpp").read_text() if (ROOT / "Source/Holypaw/UI/HolypawShopWidget.cpp").exists() else ""
PLAYW = (ROOT / "Source/Holypaw/UI/HolypawPlayHudWidget.cpp").read_text() if (ROOT / "Source/Holypaw/UI/HolypawPlayHudWidget.cpp").exists() else ""
if "GetFaithLine" not in PLAYW:
    errors.append("play HUD missing conversion stage line")
JOURNALW = (ROOT / "Source/Holypaw/UI/HolypawJournalWidget.cpp").read_text() if (ROOT / "Source/Holypaw/UI/HolypawJournalWidget.cpp").exists() else ""
CODEXW = (ROOT / "Source/Holypaw/UI/HolypawCodexWidget.cpp").read_text() if (ROOT / "Source/Holypaw/UI/HolypawCodexWidget.cpp").exists() else ""

if "DashRect" not in THEME or "CornerKnots" not in THEME:
    errors.append("UI theme missing stitched panel chrome")
if "TextBlock" not in THEME or "WrapLines" not in THEME:
    errors.append("UI theme missing wrapped text")
if "Ellipsize" not in THEME:
    errors.append("UI theme missing ellipsize")
if "ChipRow" not in THEME or "MeasureChip" not in THEME:
    errors.append("UI theme missing flowing chip row")
if "Toast" not in THEME or "Meter" not in THEME:
    errors.append("UI theme missing toast/meter")
if "KeyWidth" not in THEME:
    errors.append("UI theme missing measured keycaps")
if "DrawIcon" not in ICONS or "EHolypawUiIcon::Heart" not in ICONS or "EHolypawUiIcon::Lantern" not in ICONS:
    errors.append("UI icons missing stitched heart/lantern glyphs")
if 'NSLOCTEXT("HolypawUI"' not in COPY:
    errors.append("UI copy missing HolypawUI loc keys")
if "HeartsHeat" not in MAPW or "WorldToMap" not in MAPW:
    errors.append("map widget missing Hearts heat atlas")
if "GetPeakCenter" not in MAPW:
    errors.append("map widget missing cottage/peak pins")
if "EHolypawPawnMode::Pause" in TITLEW:
    errors.append("title widget still owns pause — pause is its own product surface")
if "UHolypawPauseWidget" not in HUD:
    errors.append("HUD missing pause overlay")
if "UHolypawMapWidget" not in HUD:
    errors.append("HUD missing map overlay")
if "UHolypawJournalWidget" not in HUD:
    errors.append("HUD missing journal overlay")
if "UHolypawTalkWidget" not in HUD:
    errors.append("HUD missing talk overlay")
if "UHolypawShopWidget" not in HUD:
    errors.append("HUD missing shop overlay")
if "UHolypawPlayHudWidget" not in HUD:
    errors.append("HUD missing play HUD overlay")
if "UHolypawCodexWidget" not in HUD:
    errors.append("HUD missing codex overlay")
for leftover in ("Survey Map", "Bear Faith Journal", "Testimony", "Faith stall", "Villain Codex"):
    if leftover in HUD:
        errors.append(f"HUD still dumping leftover '{leftover}' strings")
if "IconForAbility" not in BATTLEW:
    errors.append("battle overlay missing stitched ability icons")
if "VerbRow" not in TALKW:
    errors.append("talk overlay missing verb row")
if "ShopDiscount" not in SHOPW and "Hearts discount" not in SHOPW:
    errors.append("shop overlay missing Hearts discount")
if "MiracleCharge" not in PLAYW:
    errors.append("play HUD missing Miracle bar")
if "TextBlock" not in TALKW or "TextBlock" not in (ROOT / "Source/Holypaw/UI/HolypawJournalWidget.cpp").read_text():
    errors.append("talk/journal missing wrapped body copy")
if "LandTint" not in MAPW:
    errors.append("map widget missing continent tint")
if "GetTalkSpeaker" not in (ROOT / "Source/Holypaw/Character/HolypawCharacter.h").read_text():
    errors.append("character missing GetTalkSpeaker for talk surface")
if "GetPeakCenter" not in (ROOT / "Source/Holypaw/HolypawWorldBuilder.h").read_text():
    errors.append("world missing GetPeakCenter for map pins")
if "Paused" not in PAUSEW:
    errors.append("pause widget missing paused title")
if "ChipRow" not in TITLEW or "ChipRow" not in PAUSEW or "ChipRow" not in SHOPW:
    errors.append("title/pause/shop missing flowing chip rows")
if "ChipRow" not in MAPW or "ChipRow" not in JOURNALW:
    errors.append("map/journal missing flowing chip rows")
if 'TEXT("Lanterns")' in MAPW:
    errors.append("map still invents leftover Lanterns string")
if "HomePin" not in MAPW or "PeakPin" not in MAPW:
    errors.append("map missing loc Home/Peak pins")
if 'TEXT("Fluffy Party")' in BATTLEW or 'TEXT("Pockets")' in BATTLEW:
    errors.append("battle overlay still invents leftover party/pocket titles")
if "Recruits %d" in JOURNALW or "Talk 4 takes a job" in JOURNALW:
    errors.append("journal still invents leftover recruit/errand strings")
if "Seen %d" in CODEXW or 'TEXT("fell")' in CODEXW:
    errors.append("codex still invents leftover seen/fell strings")
if "Quiet line at" in TALKW:
    errors.append("talk still invents leftover quiet-line strings")
if "NeedAp" not in SHOPW:
    errors.append("shop missing can't-afford copy")

if not (ROOT / "Source/Holypaw/Audio/HolypawAudio.cpp").exists():
    errors.append("missing procedural audio")
if not (ROOT / "Source/Holypaw/Anim/HolypawProcAnim.cpp").exists():
    errors.append("missing procedural anim")
ANIM = (ROOT / "Source/Holypaw/Anim/HolypawProcAnim.cpp").read_text() if (ROOT / "Source/Holypaw/Anim/HolypawProcAnim.cpp").exists() else ""
ANIMH = (ROOT / "Source/Holypaw/Anim/HolypawProcAnim.h").read_text() if (ROOT / "Source/Holypaw/Anim/HolypawProcAnim.h").exists() else ""
for needle in (
    "WrapKeys",
    "BlinkKeys",
    "KneelKeys",
    "VictorySpinKeys",
    "PlayHug",
    "PlayVictory",
    "PlayConvert",
    "PlayConvertBow",
    "EvaluateParty",
    "EarStiffness",
    "ClipName",
):
    if needle not in ANIM and needle not in ANIMH:
        errors.append(f"proc anim missing {needle}")
if "HolypawAnim::PlayHug" not in CHAR:
    errors.append("character missing hug wrap clip")
if "HolypawAnim::PlayConvert" not in CHAR:
    errors.append("character missing convert hold-then-victory")
if "CelebrateConvert" not in CHAR:
    errors.append("character missing convert victory")
if "EvaluateParty" not in CHAR:
    errors.append("character missing party trail motion")
if "PlayConvertBow" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("humans missing convert bow")
if "EvaluateFluffy" not in (ROOT / "Source/Holypaw/Actors/WildFluffy.cpp").read_text():
    errors.append("fluffies missing ear flop")
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
if "Shutter" not in (ROOT / "Source/Holypaw/Actors/FaithStall.cpp").read_text():
    errors.append("stall missing Hearts shutter")
if "IsUnlatched" not in (ROOT / "Source/Holypaw/Actors/FaithStall.cpp").read_text():
    errors.append("stall missing Hearts gate")
if "StartTalk" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("believers should talk, not re-hug")
if "ClapBurst" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("believers missing clap burst")
if "IsClapping" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("believers missing clap")
if "Sash" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("believers missing sash outfit")

FAITH = (ROOT / "Source/Holypaw/Faith/HolypawFaithSim.cpp").read_text() if (ROOT / "Source/Holypaw/Faith/HolypawFaithSim.cpp").exists() else ""
BANNER = (ROOT / "Source/Holypaw/Actors/HolypawMillBanner.cpp").read_text() if (ROOT / "Source/Holypaw/Actors/HolypawMillBanner.cpp").exists() else ""
PULSE = (ROOT / "Source/Holypaw/Cities/HolypawConversionPulse.cpp").read_text() if (ROOT / "Source/Holypaw/Cities/HolypawConversionPulse.cpp").exists() else ""
if not FAITH:
    errors.append("missing HolypawFaithSim.cpp")
if not BANNER:
    errors.append("missing HolypawMillBanner.cpp")
if not PULSE:
    errors.append("missing HolypawConversionPulse.cpp")
if "ShopOpenNeed" not in FAITH:
    errors.append("faith sim missing shop Heart gate")
if "BannerFallNeed" not in FAITH:
    errors.append("faith sim missing mill banner Heart gate")
if "DuskHymnNeed" not in FAITH:
    errors.append("faith sim missing dusk hymn Heart gate")
if "Mill banners down" not in FAITH:
    errors.append("faith sim missing banner-down copy")
if "HandmadeRibbon" not in BANNER:
    errors.append("mill banners missing handmade replacement")
if "PlaceRibbonMillBanners" not in PULSE:
    errors.append("conversion pulse missing Ribbon mill banners")
if "PlayHymn" not in PULSE:
    errors.append("conversion pulse missing dusk hymn pad")
if "PlayHymn" not in (ROOT / "Source/Holypaw/Audio/HolypawAudio.cpp").read_text():
    errors.append("audio missing dusk hymn pad")
if "ParadeGoal" not in SCHEDULE:
    errors.append("schedule missing believer parade goals")
if "PlazaGoal" not in SCHEDULE:
    errors.append("schedule missing plaza clap-walk")
if "VInterpConstantTo" not in SCHEDULE:
    errors.append("schedule missing real walk (not a slide)")
if "ParadeSalt" not in SCHEDULE:
    errors.append("schedule missing staggered parade")
if "LivingSign" not in FAITH:
    errors.append("faith sim missing living sign copy")
if "NotifyConvertPulse" not in PULSE:
    errors.append("conversion pulse missing nearby clap/banner drop")
if "FountainPool" not in PULSE:
    errors.append("conversion pulse missing fountain look")
if "DressOpenStall" not in INTERIOR:
    errors.append("interior kit missing open plaza kiosk")
if "CelebrateHold" not in SCHEDULE:
    errors.append("schedule missing convert celebrate-hold")
if "NoticeConvert" not in PULSE:
    errors.append("conversion pulse missing skeptic look-at")
if "Fountain Shopkeep" not in WORLD:
    errors.append("world missing fountain shopkeep")
if "CreditZone" not in CHAR:
    errors.append("character missing home-city Heart credit")

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
    ("CanopyPuffs", WORLD, "world"),
    ("GradeVolume", WORLD, "world"),
    ("FillLight", WORLD, "world"),
    ("earL", CHAR, "character"),
    ("HaloLight", CHAR, "character"),
    ("TickProcAnim", CHAR, "character"),
    ("TickHuman", SCHEDULE, "schedule"),
    ("ChapelGoal", SCHEDULE, "schedule"),
    ("ParadeGoal", SCHEDULE, "schedule"),
    ("DressShopRoom", INTERIOR, "interior kit"),
    ("DressMillHall", INTERIOR, "interior kit"),
    ("MillConveyor", INTERIOR, "interior kit"),
    ("ShopCounter", INTERIOR, "interior kit"),
    ("Floor Foreman", WORLD, "world"),
    ("Shopkeep", (ROOT / "Source/Holypaw/HolypawDialogueCatalog.cpp").read_text(), "dialogue"),
    ("RoleFor", (ROOT / "Source/Holypaw/Components/PartyComponent.cpp").read_text(), "party"),
    ("fluffBurst", CHAR, "character"),
    ("DuskHymn", CHAR, "character"),
    ("PulseLivingWorld", CHAR, "character"),
    ("PlaceRibbonMillBanners", WORLD, "world"),
    ("ShopClosedLine", CHAR, "character"),
    ("NotifyConvertPulse", CHAR, "character"),
    ("ribbonPlaza", WORLD, "world"),
):
    if needle not in blob:
        errors.append(f"{label} missing {needle}")

LOOK = ROOT / "Source/Holypaw/Look/HolypawLook.cpp"
if not LOOK.exists():
    errors.append("missing HolypawLook.cpp presentation bible")
else:
    look = LOOK.read_text()
    for needle in ("GradeVolume", "DressSun", "DressClouds", "DressCamera", "Paint", "Lantern", "SampleHour", "AimCatchlight"):
        if needle not in look:
            errors.append(f"look missing {needle}")

if "r.ContactShadows" not in ENGINE:
    errors.append("DefaultEngine missing contact shadows")
if "InnerEarL" not in (ROOT / "Source/Holypaw/Character/HolypawCharacter.h").read_text():
    errors.append("teddy missing inner-ear sockets")
if "Glow" not in (ROOT / "Source/Holypaw/Actors/TravelLantern.cpp").read_text():
    errors.append("lanterns missing point light")
if "EyeL" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("humans missing eyes")
if "CanopyPuffs" not in WORLD:
    errors.append("world missing layered canopies")
if "SampleHour" not in WORLD:
    errors.append("world lighting missing SampleHour")
if "WindowWarm" not in WORLD:
    errors.append("world missing warm windows")
if "TEXT(\"tail\")" not in CHAR and "tail" not in CHAR:
    errors.append("teddy missing tail socket")
if "PawWrap" not in ANIM and "WalkRefSpeed" not in ANIMH:
    errors.append("teddy missing walk-and-wrap limb drive")
if "CottageDoor" not in WORLD:
    errors.append("cottage missing door")
if "22.f, 24.f" not in (LOOK.read_text() if LOOK.exists() else ""):
    errors.append("hour curve missing 22h night key")
if "Hood" not in (ROOT / "Source/Holypaw/Actors/TravelLantern.cpp").read_text():
    errors.append("lanterns missing hood")
if "HindL" not in (ROOT / "Source/Holypaw/Actors/WildFluffy.cpp").read_text():
    errors.append("fluffies missing hind paws")
if "Cloak" not in (ROOT / "Source/Holypaw/Actors/HostilePet.cpp").read_text():
    errors.append("villains missing cloak")
if "Bangs" not in (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text():
    errors.append("humans missing bangs")
if "PlaceBed" not in INTERIOR:
    errors.append("interior kit missing PlaceBed furniture")
if "PlaceCampKit" not in INTERIOR:
    errors.append("interior kit missing camp kit")
if "PlaceAwningStall" not in INTERIOR:
    errors.append("interior kit missing awning stall")
if "DressCityPlaza" not in WORLD:
    errors.append("towns missing plaza furniture")
if "PlaceClothesline" not in WORLD:
    errors.append("cottage yard missing clothesline")
if "CottageHearth" not in INTERIOR:
    errors.append("cottage missing hearth")

# UHT / editor-rebuild hygiene. The "missing module" dialog hides these.
for header in (ROOT / "Source/Holypaw").rglob("*.h"):
    text = header.read_text()
    if ".generated.h" not in text:
        continue
    includes = re.findall(r'#include\s+"([^"]+)"', text)
    if includes and not includes[-1].endswith(".generated.h"):
        errors.append(f"{header.relative_to(ROOT)}: last include must be .generated.h (found {includes[-1]})")
    if "HolypawProcAnim.h" in text:
        errors.append(f"{header.relative_to(ROOT)} includes HolypawProcAnim.h — keep that namespace header out of UHT")

for name in (
    "Character/HolypawCharacter.h",
    "Actors/HugHuman.h",
    "Actors/WildFluffy.h",
):
    text = (ROOT / "Source/Holypaw" / name).read_text()
    if "TUniquePtr<" not in text:
        errors.append(f"{name} should pimpl motion with TUniquePtr")

targets = (ROOT / "Source/Holypaw.Target.cs").read_text() + (ROOT / "Source/HolypawEditor.Target.cs").read_text()
if "EngineIncludeOrderVersion.Oldest" not in targets:
    errors.append("Target.cs should use EngineIncludeOrderVersion.Oldest so 5.8 IWYU Latest does not fail the live rebuild")
uproject = (ROOT / "Holypaw.uproject").read_text()
if '"ModelContextProtocol"' in uproject and re.search(
    r'"Name":\s*"ModelContextProtocol"[\s\S]*?"Enabled":\s*true', uproject
):
    errors.append("ModelContextProtocol should stay disabled so a missing plugin cannot block editor load")
if not (ROOT / "Tools/BuildHolypaw.bat").exists():
    errors.append("missing Tools/BuildHolypaw.bat")

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
