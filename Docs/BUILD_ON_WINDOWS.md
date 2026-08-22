# Build Holypaw on Windows (no .sln required)

## You might not get `Holypaw.sln`

That is normal on recent setups:

| What you see | What it means |
| --- | --- |
| Visual Studio opened a **workspace** (folder view) | VS 2022 **17.12+** with **Visual Studio Tools for Unreal Engine** — you do **not** need a `.sln`. |
| `Holypaw.code-workspace` | **VS Code / Cursor**, not Visual Studio. Use `BuildHolypaw.bat` instead (below). |
| `Holypaw.sln` in the project folder | Classic Unreal — open that file in Visual Studio. |
| Files under `Intermediate\ProjectFiles\` | Unreal’s generated project files. Some installs put the `.sln` here instead of the root. |

Git never contains `Holypaw.sln`. Right-click **Generate Visual Studio project files** creates local files on your PC only.

## Easiest: build without opening any solution

1. Install **Unreal Engine 5.8** (Epic Launcher).
2. Install **Visual Studio 2022** workload **Game development with C++** + a **Windows 10/11 SDK**.
3. Pull branch `cursor/fix-unreal-compile-f843` (PR #8).
4. Double-click **`BuildHolypaw.bat`** in the same folder as `Holypaw.uproject`.

   Or in Command Prompt:

   ```
   BuildHolypaw.bat "C:\Program Files\Epic Games\UE_5.8"
   ```

5. Wait for **BUILD OK**.
6. Open **`Holypaw.uproject`**.

This uses Unreal Build Tool directly. No `.sln` required.

## If Visual Studio already opened a workspace

You are probably fine.

1. In Visual Studio: **File → Open → Folder** (or **Open → Unreal Engine Project**).
2. Pick the folder that contains **`Holypaw.uproject`**.
3. Top toolbar: **Development Editor** and **Win64**.
4. **Build → Build All** (or find **HolypawEditor** in Solution Explorer and **Build**).
5. When that succeeds, open **`Holypaw.uproject`**.

Install **Visual Studio Tools for Unreal Engine** if the Unreal targets do not appear: Visual Studio Installer → Modify → Individual components → search **Unreal**.

## If you still want `Holypaw.sln`

1. Double-click **`GenerateHolypawSln.bat`** in the project root.
2. Look in:
   - same folder as `Holypaw.uproject`
   - `Intermediate\ProjectFiles\Holypaw.sln`
3. In File Explorer search the project folder for `*.sln`.

## Still stuck?

Send a screenshot or list of **new files** that appeared after Generate (names only), plus whether you use **Visual Studio** or **VS Code/Cursor**.
