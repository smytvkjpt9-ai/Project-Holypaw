# The Fluffy Ascendancy (Project Holypaw)

Unreal Engine **5.8** C++ prototype. Same painterly / plush look everywhere. No Megascans. No Pokémon IP. No Sanrio IP.

You are a **handmade teddy**. The **Poly Mill** mass-produces cheap polyester copies and wants the world identical and unloved. You hug people into the **Bear Faith**, recruit fluffies, and unstuff Poly officers until humans keep handmade toys and drop the factory empire.

The old browser toy is in [`legacy/web-prototype/index.html`](legacy/web-prototype/index.html).

## Campaign (J journal)

Twelve missions, start to finish:

1. Wake in Stuffed Park
2. Recruit a fluffy
3. Unstuff a Scrap Dog
4. Fire your first Miracle (M)
5. Reach Ribbon City
6. Convert three people by hugging
7. Defeat Silk Magistrate (Poly Court)
8. Visit Tidewell, Hearthfold, Emberfen, Snowveil
9. Four Rites — the four regional bosses
10. Velvet Tyrant on the peak
11. The Unmaker
12. Miracle on Velvet Peak — people kneel, Poly Mill falls

## Combat

Camera pulls in. Commands: **1** Slap **2** Beam **3** Party **4** Flee **5** Guard **6** Hymn. Poly Mill enemies take extra damage from **Poly Rip**.

## Three skill trees (K, Tab to cycle)

Hug, Miracle, Party — six skills each (1–6 to buy the current tree).

## Art honesty

This cloud VM cannot sculpt Nanite characters or ship a production FBX pipeline. Actors are **assembled engine primitives** (ears, snout, paws, eyes, human heads/arms, fluffy tails) in the Holypaw palette. Drop real meshes in Unreal on your PC later; the C++ sockets are already named.

## Testing without the editor

```
python3 Tools/ValidateHolypaw.py
```

Checks villain/mission catalogs, world spawns, skill-tree counts, and campaign hooks. Playtesting still needs UE 5.8 on your machine.

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
