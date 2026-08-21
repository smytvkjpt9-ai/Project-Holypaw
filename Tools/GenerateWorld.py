#!/usr/bin/env python3
"""
Holypaw world helper for Unreal Editor 5.8 (Python Editor Script Plugin).

The C++ AHolypawWorldBuilder already generates the world at Play.
This script is optional: it places a WorldBuilder in the current level and saves
/Game/Holypaw/Maps/M_HolypawWorld so the layout is visible in the editor viewport.

Usage (Output Log):
    py "Tools/GenerateWorld.py"

Or from the Editor Python console after opening the project.
"""

from __future__ import annotations

import unreal


WORLD_BUILDER = "/Script/Holypaw.HolypawWorldBuilder"
MAP_PATH = "/Game/Holypaw/Maps/M_HolypawWorld"


def _has_builder(editor_actor_subsystem) -> bool:
    for actor in editor_actor_subsystem.get_all_level_actors():
        if actor.get_class().get_name().startswith("HolypawWorldBuilder"):
            return True
    return False


def run() -> None:
    actor_sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    asset_sub = unreal.get_editor_subsystem(unreal.EditorAssetSubsystem)
    level_sub = unreal.get_editor_subsystem(unreal.LevelEditorSubsystem)

    if not asset_sub.does_directory_exist("/Game/Holypaw"):
        asset_sub.make_directory("/Game/Holypaw")
    if not asset_sub.does_directory_exist("/Game/Holypaw/Maps"):
        asset_sub.make_directory("/Game/Holypaw/Maps")

    if not _has_builder(actor_sub):
        cls = unreal.load_class(None, WORLD_BUILDER)
        if cls is None:
            unreal.log_error("HolypawWorldBuilder class not found. Compile the Holypaw C++ module first.")
            return
        actor_sub.spawn_actor_from_class(cls, unreal.Vector(0.0, 0.0, 0.0), unreal.Rotator(0.0, 0.0, 0.0))
        unreal.log("Spawned AHolypawWorldBuilder at origin.")
    else:
        unreal.log("World builder already in the level.")

    # Persist so you can set this as the editor startup map later.
    if level_sub:
        ok = level_sub.save_current_level()
        unreal.log("Saved current level." if ok else "Could not save current level (save it manually).")

    unreal.log("Holypaw generate helper finished. Press Play — terrain, cottage, path, and city spawn at BeginPlay.")


if __name__ == "__main__":
    run()
