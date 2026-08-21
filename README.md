# The Fluffy Ascendancy (Project Holypaw)

Unreal Engine **5.8** C++ prototype. Same painterly / plush look everywhere. No Megascans. No Pokémon IP.

You wake in a **forest cottage**, follow lanterns into **Ribbon City**, then out to four more settlements. The old browser toy is in [`legacy/web-prototype/index.html`](legacy/web-prototype/index.html).

## The living world (generated at Play)

| Place | What it is |
| --- | --- |
| Stuffed Park cottage | Home. Heal on the porch. |
| Ribbon City | Main city: plaza, spire, market stall, people |
| Tidewell | Harbor town on the coast, docks, water |
| Hearthfold | Farm town in the homestead hills |
| Emberfen | Stilted village in Crimson Hollow |
| Snowveil | Alpine hamlet on the way to the peak |
| Velvet Peak | Sacred mountain and shrine. **Velvet Tyrant** and **The Unmaker** |

Roads with lanterns connect cottage → Ribbon City → the other towns. Press **N** for distances. Press **V** for the Villain Codex.

## Villains (all 40 in the world)

The four hostiles from the HTML prototype (Scrap Dog, Corp Cat, Razor Petbot, Void Rat) plus regional packs, elites, bosses, and two world bosses. Each has a catalog id, look, special, intro/attack/defeat lines, and Codex entry.

| Region | Boss | Special |
| --- | --- | --- |
| Ribbon City | Silk Magistrate | Faith Burn, blocks flee |
| Tidewell | Brine Warden | Double Strike, blocks flee |
| Hearthfold | Harvest Overseer | Cut Party, blocks flee |
| Emberfen | Bog King | Poison Thread, blocks flee |
| Snowveil | Aurora Warden | Frostbite, blocks flee |
| Velvet Peak | Velvet Tyrant (world) | Rage |
| Velvet Peak | The Unmaker (world) | Faith Burn |

Specials: Rip, Drain Faith, Eat Miracle, Double Strike, Armor Plates, Frostbite, Poison Thread, Rage, Cut Party, Faith Burn. First sighting logs the Codex. Boss kills pay extra AP/FP and mend stuffing.

## Open in Unreal 5.8

1. Install **Unreal Engine 5.8** and a C++ toolchain (VS 2022 Game development / Xcode).
2. Right-click `Holypaw.uproject` → Generate project files.
3. Open the project, compile `Holypaw`, press **Play**.

Plugins in the `.uproject`: Python Editor Script, Procedural Mesh, Enhanced Input, Model Context Protocol (optional).

Optional editor helper: `py "Tools/GenerateWorld.py"` after compile.

## Controls

| Input | Action |
| --- | --- |
| WASD / left stick | Move |
| Mouse / right stick | Look |
| Space | Jump |
| E | Interact (recruit, hug, rest, stall, sign, challenge) |
| K | Skill tree (1–6 to buy) |
| P | Party |
| M | Miracle (bar full) |
| N | Survey map |
| V | Villain Codex |
| Esc | Close menus |
| 1–4 in battle | Slap / Cuddle Beam / Party Assault / Flee |

Faith stalls in each city: **10 AP → 8 FP**.

## Art

One look for cottage, woods, every city, snow, and coast. No Quixel / Megascans. Plush palette (rose, mint, gold, powder blue). Villains use the same painterly shapes, just meaner colors.

## Cursor MCP later (your PC)

`ModelContextProtocol.StartServer` → `http://127.0.0.1:8000/mcp`. See `.cursor/mcp.json.example`.
