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


villain_enum = enum_names(TYPES, "EHolypawVillain")
mission_enum = enum_names(TYPES, "EHolypawMission")
catalog_ids = find_ids(VILLAINS, "EHolypawVillain")
spawn_ids = find_ids(WORLD, "EHolypawVillain")
mission_ids = find_ids(MISSIONS, "EHolypawMission")
skill_ids = re.findall(r'TEXT\("([a-zA-Z]+)"\),\s*TEXT\("([^"]+)"\)', SKILLS)

if "Sanrio" in CHAR or "Hello Kitty" in CHAR or "Sanrio" in MISSIONS:
    errors.append("copyright: Sanrio/Hello Kitty string found — keep Poly Mill original")

missing_cat = [n for n in villain_enum if n not in catalog_ids]
if missing_cat:
    errors.append(f"villain catalog missing {missing_cat}")

missing_spawn = [n for n in villain_enum if n not in spawn_ids]
if missing_spawn:
    errors.append(f"world spawn missing {missing_spawn}")

if len(villain_enum) != 40:
    errors.append(f"expected 40 villains, found {len(villain_enum)}")

if len(mission_enum) != 12:
    errors.append(f"expected 12 missions, found {len(mission_enum)}")

missing_mission = [n for n in mission_enum if n not in mission_ids]
if missing_mission:
    errors.append(f"mission catalog missing {missing_mission}")

trees = SKILLS.count("EHolypawSkillTree::Hug") + SKILLS.count("EHolypawSkillTree::Miracle") + SKILLS.count("EHolypawSkillTree::Party")
# Add() calls per tree
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

if errors:
    print("FAIL")
    for e in errors:
        print(" -", e)
    sys.exit(1)

print("OK")
print(f" villains={len(villain_enum)} spawned={len(spawn_ids)} missions={len(mission_enum)} skills hug/miracle/party={hug}/{miracle}/{party}")
sys.exit(0)
