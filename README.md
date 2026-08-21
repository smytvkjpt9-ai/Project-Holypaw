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
| Velvet Peak | Sacred mountain and shrine. **Velvet Tyrant** (alpha) nearby |
| Survey camps | Heal tents on the roads between cities |

Roads with lanterns connect cottage → Ribbon City → the other towns. Press **N** for distances.

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
| E | Interact (recruit, hug, rest, stall, sign) |
| K | Skill tree (1–6 to buy) |
| P | Party |
| M | Miracle (bar full) |
| N | Survey map |
| 1–4 in battle | Slap / Cuddle Beam / Party Assault / Flee |

Faith stalls in each city: **10 AP → 8 FP**.

## Art

One look for cottage, woods, every city, snow, and coast. No Quixel / Megascans. Plush palette (rose, mint, gold, powder blue).

## Cursor MCP later (your PC)

`ModelContextProtocol.StartServer` → `http://127.0.0.1:8000/mcp`. See `.cursor/mcp.json.example`.
