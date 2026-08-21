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
    "PlayerBattleAttack(TEXT(\"guard\"))",
    "PlayerBattleAttack(TEXT(\"hymn\"))",
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
if "EnhancedInput.EnhancedPlayerInput" not in INPUT:
    errors.append("DefaultInput not using Enhanced Input player class")

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
