# Connect Cursor to Unreal (Holypaw)

Unreal **5.8 only**. The MCP server runs **inside the editor** on your PC — not in Cloud Agent, not in Cursor.

## If the plugin checkbox will not stay on

On older `main` commits, `Holypaw.uproject` had **Model Context Protocol disabled on purpose** so a missing MCP install would not block the editor. Enabling it in **Edit → Plugins** could look like it worked, then **git pull reset the file** and the plugin was off again after restart.

Current `main` enables MCP + All Toolsets as **optional** plugins. After you pull, restart Unreal once.

If the checkbox is **gray** or clicking does nothing:

1. Confirm **Unreal Engine 5.8** (Epic Launcher → Library). MCP does not exist in 5.7 or earlier.
2. In **Edit → Plugins**, enable **Show Engine Content** (bottom-left gear) so engine plugins appear.
3. Search **Model Context Protocol** (not just “MCP”).
4. Enable **All Toolsets** as well, then restart when prompted.
5. Read **Window → Developer Tools → Output Log** on startup. Search for `ModelContextProtocol` or `failed to load`.

If Holypaw itself will not compile, fix the C++ build first (`BuildHolypaw.bat` / `Docs/BUILD_ON_WINDOWS.md`). A broken module load can block other plugins.

## Start the server (Unreal console, not Cursor)

1. Open `Holypaw.uproject` in Unreal 5.8.
2. Press **`** (backtick) to open the **editor console**.
3. Run:

   ```
   ModelContextProtocol.StartServer
   ```

4. Output Log should show `LogModelContextProtocol` and port **8000**.

Optional: **Edit → Editor Preferences → General → Model Context Protocol → Auto Start Server**.

PowerShell test (server up = **405** response, not “connection refused”):

```powershell
curl.exe -v http://127.0.0.1:8000/mcp
```

## Point Cursor at Unreal

Create **`.cursor/mcp.json`** next to `Holypaw.uproject` (or copy from `.cursor/mcp.json.example`):

```json
{
  "mcpServers": {
    "unreal-local": {
      "type": "http",
      "url": "http://127.0.0.1:8000/mcp"
    }
  }
}
```

1. **File → Open Folder** → Holypaw root (folder with `Holypaw.uproject`).
2. **Settings → Tools & MCP** → enable **unreal-local**.
3. Unreal must already be running with the server started (steps above).

## Order that works

```
Unreal open → plugins enabled → ModelContextProtocol.StartServer → curl 405 → Cursor open folder → MCP green
```

Cursor cannot start Unreal’s server. Cloud Agent cannot reach `127.0.0.1` on your machine.

## Still stuck?

Note exactly what happens when you click Enable:

| Behavior | Likely cause |
| --- | --- |
| Grayed out | Wrong UE version, or plugin not in your engine install |
| Checks, then off after restart | Old uproject had `Enabled: false`; pull latest `main` |
| Checks, server command does nothing | Plugin not loaded — check Output Log for load errors |
| Cursor toggle red | Unreal not running or server not started |

Paste the **Output Log** lines containing `ModelContextProtocol` or `Plugin` if you need help.
