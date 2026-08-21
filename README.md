# The Fluffy Ascendancy (Project Holypaw)

Unreal Engine **5.8** C++ prototype. Same painterly / plush look everywhere. No Megascans. No Pokémon IP. No Sanrio IP.

You are a **handmade teddy**. The **Poly Mill** mass-produces cheap polyester copies and wants the world identical and unloved. You hug people into the **Bear Faith**, recruit fluffies, and unstuff Poly officers until humans keep handmade toys and drop the factory empire.

**Opening (locked):** you spawn on the porch of a small forest cottage in Stuffed Park. Velvet Peak sits on the horizon. Follow the lantern road into **Ribbon City** (Plaza, Market, Cloth Quarter, Harbor Steps, Quiet Rows). You do not spawn in the plaza.

The world is also a **plush Earth**: continents, oceans, deserts, ice caps, and **26 cities** generated at Play from `HolypawAtlas.cpp` (no binary `.umap`).

**The joke:** you hug humans until their last serious thought falls out. The HUD **Hearts** counter is how many people now believe a teddy should run things. Miracles sermon the neighborhood. It is cute. It is also a coup.

The old browser toy is archived at [`legacy/web-prototype/index.html`](legacy/web-prototype/index.html).

## Open in Unreal 5.8 (required)

This cloud repo is source + a runtime world builder, not a double-click `.exe`.

1. Install **Unreal Engine 5.8** via the Epic Launcher (Windows recommended).
2. Right-click `Holypaw.uproject` → **Generate Visual Studio project files** (or Xcode on Mac).
3. Open the project, compile the `Holypaw` module.
4. Optional editor helper (after compile), from the Output Log:

   ```
   py "Tools/GenerateWorld.py"
   ```

   That places `AHolypawWorldBuilder` in the current level. Play still generates cottage, path, city, and biomes at BeginPlay even if you skip this.
5. Press **Play**. Wake at the forest cottage, recruit a fluffy, hug the ranger, follow lanterns to Ribbon City.
6. Later, on your PC: `ModelContextProtocol.StartServer` → Cursor MCP at `http://127.0.0.1:8000/mcp` (see `.cursor/mcp.json.example`). Epic’s stock plugin is a server framework; add tools or Python locally if you want the Editor to place actors from Cursor.

Plugins already enabled: **PythonScriptPlugin**, **EnhancedInput**, **ProceduralMeshComponent**, **ModelContextProtocol** (optional).

## Campaign (J journal)

Thirteen missions, start to finish:

1. Wake in Stuffed Park
2. Recruit a fluffy
3. Unstuff a Scrap Dog
4. Fire your first Miracle (M)
5. Reach Ribbon City
6. Convert three people by hugging
7. Defeat Silk Magistrate (Poly Court)
8. Visit Tidewell, Hearthfold, Emberfen, Snowveil
9. Globe Trek — Lantern Angeles, Palma Dusk, Cherry Loom
10. Four Rites — the four regional bosses
11. Velvet Tyrant on the peak
12. The Unmaker
13. Miracle on Velvet Peak — people kneel, Poly Mill falls

## Plush Earth (N map)

Atlas is the single source of truth for cities, roads, and landmasses. The world builder instances towns, roads, and water from those tables.

| Continent | Cities |
| --- | --- |
| Home Sewn | Ribbon City, Tidewell, Hearthfold, Emberfen, Snowveil, Lantern Angeles, Mossgate, Quiltland, Dust Mesa |
| South Sewn | Carnival Bahia, Andes Loom |
| Old World | Clockhaven, Velvet Seine, Marble Forum |
| Sand Belt | Palma Dusk, Ivory Spire, Sand Hymn, Cape Plush, Savannah Bell |
| Silk East / Cherry Isle | Silk Delta, Spice Harbor, Cherry Loom |
| Coral South | Coral Choir |
| Felt North / South | Aurora Borough, Tundra Parish, Felt Ice Camp |

Wild biomes fill the land between cities: Stuffed Park, Nursery Hills, Cozy Homestead, Ribbon Coast, Crimson Hollow, Alabaster Ridge, plus desert, jungle, ocean, and ice. Velvet Peak has a shrine and a **sky rift**. Andes Loom and the Silk Ridge are extra ranges.

## Combat (UMG)

Turn-based overlay (C++ `UHolypawBattleWidget`). Camera pulls in. Commands: **1** Soft Slap **2** Cuddle Beam **3** Party Assault **4** Flee **5** Guard **6** Hymn. Poly Mill enemies take extra damage from **Poly Rip**.

## Three skill trees (K, Tab to cycle)

Hug, Miracle, Party — six skills each (1–6 to buy the current tree). Phase 1 flags include Soft Fur, Button Eyes, Halo Step, Miracle Echo, Party Bond, Fluff Shield. Party size max **4**.

## Art honesty

This cloud VM cannot sculpt Nanite characters or ship a production FBX pipeline. Actors are **assembled engine primitives** (ears, snout, paws, eyes, human heads/arms, fluffy tails) in the Holypaw palette. Towns, roads, and seas use **instanced kits**. Sky Atmosphere, volumetric clouds, height fog, and a pastel post-process grade. **Zero Quixel / Megascans / Fab photoscans.** Drop real meshes in Unreal on your PC later; the C++ sockets are already named.

## Testing without the editor

```
python3 Tools/ValidateHolypaw.py
```

Checks villain/mission catalogs, 26-city atlas bounds, UMG overlay, Ribbon districts, skill-tree counts, plugins, and campaign hooks. Playtesting still needs UE 5.8 on your machine.

## Controls

| Input | Action |
| --- | --- |
| WASD / mouse | Move / look |
| Space | Jump |
| E | Interact (cottage rest, recruit, hug) |
| K | Skill trees (Tab cycles) |
| J | Journal |
| P | Party |
| M | Miracle |
| N | Map |
| V | Villain Codex |
| Esc | Close menus |
| 1–6 in battle | Slap / Beam / Party / Flee / Guard / Hymn |

Faith stalls: **10 AP → 8 FP**.
