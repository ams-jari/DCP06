# DCP06 — Linux Captivate Simulator (WSL)

**Date:** 2026-06-14  
**Environment:** WSL2 + Ubuntu 22.04, Captivate v10.0.0-rc.309 Linux packages  
**Status:** Simulator GUI OK; DCP06 **in Apps menu** via dev install; **launch needs signed `.lxx`** (see §7.1)

**Related:** [DCP06_POSIX_WSL_Setup.md](DCP06_POSIX_WSL_Setup.md), [DCP06_POSIX_Linux_Build_Steps.md](DCP06_POSIX_Linux_Build_Steps.md) (Step 7)

---

## 1. Packages (from Leica Captivate v10 distribution)

| Package | Path in zip | Purpose |
|---------|-------------|---------|
| `leicacaptivate_10.0.0-rc.309_amd64.deb` | `Simulator/Simulator/` | Linux Captivate GUI (`captivate` command) |
| `ts-grpc-client_2.1.1_amd64.deb` | `Simulator/Simulator/` | TS20 instrument sim + gRPC client (Docker) |
| `*.lxx` (samples) | `Simulator/Simulator/LoadableApplications/` | Partner plugin packages (e.g. HelloWorldGSV) |

Install inside WSL:

```bash
sudo dpkg -i leicacaptivate_10.0.0-rc.309_amd64.deb
sudo dpkg -i ts-grpc-client_2.1.1_amd64.deb
sudo apt -f install   # if dependencies missing
```

---

## 2. Required configuration

### TS sensor type (fixes “No response from total station” popup)

Add to `/etc/captivate/captivate.env` (re-add after each `leicacaptivate` deb reinstall):

```bash
echo 'DEFAULT_SENSOR_TYPE=2300' | sudo tee -a /etc/captivate/captivate.env
grep DEFAULT_SENSOR_TYPE /etc/captivate/captivate.env
```

### Docker (TS simulator)

User must be in the `docker` group, or use `sudo` for Docker commands:

```bash
sudo usermod -aG docker $USER   # once; then restart WSL or: newgrp docker
sudo service docker start
```

---

## 3. Daily startup (correct commands)

**Use the installed launcher — not the binary from the Windows-mounted zip.**

```bash
cd ~    # any directory is fine; do NOT cd into Simulator/Simulator on /mnt/c

# Terminal 1 — TS simulator + gRPC (sudo if docker permission denied)
sudo start_ts_simulator_grpc_client -d
sudo docker ps    # expect container: ts20-simulator

# Terminal 2 — Captivate GUI (no sudo)
captivate
```

| Command | sudo? | Notes |
|---------|-------|-------|
| `start_ts_simulator_grpc_client -d` | Often yes (Docker socket) | “permission denied” without sudo/`newgrp docker` |
| `captivate` | **No** | Runs as your user via `gui-app` group; sudo breaks WSLg display |

Stop Captivate: **Ctrl+C**, then run `captivate` again to reload.

Verify launcher:

```bash
which captivate   # /usr/bin/captivate
```

**Do not run:**

```bash
/mnt/c/.../Captivate_v10.0.0-rc.309/Simulator/Simulator/LeicaCaptivate
```

That path flashes and dies with `Killed` even when the deb install is healthy.

---

## 4. GStreamer / QML console noise (normal)

On startup you may see many `GStreamer-WARNING` lines and:

```
module "org.freedesktop.gstreamer.Qt6GLVideoItem" is not installed
```

These appear on WSL even when Captivate **works**. They are not the crash cause if the GUI stays open.

---

## 5. POSIX build reconnaissance completed (2026-06-13 — 2026-06-14)

| Step | Result | Notes |
|------|--------|-------|
| 1–4 | Header/link/min plugin/logger smoke | See [Build Steps](DCP06_POSIX_Linux_Build_Steps.md) |
| 5 | `~/build/dcp06-step05/DCP06.so` (x86-64) | `Start15751` exported |
| 6 | `~/build/dcp06-step06/DCP06.so` (ARM) | Yocto cross-SDK |
| 7 | MkEdit dev install + Apps menu | **Partial** — tile visible; launch needs signed `.lxx` |

---

## 6. Step 7 — MkEdit / `.lxx` / dongle

| Action | Dongle? | Result (2026-06-15) |
|--------|---------|------------------------|
| Compile `DCP06.so` (Step 5) | No | `libDCP06.so` — OK |
| MkEdit PObs + dev install `-I:` | No | Plugin registers in Apps menu; **no** `libDCP06.so.sig` |
| MkEdit LOB package `-C` (`DCP06_Release.xml`) | **Yes** | `DCP06.lxx` + signature — **needed to run app** |

Linux dev install (`-I:`) is **not** equivalent to Windows plain-`DCP06.dll` copy: v10 Linux sim **enforces binary signature** on partner plugins. Use signed `.lxx` for a runnable app (coordinate with Pasi).

Script:

```bash
bash "$DCP06_ROOT/Project/Linux/step07_lxx_package/build_lxx.sh" package   # dongle
bash "$DCP06_ROOT/Project/Linux/step07_lxx_package/build_lxx.sh" install   # experimental
```

MkEdit runs on **Windows** via `cmd.exe` from WSL; config is `Project/x86_64-ubuntu_22.04-gcc11/Config/DCP06_RelWithDebInfo.xml`.

---

## 7. DCP06 plugin crash — IMPORTANT

**Observed (2026-06-14):** Running Step 7 `install` left a broken plugin under:

```
/home/gui-app/captivate/internal-storage/System/Plugin/DCP06/
```

Captivate scans **every subfolder** of `Plugin/` at startup. The malformed `.app` files caused:

```
PANIC: Unexpected line format. - type 0x0  (LOD_APP_Properties.cpp)
```

Symptom: GUI flashes, terminal shows `Killed .../LeicaCaptivate`.

**Renaming the folder is not enough** (e.g. `DCP06.disabled.*` is still loaded).

### Recovery

Move the folder **completely out** of `Plugin/`:

```bash
mv /home/gui-app/captivate/internal-storage/System/Plugin/DCP06 \
   ~/DCP06.disabled.backup
# or whatever name exists under Plugin/
ls /home/gui-app/captivate/internal-storage/System/Plugin/   # should be empty or only valid plugins
captivate   # should stay open
```

Confirm in log (no new PANIC lines):

```bash
tail -5 /home/gui-app/captivate/internal-storage/Log/ErrorLog_01.txt
```

### Root cause (2026-06-15)

Removing only `DCP06.sys` from the plugin folder lets Captivate start; the crash is in **`.sys` metadata**, not `.app` / `.so`.

Compared to working built-in `Disto.sys` and SDK HelloWorld samples, MkEdit-installed `DCP06.sys` was missing:

| Issue | DCP06 (broken) | Working plugins |
|-------|----------------|-----------------|
| `App.Type` | missing | `mixed` |
| EntryPoint comment line | missing | `;App.EntryPoint=<...>` before real line |
| Line endings in body | LF only | CRLF (`\r\n`) |

Fix: update `Project/x86_64-ubuntu_22.04-gcc11/DCP06.sys` (CRLF + `App.Type=mixed` + comment), then re-run `build_lxx.sh install`.

### 7.1 Launch — “This app may be damaged” (2026-06-15)

After the `.sys` fix, Captivate **starts**, DCP06 **tile #25 appears** in Apps (icon may be blank). Tapping it shows *“This app may be damaged.”*

Error log at click time:

```
Unable to open file .../Plugin/DCP06/libDCP06.so.sig
Signature verification failed: public key hash mismatch.
AppLoader ApplicationC::checkValidity: application is not signed
```

**Cause:** Linux Captivate v10 verifies partner plugin binaries. MkEdit dev install (`-I:`) without a dongle does **not** produce `libDCP06.so.sig`. Windows simulator dev install (plain `DCP06.dll` copy) does not enforce this the same way.

**Next step for a runnable app:** signed **`.lxx`** package (dongle / Pasi) — see HelloWorld `Release.xml` POB type `APPL_DLL_LINUX_SIGNATURE` in the SDK. Optional interim test: load `HelloWorldGSV.lxx` from the Captivate v10 distribution to confirm the official partner install path.

**Blank menu icon:** PNGs are under `SWXRes/` (manual post-install copy). May improve after a full `.lxx` install with `APPRESOURCES` POB; Disto also ships separate `*Focus_1X.png` icons.

---

## 8. Handoff to Pasi — signed `.lxx` required

**Verified without dongle (2026-06-15):**

1. `build_lxx.sh install` → DCP06 tile in Apps menu (App.Id 15751)
2. Captivate starts with plugin installed (after `.sys` fix)
3. Clicking DCP06 → *“This app may be damaged”* — missing `libDCP06.so.sig`

**Request (dongled Windows PC):**

```bash
# WSL — stage assets
bash "$DCP06_ROOT/Project/Linux/step07_lxx_package/build_lxx.sh" package
```

Then on Windows with dongle, from `Project/`:

```
MkEdit.exe -M:mk -F:x86_64-ubuntu_22.04-gcc11\Config\DCP06_Release.xml -C
```

Output: `Project/Linux/step07_lxx_package/out/RelWithDebInfo/DCP06.lxx` (includes `APPL_DLL_LINUX_SIGNATURE` POB).

Install via Captivate v10 Linux sim UI (same path as `HelloWorldGSV.lxx` in the Captivate distribution). Confirm DCP06 launches and menu icon loads.

**Open questions for Leica / Pasi:**

1. Official partner `.lxx` install workflow in Linux Captivate v10 UI
2. ARM `.yxx` packaging for TS20 device (Step 6 `DCP06.so` output)
3. Whether unsigned dev install is supported on Linux long-term (MkEdit deprecated)

## 9. Revision history

| Date | Change |
|------|--------|
| 2026-06-14 | Initial doc: sim install, startup, Step 7, dongle, DCP06 crash recovery |
| 2026-06-15 | `.sys` crash root cause + fix; Apps menu verified; launch blocked on missing `.sig`; Pasi handoff §8 |
