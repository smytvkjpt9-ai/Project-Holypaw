# The Fluffy Ascendancy (Project Holypaw)

Unreal Engine **5.8** C++ prototype. Same painterly / plush look everywhere: forest cottage, lantern road, Ribbon City, Velvet Peak. No Megascans. No Pokémon IP.

The old browser toy is in [`legacy/web-prototype/index.html`](legacy/web-prototype/index.html).

## What Phase 1 is

You wake **inside a forest cottage clearing**, follow lanterns into a **blockout Ribbon City**, recruit fluffies, hug people, fight hostile pets, spend Affection, fire a Miracle. Extra biomes are tinted regions on the same terrain, not fully dressed.

World layout is spawned at **Play** by `AHolypawWorldBuilder` (no binary `.umap` required).

## Open in Unreal 5.8

1. Install **Unreal Engine 5.8** (Epic Launcher).
2. Install Visual Studio 2022 with **Game development with C++** (Windows) or Xcode (Mac).
3. Right-click `Holypaw.uproject` → **Generate Visual Studio project files**.
4. Open `Holypaw.uproject`. Let it compile the `Holypaw` module.
5. Press **Play**. You should spawn on the cottage porch.

Plugins already enabled in the `.uproject`:

- Python Editor Script Plugin
- Procedural Mesh Component
- Enhanced Input (plugin on; Phase 1 still uses classic axis mappings so it runs without Input Action assets)
- **Model Context Protocol** (Epic, experimental). Auto-start is **off** so the editor does not hang without a client.

### Optional: save a map

With the project compiled, in Output Log:

```
py "Tools/GenerateWorld.py"
```

That places a World Builder actor and tries to save the current level. Play still generates geometry at BeginPlay.

## Controls

| Input | Action |
| --- | --- |
| WASD / left stick | Move |
| Mouse / right stick | Look |
| Space | Jump |
| E | Interact (recruit, hug, rest) |
| K | Skill tree (1–6 to buy) |
| P | Party |
| M | Miracle (when the bar is full) |
| 1–4 in battle | Slap / Cuddle Beam / Party Assault / Flee |

## Hook Cursor MCP later (on your PC)

Epic’s 5.8 MCP plugin is a **server only**. It does not place actors until you add tools.

1. Edit → Plugins → enable **Model Context Protocol** (already on in this project).
2. Enable **Python Editor Script Plugin**.
3. Console: `ModelContextProtocol.StartServer`
4. Default URL: `http://127.0.0.1:8000/mcp`
5. Point Cursor at that URL and add a toolkit (for example UnrealMCPToolkit) for `execute_python`.

Do not expect this cloud repo to talk to your local editor by itself.

## Art rules

- One look for the whole game (cottage, woods, city, mountain).
- No Quixel / Megascans / Fab photoscans.
- Plush palette: rose, mint, gold, powder blue — not Hisui mud, not Warcraft stone.

## Later (not this pass)

Dressed biomes, city interiors, denser foliage, water, packaged shipping build, wiring Cursor from this VM.
