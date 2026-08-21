# The Fluffy Ascendancy (Project Holypaw)

Unreal Engine **5.8** C++ prototype. Same painterly / plush look everywhere. No Megascans. No Pokémon IP. No Sanrio IP.

You are a **handmade teddy**. The **Poly Mill** mass-produces cheap polyester copies and wants the world identical and unloved. You hug people into the **Bear Faith**, recruit fluffies, and unstuff Poly officers until humans keep handmade toys and drop the factory empire.

The world is a **plush Earth**: continents, oceans, deserts, ice caps, and **26 cities** generated at Play from `HolypawAtlas.cpp` (no binary `.umap`). Opening route is still the forest cottage in Stuffed Park → lantern road → Ribbon City.

The old browser toy is in [`legacy/web-prototype/index.html`](legacy/web-prototype/index.html).

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

Wild biomes (forest, homestead, mire, coast, desert, jungle, ocean, ice) fill the land between cities. Velvet Peak, Andes Loom, and the Silk Ridge are the mountain ranges.

## Combat

Camera pulls in. Commands: **1** Slap **2** Beam **3** Party **4** Flee **5** Guard **6** Hymn. Poly Mill enemies take extra damage from **Poly Rip**.

## Three skill trees (K, Tab to cycle)

Hug, Miracle, Party — six skills each (1–6 to buy the current tree).

## Art honesty

This cloud VM cannot sculpt Nanite characters or ship a production FBX pipeline. Actors are **assembled engine primitives** (ears, snout, paws, eyes, human heads/arms, fluffy tails) in the Holypaw palette. Towns, roads, and seas use **instanced kits** so the globe can stay large. Drop real meshes in Unreal on your PC later; the C++ sockets are already named.

## Testing without the editor

```
python3 Tools/ValidateHolypaw.py
```

Checks villain/mission catalogs, 26-city atlas bounds, world spawns, skill-tree counts, and campaign hooks. Playtesting still needs UE 5.8 on your machine.

## Controls

| Input | Action |
| --- | --- |
| WASD / mouse | Move / look |
| E | Interact |
| K | Skill trees (Tab cycles) |
| J | Journal |
| P | Party |
| M | Miracle |
| N | Map |
| V | Villain Codex |
| Esc | Close menus |
| 1–6 in battle | Slap / Beam / Party / Flee / Guard / Hymn |

Faith stalls: **10 AP → 8 FP**.

## Open in Unreal 5.8

1. Install **Unreal Engine 5.8** and a C++ toolchain.
2. Right-click `Holypaw.uproject` → Generate project files.
3. Compile `Holypaw`, press **Play**.

Optional: `py "Tools/GenerateWorld.py"` after compile. MCP later: `ModelContextProtocol.StartServer` → see `.cursor/mcp.json.example`.
