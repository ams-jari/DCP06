# DCP06 — Linux Captivate Simulator (WSL)

**Date:** 2026-06-14  
**Environment:** WSL2 + Ubuntu 22.04, Captivate v10.0.0-rc.309 Linux packages  
**Status:** Simulator GUI verified working; DCP06 plugin dev-install **not yet safe** (see §7)

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
| 7 | MkEdit staging + PObs | `.lxx` needs dongle; dev install experimental |

---

## 6. Step 7 — MkEdit / `.lxx` / dongle (same rule as Windows)

| Action | Dongle? | Artifact |
|--------|---------|----------|
| Compile `DCP06.so` (Step 5) | No | `libDCP06.so` |
| MkEdit PObs + dev install `-I:` | No | Files under `internal-storage/System/Plugin/DCP06/` |
| MkEdit LOB package `-C` only | **Yes** | `DCP06.lxx` |

Linux is **not** stricter than Windows — both gate **packaged** partner files (`.lxx` / `.dxx`). Day-to-day sim work on Windows uses MkEdit **install** (`-I:`), not `.dxx`. Linux dev install is the same idea but our first attempt produced an incomplete plugin tree.

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

**Do not re-run `build_lxx.sh install`** until install produces a single valid `DCP06.app` (coordinate with Pasi / dongled MkEdit workflow).

---

## 8. Open items for Pasi / Leica

1. Signed `.lxx` install workflow in Linux Captivate v10 UI  
2. Whether dev install without dongle is supported long-term (MkEdit deprecated)  
3. Correct Linux `.app` format when `APPL_DLL_LINUX` + incomplete resources POB  
4. ARM `.yxx` packaging for TS20 device builds (Step 6 output)

---

## 9. Revision history

| Date | Change |
|------|--------|
| 2026-06-14 | Initial doc: sim install, startup, Step 7, dongle, DCP06 crash recovery |
