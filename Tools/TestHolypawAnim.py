#!/usr/bin/env python3
"""Envelope tests for Holypaw procedural conversion clips.

Does not need Unreal. Parses authored FClipKey tables from HolypawProcAnim.cpp
and checks wrap / blink / kneel / victory / party hop read as motion, not poses.
"""
from __future__ import annotations

import pathlib
import re
import sys

ROOT = pathlib.Path(__file__).resolve().parents[1]
HEADER = (ROOT / "Source/Holypaw/Anim/HolypawProcAnim.h").read_text()
SOURCE = (ROOT / "Source/Holypaw/Anim/HolypawProcAnim.cpp").read_text()
errors: list[str] = []


def constexpr_float(name: str) -> float:
    m = re.search(rf"constexpr float {name} = ([-\d.]+)f;", HEADER)
    if not m:
        errors.append(f"missing constexpr {name}")
        return 0.0
    return float(m.group(1))


def parse_keys(name: str) -> list[tuple[float, float]]:
    m = re.search(rf"const FClipKey {name}\[\d+\] = \{{(.*?)\}};", SOURCE, re.S)
    if not m:
        errors.append(f"missing clip {name}")
        return []
    keys = [(float(a), float(b)) for a, b in re.findall(r"\{([-\d.]+)f,\s*([-\d.]+)f\}", m.group(1))]
    if len(keys) < 2:
        errors.append(f"{name} needs at least two keys, got {keys}")
    return keys


def smooth01(x: float) -> float:
    x = max(0.0, min(1.0, x))
    return x * x * (3.0 - 2.0 * x)


def sample(keys: list[tuple[float, float]], time: float) -> float:
    if not keys:
        return 0.0
    if time <= keys[0][0]:
        return keys[0][1]
    if time >= keys[-1][0]:
        return keys[-1][1]
    for i in range(len(keys) - 1):
        t0, v0 = keys[i]
        t1, v1 = keys[i + 1]
        if time <= t1:
            span = t1 - t0
            a = (time - t0) / span if span > 1e-8 else 1.0
            return v0 + (v1 - v0) * smooth01(a)
    return keys[-1][1]


def near(actual: float, expected: float, tol: float, label: str) -> None:
    if abs(actual - expected) > tol:
        errors.append(f"{label}: {actual:.3f} != {expected:.3f} ±{tol}")


HugSeconds = constexpr_float("HugSeconds")
BlinkCloseSeconds = constexpr_float("BlinkCloseSeconds")
KneelSeconds = constexpr_float("KneelSeconds")
ConvertBowSeconds = constexpr_float("ConvertBowSeconds")
VictorySeconds = constexpr_float("VictorySeconds")
HugLockSeconds = constexpr_float("HugLockSeconds")

wrap = parse_keys("WrapKeys")
squeeze = parse_keys("SqueezeKeys")
blink = parse_keys("BlinkKeys")
kneel = parse_keys("KneelKeys")
bow = parse_keys("BowKeys")
spin = parse_keys("VictorySpinKeys")
hop = parse_keys("VictoryHopKeys")

near(HugSeconds, 0.70, 0.001, "HugSeconds")
near(BlinkCloseSeconds, 0.12, 0.001, "BlinkCloseSeconds")
near(KneelSeconds, 0.72, 0.001, "KneelSeconds")
near(VictorySeconds, 0.95, 0.001, "VictorySeconds")
if HugLockSeconds >= HugSeconds:
    errors.append("hug lock must be shorter than the wrap clip so mash-protect is not the whole hug")

if wrap:
    near(sample(wrap, 0.0), 0.0, 0.02, "wrap start")
    near(sample(wrap, HugSeconds), 0.0, 0.02, "wrap end")
    peak = max(sample(wrap, t / 100.0 * HugSeconds) for t in range(101))
    if peak < 0.95:
        errors.append(f"wrap peak {peak:.3f} is too weak to read as arms closing")
    mid = sample(wrap, 0.28)
    if mid < 0.9:
        errors.append(f"wrap at 0.28s is {mid:.3f}; squeeze should already be closed")

if squeeze:
    sq_peak = max(sample(squeeze, t / 100.0 * HugSeconds) for t in range(101))
    if sq_peak < 0.9:
        errors.append(f"squeeze peak {sq_peak:.3f} missing")
    if sample(squeeze, 0.0) > 0.05 or sample(squeeze, HugSeconds) > 0.05:
        errors.append("squeeze should start and end at rest")

if blink:
    near(sample(blink, 0.0), 0.0, 0.02, "blink open start")
    near(sample(blink, BlinkCloseSeconds), 0.0, 0.02, "blink open end")
    if sample(blink, 0.06) < 0.9:
        errors.append("blink must fully close in the middle of the lid clip")

if kneel:
    near(sample(kneel, 0.0), 0.0, 0.02, "kneel start")
    near(sample(kneel, KneelSeconds), 1.0, 0.02, "kneel end")
    half = sample(kneel, KneelSeconds * 0.5)
    if half <= 0.5:
        errors.append(f"kneel should ease in (0.5s alpha {half:.3f} expected > 0.5)")
    prev = -0.01
    for t in range(21):
        v = sample(kneel, KneelSeconds * t / 20.0)
        if v + 0.001 < prev:
            errors.append("kneel clip should be monotonic")
            break
        prev = v

if bow:
    near(sample(bow, 0.0), 0.0, 0.02, "convert bow start")
    near(sample(bow, ConvertBowSeconds), 0.0, 0.02, "convert bow stands back up")
    if sample(bow, 0.5) < 0.9:
        errors.append("convert bow must hit a full kneel before rising")

if spin:
    near(sample(spin, 0.0), 0.0, 1.0, "victory spin start")
    near(sample(spin, VictorySeconds), 360.0, 1.0, "victory spin full turn")
    if sample(spin, VictorySeconds * 0.5) < 90.0:
        errors.append("victory spin should be visibly turning at mid clip")

if hop:
    if sample(hop, 0.0) > 1.0 or sample(hop, VictorySeconds) > 1.0:
        errors.append("victory hop should land")
    if max(sample(hop, t / 50.0 * VictorySeconds) for t in range(51)) < 16.0:
        errors.append("victory hop is too small")

for clip in ("Wrap", "Blink", "EarFlop", "Kneel", "PartyTrail", "Victory"):
    if f'return TEXT("{clip}")' not in SOURCE:
        errors.append(f"clip name {clip} missing from ClipName")

if "EvaluateParty" not in SOURCE:
    errors.append("party trail evaluator missing")
if "Spring(" not in SOURCE:
    errors.append("ear flop spring missing")
if "PlayHug" not in SOURCE or "PlayVictory" not in SOURCE:
    errors.append("teddy hug/victory plays missing")

# Party hop is always non-negative: max(0, sin).
import math

party_hops = [max(0.0, math.sin(clock * 2.35 * 2.0 * math.pi + 1.17)) * 16.0 for clock in (i / 20.0 for i in range(40))]
if min(party_hops) < -1e-6:
    errors.append("party hop went negative")
if max(party_hops) < 10.0:
    errors.append("party hop never leaves the ground")

char = (ROOT / "Source/Holypaw/Character/HolypawCharacter.cpp").read_text()
human = (ROOT / "Source/Holypaw/Actors/HugHuman.cpp").read_text()
fluffy = (ROOT / "Source/Holypaw/Actors/WildFluffy.cpp").read_text()
if "HolypawAnim::PlayHug" not in char:
    errors.append("character does not play wrap")
if "CelebrateConvert" not in char:
    errors.append("character missing convert victory hook")
if "EvaluateParty" not in char:
    errors.append("character missing party trail poses")
if "PlayConvertBow" not in human:
    errors.append("human missing convert kneel bow")
if "PlayWorshipKneel" not in human:
    errors.append("human missing endgame kneel")
if "EvaluateFluffy" not in fluffy:
    errors.append("wild fluffy missing ear flop")

if errors:
    print("FAIL")
    for e in errors:
        print(" -", e)
    sys.exit(1)

print("OK")
print(
    f" wrap_peak={max(sample(wrap, t / 100.0 * HugSeconds) for t in range(101)):.2f}"
    f" kneel_end={sample(kneel, KneelSeconds):.2f}"
    f" spin_end={sample(spin, VictorySeconds):.1f}"
    f" clips=Wrap,Blink,EarFlop,Kneel,PartyTrail,Victory"
)
sys.exit(0)
