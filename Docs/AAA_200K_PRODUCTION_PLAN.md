# The Fluffy Ascendancy — AAA production plan (200,000+ lines)

**Product:** The Fluffy Ascendancy (Project Holypaw)
**Engine:** Unreal Engine 5.8, C++ first
**Target:** a shippable PC AAA-scoped original — not a prototype, not a Pokémon clone, not a Sanrio parody
**Hard floor:** **200,000 lines** of *real* game code and authored content (C++ / headers / catalogs / tests / tools). Comments, blank lines, and duplicated empty classes do **not** count.

Today the playable module is about **6,200 lines**. That is a vertical slice. This document is the production bible that turns the slice into a title.

---

## 0. What “AAA” means here

AAA is not a line-count trophy. It is:

1. **A complete fantasy** — handmade teddy, hugs, AP/FP/miracles, recruit fluffies, convert humans into the Bear Faith, topple the **Poly Mill**. Cute coup. Never grim brainwashing. Never Pokémon IP. Never Sanrio / Hello Kitty.
2. **A living plush Earth** — 26 named cities that feel like places, not colored cubes on a heightfield.
3. **Combat that has weight** — camera, timing, unique villain kits, boss phases, juice you can feel without a mouse-UI trap.
4. **Systems that remember you** — save/load, city Hearts, shops, inventory, fast travel, settings, accessibility.
5. **Production presentation** — one painterly/plush art bible (rose, mint, gold, powder blue). Sky, lighting, animation, audio, UI that belong in a store page trailer.
6. **A packaged build** on the user’s PC. This cloud VM cannot run Unreal Editor or emit an `.exe`. Code lands here; cook/package happens locally.

**What this VM will never fake:** Nanite film-teddy sculpts, recorded orchestra, mocap, Quixel/Megascans. Those are **socketed** (named components, cue IDs, material parameters) so artists drop assets in UE 5.8 later. Until then, assembled engine primitives + instanced kits stay the look. **Zero photoscans.**

**What 200k lines *will* be:** gameplay systems, per-city simulation, dialogue, quests, combat AI, UI, audio cue tables, items, tests, and generators. Volume comes from **content that plays**, not from padding.

---

## 1. Lock-ins (do not reopen)

| Lock | Rule |
| --- | --- |
| Opening | Spawn on the forest cottage porch in Stuffed Park. Lantern road → Ribbon City. Never spawn in the plaza. |
| World | Generated at Play by `AHolypawWorldBuilder` from `HolypawAtlas.cpp`. No binary `.umap` as source of truth. |
| Input | Classic axis/action mappings. **N** = map. **M** = Miracle. Space = jump. E = interact. Battle stays GameOnly (keys 1–6). |
| Art | One painterly/plush palette. Engine primitives + ISM kits until real meshes exist. |
| Endgame | Poly Mill (cheap polyester factory empire). Humans take the Bear Faith. Factory falls. |
| Tone | Quirky hug-coup. Hearts on the HUD. No cruelty. |
| Construction | `CreateDefaultSubobject` only in constructors. Runtime `NewObject` meshes: `AddInstanceComponent` + `RegisterComponent`. |
| Plugins | Python, Enhanced Input (present but player uses classic `PlayerInput`), ProceduralMesh, optional MCP. |

---

## 2. Why the current game is not AAA

The slice already has: 26-city atlas, 13 missions, 40 villains, UMG battle overlay, three 6-node skill trees, hug conversion juice, cottage → Ribbon opening.

It is missing everything a store-page title needs:

- No save game, no title screen, no settings, no gamepad remap.
- Cities are kits, not interiors + named people + shops + schedules.
- Combat is one enemy, six buttons, no status engine, no boss phases.
- No audio. No animation beyond bounces.
- No inventory, economy, or crafting.
- No dialogue graph (toasts and one-liners).
- `AHolypawCharacter` owns battle, HUD flags, hug, miracle, map, codex — a god class.
- World is one actor (`HolypawWorldBuilder.cpp` ~1,100 lines) generating the globe in one shot.

The 200k-line rewrite **splits that god class**, **fills the globe with play**, and **ships systems**.

---

## 3. Architecture (modules)

Split the single `Holypaw` module so 200k lines do not live in one compile unit.

```
Holypaw.uproject
├── HolypawCore        types, save, settings, time, subsystems
├── HolypawWorld       atlas, streaming, kits, 26 city builders, dungeons
├── HolypawCombat      battle director, abilities, status, villain AI
├── HolypawNarrative   dialogue VM, quests, cinematics, journal
├── HolypawCharacters  teddy, humans, fluffies, believer parade
├── HolypawAI          schedules, crowd, party followers
├── HolypawUI          title → pause → map → shop → battle → photo
├── HolypawAudio       mixer, cue IDs, stingers (WAV later)
├── HolypawData        authored catalogs (items, dialogue, lore) — generated C++ allowed
├── HolypawEditor      editor utilities (optional module)
└── Holypaw            glue GameMode / Character / remaining actors
```

**GameInstance** (`UHolypawGameInstance`) owns save, audio, settings, and subsystem startup. GameMode only spawns the world builder and applies travel options.

**Streaming analog (no World Partition maps):** keep runtime generation, but **chunk** it. Terrain/flora/cities load in a ring around the teddy (`UHolypawWorldStream`). Far cities exist as atlas data + a lod kit; near cities run full NPC sim. This is mandatory or 26 dressed cities will hitch.

**Data rule:** Python under `Tools/gen/` authors CSV/JSON **and emits C++ catalogs**. Generated files are committed. Validators fail the build if IDs drift from enums. That is how dialogue and items hit tens of thousands of lines without hand-typing braces wrong.

---

## 4. Line ledger (floor = 200,000)

Every number is a **minimum unique-line budget**. Implementation may go over. It may not go under by dumping comments.

| # | Pillar | Min lines | What the lines actually are |
| --- | --- | ---: | --- |
| P1 | Core architecture | 18,500 | GameInstance, save slots, settings, input remap, accessibility, time-of-day, weather, pooling, streamer, versioning |
| P2 | Teddy, party, conversion | 16,000 | Procedural teddy motion, hug/miracle/faith contagion, city Hearts, party roles, 48 fluffy species |
| P3 | Combat | 24,000 | Battle director, 80+ abilities, status/resist, 40 villain AI kits, 12 boss phase scripts, juice |
| P4 | 26 living cities | 52,000 | Shared city sim + **26 first-class city modules** (districts, NPCs, shops, interiors, events) |
| P5 | Narrative & dialogue | 48,000 | Dialogue VM, 40 story beats, 220 side quests, 650 speakers, lore/codex |
| P6 | Items / economy / craft | 12,000 | 400 items, per-city shops, sewing/stuffing recipes, inventory |
| P7 | UI suite | 14,000 | Title, load, pause, map, journal, battle, shop, inn, photo, credits |
| P8 | Audio runtime | 8,000 | Buses, cue table, city themes, stingers, mixer (assets later) |
| P9 | Dungeons & wilderness | 14,000 | Poly Mill raid, cottage/Ribbon interiors, shrines, encounter tables |
| P10 | Tools, tests, loc, meta | 14,000 | Generators, validators, automation, EN string tables + loc keys, achievements |
| | **Total floor** | **220,500** | **Buffer over 200,000 so content cuts cannot sneak under** |

Track with `python3 Tools/CountHolypawLoc.py`. The counter **excludes** `legacy/`, blank lines, and `//`-only lines.

### 4.1 Pillar 1 — Core (18,500)

**Files (new):**

- `Source/HolypawCore/HolypawGameInstance.cpp/.h`
- `Save/HolypawSaveGame.cpp/.h` — 3 slots + autosave + ironman optional
- `Save/HolypawSaveCodec.cpp` — versioned blob (position, party, skills, Hearts per city, quest flags, inventory, seen villains, time-of-day)
- `Settings/HolypawSettings.cpp` — video, audio, input, subtitles, colorblind, reduce-flash, hold-to-hug
- `Input/HolypawInputRouter.cpp` — classic mappings stay default; remap table; gamepad icons
- `Time/HolypawClock.cpp` — 24-minute day, festival days, miracle dusk bonus
- `Weather/HolypawWeather.cpp` — clear / gold haze / plush snow / polyester smog near mill
- `World/HolypawWorldStream.cpp` — chunk load/unload, ISM budget, far-city lod
- `World/HolypawPool.cpp` — humans, villains, pickups
- `Version/HolypawBuildInfo.cpp`

**Save contents (minimum):** teddy transform, HP/AP/FP, party of 4, skill unlocks, mission index, `TMap<City, Hearts>`, inventory, quest bitfield, villain seen/defeated, clock, weather, last rest, fast-travel unlocks.

**Do not** serialize UObject pointers. IDs only.

### 4.2 Pillar 2 — Teddy, party, conversion (16,000)

Split `AHolypawCharacter` until it is a pawn + input, not a game.

- `Components/HolypawMotor.cpp` — walk, jump, slope, hug-lock 0.28s, explore camera 540 / battle 300
- `Components/HolypawTeddyAssembly.cpp` — named sockets (`earL`, `snout`, `halo`, `pawL`…) for later FBX
- `Anim/HolypawProcAnim.cpp` — bob, blink, ear flop, hug wrap, kneel, victory spin (procedural until skeletal mesh exists)
- `Faith/HolypawFaithSim.cpp` — per-human convert %, sermon radius, **city Hearts**, believer outfits, one-shot kneel (`bKnelt`)
- `Faith/HolypawParade.cpp` — converted humans follow routes, clap, form ribbon lines at festivals
- `Party/HolypawPartyAI.cpp` — 4 followers, formations, battle roles (tank fluff / hymn / slap)
- `Catalogs/HolypawFluffyCatalog.cpp` — **48 species** (expand from 6), stats, recruit lines, habitat

Conversion fantasy stays the joke: stuffing Bear Faith into people until they clap. Believers do **not** grant more AP. Per-city Hearts gate shops, fast travel, and mill raids.

### 4.3 Pillar 3 — Combat (24,000)

Extract battle out of the character.

- `Combat/HolypawBattleDirector.cpp` — turns, speed, interrupt, flee rules, `bBlocksFlee`
- `Combat/HolypawAbility.cpp` + `HolypawAbilityCatalog.cpp` — **80 abilities** (slap, beam, party, guard, hymn + 75 more: stitch, unstuff, poly-rip, lullaby, button-beam…)
- `Combat/HolypawStatus.cpp` — rip, drain faith, frost, poison thread, rage, armor plates, hymn shield
- `Combat/HolypawCameraRig.cpp` — pull-in, hitstop, land-shake, slow-mo on convert-kill
- `AI/HolypawVillainBrain.cpp` — one brain class, **data-driven per villain**
- `AI/Bosses/` — 12 phase scripts (Silk Magistrate, Brine Warden, Harvest Overseer, Bog King, Aurora Warden, Velvet Tyrant, Unmaker, Miracle Eater, mill directors, world bosses)

Combat stay **turn-based overlay**, GameOnly input, keys 1–6 plus skill overflow on a command list. Instant victory when HP hits 0 (keep that bugfix). Poly Mill faction takes extra **Poly Rip**.

### 4.4 Pillar 4 — 26 living cities (52,000) — the bulk of AAA feel

Shared (`8,000`):

- `City/HolypawCitySim.cpp` — schedules (dawn stall, noon plaza, dusk hymn, night windows)
- `City/HolypawInteriorKit.cpp` — door volumes, furniture kits, rest beds
- `City/HolypawShopKeep.cpp` — stock, prices, Hearts discounts
- `City/HolypawCrowd.cpp` — ISM pedestrians + a budget of possessed NPCs

Then **one file per city, ~1,700 unique lines each** (`26 × 1,700 = 44,200`):

`Source/HolypawWorld/Cities/City_RibbonCity.cpp` … `City_FeltIceCamp.cpp`

Each city module **must** contain:

| Layer | Minimum |
| --- | --- |
| Districts | 5 named (Ribbon already has Plaza, Market, Cloth Quarter, Harbor Steps, Quiet Rows) |
| Named NPCs | 24 unique (name, job, schedule, 1 side-quest hook) |
| Interiors | 8 enterable (inn, stall, chapel, mill office, home, museum, dock, cellar) |
| Shops | 4 (faith stall, thread, snacks, souvenirs) |
| Landmarks | 6 with sign copy |
| Local events | 4 (festival, mill protest, parade, weather gag) |
| Hearts gates | 3 (shop inventory, hymn choir, fast-travel lantern) |
| Ambient lines | 40 bark strings |

No two cities share the same NPC names or landmark copy. That is how 52k stays unique.

**City identity (do not homogenize):**

- Ribbon City — coup capital, mill shadow to the east
- Tidewell — salt, hooks, brine hymns
- Hearthfold / Quiltland — cloth, harvest
- Emberfen — mire, witch-pets
- Snowveil / Aurora / Tundra / Felt Ice — cold rites
- Lantern Angeles — cinema lanterns, billboards of polyester stars
- Dust Mesa / Palma Dusk / Ivory Spire / Sand Hymn / Cape Plush / Savannah Bell — sand belt
- Clockhaven / Velvet Seine / Marble Forum — old world pomp
- Carnival Bahia / Andes Loom — south sewn
- Silk Delta / Spice Harbor / Cherry Loom — east
- Coral Choir — reef choir
- Mossgate — forest gate between park and capital

### 4.5 Pillar 5 — Narrative (48,000)

- `Narrative/HolypawDialogueVM.cpp` (`5,000`) — nodes: line, choice, skill-check (hug/miracle/party), grant item, set flag, start battle, convert
- `Narrative/HolypawStoryBeats.cpp` (`8,000`) — expand 13 missions into **40 beats** with in-engine camera rails (cottage wake, first hug, Ribbon gates, mill courtyard, globe trek arrivals, four rites, peak miracle)
- `Quests/HolypawSideQuestCatalog.cpp` (`17,600`) — **220 side quests**, ~80 lines each (id, giver, steps, rewards, fail-cute)
- `Dialogue/Banks/` (`16,250`) — **650 speakers**, generated from `Tools/gen/dialogue/` 
- `Lore/HolypawLoreBooks.cpp` (`1,150`) — readable plush-history, mill memos, Bear Creed pamphlets

Main path stays 13 journal missions. Side quests never require Pokémon-style catching. Recruiting fluffies is friendship + hug, not a ball.

### 4.6 Pillar 6 — Items / economy (12,000)

400 items across: thread, stuffing, ribbons, snacks, souvenirs, mill scrap, hymn sheets, furniture, fluffy hats.

Sewing bench: combine scrap + stuffing → gear that changes teddy parts (visual sockets).

Currency: **AP** (adventure points) spendable; **FP** (faith); Hearts are reputation, not cash.

### 4.7 Pillar 7 — UI (14,000)

Every overlay is C++ `NativePaint` / UMG hybrid like `UHolypawBattleWidget` (this VM has no Widget Blueprints to author).

Screens: splash, title, slot select, pause, settings, world map (Hearts heat), journal, dialogue, shop, inn, party, skills, codex, inventory, photo mode, credits, game-over wake-at-cottage.

HUD: Hearts, AP/FP, zone name, toast that does **not** overwrite mission-complete (keep that fix).

### 4.8 Pillar 8 — Audio (8,000)

Code-only here: `UHolypawAudioSubsystem` + a **cue table** with IDs for every city theme, hug, convert chime, battle stinger, mill drone, miracle choir.

On the user’s PC: drop WAVs on those IDs. No silent ship. Until assets exist, procedural sine/noise stingers are allowed as placeholders **tagged `TEMP`**.

### 4.9 Pillar 9 — Dungeons & wilderness (14,000)

- Cottage interior pass (bed, table, door, fallen tree — already started; expand loft, cellar, porch night)
- Ribbon interiors + sewers
- **Poly Mill raid:** 6 floors (gate, spinning frames, dye vats, executive felt, unstuffing bay, roof hymn)
- Wilderness encounter tables per biome
- Shrines for the Four Rites
- Sky rift + volumetric clouds stay; add mill smog volume — **started:** clock-driven sun/fog plus polyester haze near the mill

### 4.10 Pillar 10 — Tools / tests / loc (14,000)

- `Tools/CountHolypawLoc.py` — running total vs 200,000
- `Tools/ValidateHolypaw.py` — expand: save roundtrip, city NPC uniqueness, quest ID uniqueness, no Sanrio/Pokémon strings
- `Tools/gen/` — dialogue, items, quests → C++
- Automation tests under `Source/HolypawTests/`
- `Loc/HolypawStrings_EN.cpp` plus key tables for ES/FR/JA (translations can be incomplete; **keys must exist**)
- Achievements: first hug, 26 cities, mill roof, 100 Hearts, party of 4 legend fluffies

---

## 5. Content bible (art / audio / animation — user PC)

Code will be AAA-scoped without these, but the **title will not look AAA** until:

1. Skeletal teddy + human + 6 hero fluffies (or keep kitbash and paint unique materials).
2. City trim sheets in the plush palette (no Megascans).
3. 26 city themes + battle + mill + cottage stems.
4. UI font and icon set (stitched).
5. Packaged `Development` then `Shipping` from UE 5.8 on Windows.

Named sockets and cue IDs are the contract. Artists never hunt through `HolypawCharacter.cpp`.

---

## 6. Implementation waves (order is mandatory)

Do not dress Felt Ice Camp before save exists. Do not write 220 quests before the dialogue VM runs.

| Wave | Name | Min lines this wave | Playable result |
| --- | ---: | ---: | --- |
| W0 | Plan + loc counter | — | this document, `CountHolypawLoc.py` |
| W1 | Persistence & meta | 8,000 | title, 3 save slots, settings, wake-at-cottage restore |
| W2 | City Hearts & travel | 6,000 | per-city Hearts, lantern fast travel, believer parade stub |
| W3 | Combat director | 10,000 | abilities extracted, status, 4 boss phases — **started:** `HolypawBattleMath`, slap combo / crit / frost / poison / hymn shield / stagger, hit flash |
| W4 | Dialogue VM | 8,000 | talk to NPCs, choices, journal updates |
| W5 | Ribbon City full | 8,000 | 24 NPCs, 8 interiors, 4 shops — the AAA vertical slice |
| W6 | Audio + proc anim | 6,000 | cue table, teddy bounce/hug/kneel |
| W7 | Items + shops | 8,000 | inventory, 120 items, Ribbon shops live |
| W8 | Remaining 25 cities | 44,000 | each city module lands playable, streamed |
| W9 | 220 side quests + banks | 34,000 | globe has errands, not empty roads |
| W10 | Poly Mill raid + remaining bosses | 12,000 | endgame dungeon
| W11 | UI suite + photo + loc keys | 14,000 | store-page pause menu
| W12 | Tests, polish, packaging hooks | 12,000 | validators green, Shipping notes |

Waves W8–W9 are where the ledger crosses 200k. Skipping them **fails the floor**.

---

## 7. Performance budget (or the globe dies)

| Budget | Cap |
| --- | --- |
| Possessed NPCs | 48 near, ISM crowd beyond |
| Villain AI | 12 active |
| Draw | ISM kits; no per-window StaticMesh actors |
| Stream radius | 2 cities + wilderness ring |
| Target | 60 fps on a mid PC in Ribbon; 30 fps acceptable in mill raid |

`GridN = 168`, `Cell = 1620` stays unless streaming proves a smaller tile is needed. Do not explode component count.

---

## 8. QA and “done”

**Code floor:** `CountHolypawLoc.py` prints `TOTAL >= 200000` and `PILLARS_OK`.

**Playable AAA-scope checklist:**

- [ ] New game → cottage porch → lanterns → Ribbon without falling through world
- [ ] Save in Ribbon, load, still have Hearts and party
- [ ] Hug conversion is still cute; believers clap; no extra AP
- [ ] Battle 1–6 never stolen by mouse UI
- [ ] 26 cities visitable; each has unique NPCs and at least one interior
- [ ] Fast travel between lanterns after Hearts gate
- [ ] Poly Mill six floors; Unmaker; peak miracle; mill falls
- [ ] 13 journal missions + 220 side quests completable
- [ ] Settings: mute, subtitles, remap, colorblind
- [ ] Validator forbids Sanrio, Pokémon, Megascans, Hello Kitty strings
- [ ] User-PC Shipping package documented in README

**Not done:** a cloud folder with 200k lines of `int Dummy26 = 26;`. The counter’s uniqueness heuristic flags repeated 40-line clones.

---

## 9. Honesty (read this before cheering)

Writing 200,000 lines in one agent session is not possible if those lines are systems that compile and play. This plan is the **contract**. Implementation proceeds wave by wave on `cursor/unreal-holypaw-phase1-14aa`, each wave committed, pushed, and PR-updated.

A AAA *store page* still needs the user’s Unreal 5.8, audio, and art. A AAA *gameplay scope* is this ledger. Both are required. Neither is optional.

---

## 10. First concrete files after this plan (Wave 1)

1. `UHolypawGameInstance` + `UHolypawSaveGame`
2. Title / slot UI (C++ widget)
3. Serialize: transform, stats, party, skills, mission, converts, seen villains
4. Load applies before `GenerateWorld` finishes spawning the teddy at saved location (cottage if new)
5. Expand validator + loc counter

Then Wave 2 (Hearts, lanterns, parade) — already identified as the highest-fun next slice — sits on top of save so progress is not a ghost.
