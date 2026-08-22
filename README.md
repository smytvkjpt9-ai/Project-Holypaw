# The Fluffy Ascendancy (Project Holypaw)

Unreal Engine **5.8** C++ prototype. Same painterly / plush look everywhere. No Megascans. No Pokémon IP. No Sanrio IP.

You are a **handmade teddy**. The **Poly Mill** mass-produces cheap polyester copies and wants the world identical and unloved. You hug people into the **Bear Faith**, recruit fluffies, and unstuff Poly officers until humans keep handmade toys and drop the factory empire.

**Opening (locked):** you spawn on the porch of a small forest cottage in Stuffed Park. Velvet Peak sits on the horizon. Follow the lantern road into **Ribbon City** (Plaza, Market, Cloth Quarter, Harbor Steps, Quiet Rows). You do not spawn in the plaza.

The world is also a **plush Earth**: continents, oceans, deserts, ice caps, and **26 cities** generated at Play from `HolypawAtlas.cpp` (no binary `.umap`).

**The joke:** you hug humans until their last serious thought falls out. **Hearts** are not a score — they change the city. First Heart opens shops. Third Heart drops mill banners and unlocks the dusk hymn. Believers clap and walk. Miracles sermon the neighborhood. It is cute. It is also a coup.

The old browser toy is archived at [`legacy/web-prototype/index.html`](legacy/web-prototype/index.html).

**AAA production plan:** [`Docs/AAA_200K_PRODUCTION_PLAN.md`](Docs/AAA_200K_PRODUCTION_PLAN.md) — 200,000+ lines of real systems and city/dialogue/combat content, not padding. Track with `python3 Tools/CountHolypawLoc.py`. **Waves 1–3 are in this branch:** title + 3 save slots, city Hearts, lantern travel, believer talk, shops, combat statuses, Ribbon shrines, day/night lighting. This cloud VM still cannot sculpt Nanite or ship an `.exe`; packaging stays on your PC.

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
5. Press **Play**. The title overlay asks for a slot (1–3, Enter / N new / L load). Then wake at the forest cottage, walk inside for bed and kitchen, recruit a fluffy, hug the ranger, follow lanterns to Ribbon City. **F5** saves. Resting at the cottage, a camp, or a walk-in **inn** also saves. Gold **lanterns** open map travel (Tab to pick, E to hop). Converted people **clap, walk, and talk**. Stalls stay shuttered until a Heart opens them (**I** for pockets). Walk into a **chapel** to fill Miracle Charge. **Fountain** wishes grant FP. Three Hearts in Ribbon drop the mill banners; dusk then plays a hymn pad. The sun, fog, mill smog, and a TEMP city-theme pad follow the clock. F6 mutes the temp stingers. In battle, **Tab** flips the overflow command page.
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

Turn-based overlay (C++ `UHolypawBattleWidget`). Camera pulls in. Commands **1–6** on the current page. **Tab in battle** cycles three pages: basics, overflow (Unstuff / Button Beam / …), and party tricks (Cheer / Tuck / Fluff Burst / Mill Hymn / Ribbon Bind / Spare Bun). Recruited fluffies have **roles** (Choir, Brace, Ripper, Scout) that change Party Assault, flee, and heals. Elites and bosses enter **phase two**; campaign bosses run `HolypawBossScript`. Frost / poison / hymn shield / rip DoT / mill polyester show on the overlay.

## Three skill trees (K, Tab to cycle)

Hug, Miracle, Party — six skills each (1–6 to buy the current tree). Phase 1 flags include Soft Fur, Button Eyes, Halo Step, Miracle Echo, Party Bond, Fluff Shield. Party size max **4**.

## Art honesty

This cloud VM cannot sculpt Nanite characters or ship a production FBX pipeline. Actors are **assembled engine primitives** (ears, inner ears, snout, nose, catchlights, arms, legs, paws, human hair/eyes/hats, fluffy tails) in the Holypaw palette (rose, mint, gold, powder blue). Towns use **instanced kits** with windows, chimneys, and layered canopies. Lumen GI, virtual shadows, contact shadows, Sky Atmosphere, volumetric clouds, fill + moon lights, lantern point lights, and a painterly post-process grade. **Zero Quixel / Megascans / Fab photoscans.** Named sockets (`earL`, `snout`, `halo`, `pawL`, `body`) are the FBX drop contract on your PC.

**Animation is a separate system** (`Source/Holypaw/Anim/HolypawProcAnim.cpp`). Pretty meshes still feel like a prototype if they do not move. The conversion fantasy is authored clips: **wrap**, **blink**, **ear flop**, **kneel**, **party trail**, **victory**. A convert is one beat: reach, wrap, hold through the bow, authored release, then spin. The teddy steps in. The human does not kneel until the wrap closes. Party squash is a land hit, not a pancake rest. Clip names stay the contract for later skeletal anims.

## Testing without the editor

```
python3 Tools/ValidateHolypaw.py
python3 Tools/TestHolypawAnim.py
python3 Tools/CountHolypawLoc.py
```

Checks villain/mission catalogs, 26-city atlas bounds, UMG overlay, Ribbon districts, skill-tree counts, plugins, and campaign hooks. The loc counter tracks unique source lines against the 200k AAA floor. Playtesting still needs UE 5.8 on your machine.

## Controls

| Input | Action |
| --- | --- |
| WASD / mouse | Move / look |
| Space | Jump |
| E | Interact (cottage rest, recruit, hug, lantern travel, inn/chapel/workshop/fountain/crate, title confirm) |
| Enter | Title continue / pause resume |
| 1–3 on title | Select save slot |
| N on title | New game on selected slot |
| L on title | Load selected slot |
| F5 | Quick save |
| F6 | Mute (settings stub) |
| F8 | Return to title (saves first) |
| I | Pockets (use snacks) |
| Esc | Pause / close menus |
| K | Skill trees (Tab cycles) |
| J | Journal |
| P | Party |
| M | Miracle |
| N | Map (Tab cycles lanterns, E hops) |
| V | Villain Codex |
| 1–6 in battle | Current page (Tab cycles basic / overflow) |

Faith stalls: shutters stay down until the city has a Heart. **E** then opens a shop (faith jars, stuffing buns, hymn ribbons). Convert locals for a Hearts discount. **I** opens pockets. 1–4 buys/uses.

Converted humans **clap and walk**, then **talk**. **1** listen **2** ask the way **3** turn in **4** take a job. Some believers have a **third quiet line** if your Faith is high enough. Journal (**J**) lists errands across Home Sewn, Globe Trek, Old World, and the Sand Belt.

Dressed cities: **all 26**. Each has districts, a walk-in inn, a walk-in shop, a font or chapel, a pickup, and an errand. The **Poly Mill** is a hall you enter (conveyors, vats, west door toward Ribbon). **Ribbon conversion is visible in the plaza:** an open fountain kiosk unlatches (no shop room on the fountain), nearby mill ads sag then fall, the fountain jet bobs and turns rose, converted people clap in place then walk, shopkeeps stay at the counter they just opened, nearby skeptics look over, signs rewrite, dusk hymn grows as you near the chapel. Hearts credit the convert's home city. Humans follow a **day / dusk / night** beat: wander, chapel hymn, inn pile-in. Talk runs through `HolypawDialogueVM`. Combat pages, roles, and boss kits run through the director. City theme pads are procedural sines (TEMP). Still not 24 NPCs per city, Nanite, or a Shipping `.exe`.
