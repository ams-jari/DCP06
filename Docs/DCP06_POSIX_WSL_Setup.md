# DCP06 — WSL2 + Ubuntu 22.04 Setup (POSIX / TS20)

**Date:** 2026-06-13  
**Purpose:** Set up a **Windows-integrated Linux dev environment** for the Captivate POSIX Plugin SDK and future DCP06 TS20 builds.  
**Scope:** **WSL2 + Ubuntu 22.04 only** — not dual-boot, not bare-metal Linux.

**Related:** [DCP06_POSIX_SDK_Analysis.md](DCP06_POSIX_SDK_Analysis.md)

---

## 1. What this gives you

| You keep on Windows | You add via WSL2 |
|---------------------|----------------|
| DCP06-VS2008 / VS2022 builds | GCC toolchain matching SDK (`ubuntu_22.04-gcc11`) |
| TS Captivate 10 simulator | Run Leica POSIX SDK tools (`lm`, Yocto installer) |
| MkEdit, TextTool, Cursor/VS | Cross-compile exploration for TS20 (`armv8-leicageo_linux_5.0-gcc13`) |
| Edit repo in `C:\Users\dell\Desktop\AMS\Development\DCP06` | Same files visible under `/mnt/c/...` from Ubuntu |

WSL2 is the recommended path from Leica’s POSIX SDK layout (Linux x86_64 host, Ubuntu 22.04 libs).

---

## 2. Prerequisites

- **Windows 10** (build 19041+) or **Windows 11**
- **Administrator** access (one-time WSL install)
- **Disk space:** plan **~20–40 GB** free  
  - POSIX SDK ~3 GB  
  - Yocto cross-SDK (optional, Stage 2) ~5–15 GB  
  - Build artifacts and apt packages
- POSIX SDK already extracted on Windows, e.g.:  
  `C:\Users\dell\Desktop\AMS\Development\Captivate_PluginSDK_POSIX_v10.0.0-rc.309`

---

## 3. Install WSL2 and Ubuntu 22.04

Run in **PowerShell as Administrator**:

```powershell
# Enable WSL and Virtual Machine Platform (may prompt reboot)
wsl --install

# If Ubuntu 22.04 is not the default, install it explicitly:
wsl --install -d Ubuntu-22.04
```

After reboot, launch **Ubuntu 22.04** from the Start menu and create your Linux username/password.

Verify versions:

```powershell
wsl --list --verbose
```

Expected: `Ubuntu-22.04` with **VERSION 2**. If VERSION is 1:

```powershell
wsl --set-version Ubuntu-22.04 2
```

Inside Ubuntu (first login):

```bash
sudo apt update && sudo apt upgrade -y
```

---

## 4. Paths — Windows vs WSL

Your usual Windows paths map under `/mnt/c/`:

| Windows | WSL (Ubuntu) |
|---------|----------------|
| `C:\Users\dell\Desktop\AMS\Development\DCP06` | `/mnt/c/Users/dell/Desktop/AMS/Development/DCP06` |
| `C:\Users\dell\Desktop\AMS\Development\Captivate_PluginSDK_POSIX_v10.0.0-rc.309` | `/mnt/c/Users/dell/Desktop/AMS/Development/Captivate_PluginSDK_POSIX_v10.0.0-rc.309` |

SDK root (inner folder):

```bash
export CAPTIVATE_POSIX_SDK="/mnt/c/Users/dell/Desktop/AMS/Development/Captivate_PluginSDK_POSIX_v10.0.0-rc.309/Captivate_PluginSDK_POSIX_v10.0.0-rc.309"
export DCP06_ROOT="/mnt/c/Users/dell/Desktop/AMS/Development/DCP06"
```

Add to `~/.bashrc` so they persist:

```bash
cat >> ~/.bashrc << 'EOF'

# DCP06 / Captivate POSIX
export CAPTIVATE_POSIX_SDK="/mnt/c/Users/dell/Desktop/AMS/Development/Captivate_PluginSDK_POSIX_v10.0.0-rc.309/Captivate_PluginSDK_POSIX_v10.0.0-rc.309"
export DCP06_ROOT="/mnt/c/Users/dell/Desktop/AMS/Development/DCP06"
export CAPTIVATE_POSIX_LIBS="$CAPTIVATE_POSIX_SDK/Binary/x86_64-ubuntu_22.04-gcc11/libs"
export CAPTIVATE_POSIX_ARM_LIBS="$CAPTIVATE_POSIX_SDK/Binary/armv8-leicageo_linux_5.0-gcc13/libs"
EOF

source ~/.bashrc
```

### Performance note

- **Editing** in Cursor on Windows + building from WSL against `/mnt/c/...` is fine to start.
- **Heavy compiles** are faster if the SDK and build tree live on the **Linux filesystem** (e.g. `~/leica/sdk`, `~/projects/DCP06`). Copy or rsync when you begin serious builds (Stage 3 below).

---

## 5. Stage 1 — Base dev packages (do this first)

Inside **Ubuntu 22.04**:

```bash
sudo apt install -y \
  build-essential \
  g++ \
  gcc \
  cmake \
  ninja-build \
  git \
  python3 \
  python3-pip \
  xz-utils \
  file \
  pkg-config \
  rsync
```

Check GCC (Ubuntu 22.04 ships GCC 11 — matches SDK binary folder name):

```bash
gcc --version
# Expected: gcc (Ubuntu 11.x.x) ...
```

Verify SDK libs are visible:

```bash
ls "$CAPTIVATE_POSIX_LIBS" | head
# Should list libABL_Tps.so.10.00.0, libCommon.so.10.00.0, ...
```

Verify LibManager runs:

```bash
"$CAPTIVATE_POSIX_SDK/Tools/LibManager/lm" --help
```

---

## 6. Stage 2 — Yocto cross-SDK (TS20 device builds)

**Status (2026-06-13):** Installed on AMS WSL (`/usr/local/leicasdk-x86_64`). ARM `DCP06.so` cross-build verified — see [DCP06_POSIX_Linux_Build_Steps.md Step 6](DCP06_POSIX_Linux_Build_Steps.md#step-6--arm-cross-compile-dcp06so-for-ts20).

The installer must run **inside WSL** (not PowerShell):

```bash
cd "$CAPTIVATE_POSIX_SDK/Platform-SDK"
chmod +x cortexa53-crypto-sdk_v5.0.23.sh
sudo ./cortexa53-crypto-sdk_v5.0.23.sh
```

- Installer: **GeoSurv SDK 5.0.23** (`cortexa53-crypto-sdk_v5.0.23.sh`, ~1.6 GB self-extracting script)
- Default install: `/usr/local/leicasdk-x86_64` (requires `sudo`)
- Needs: `python3`, `xz`, host `gcc` (installed in Stage 1)
- Harmless warning during install: `setlocale: LC_ALL: cannot change locale (en_US.UTF-8)`

After install, source the environment script **before every ARM cross-build session**:

```bash
source /usr/local/leicasdk-x86_64/environment-setup-cortexa53-crypto-leicageo-linux
export CAPTIVATE_POSIX_ARM_LIBS="$CAPTIVATE_POSIX_SDK/Binary/armv8-leicageo_linux_5.0-gcc13/libs"
```

Verify the cross-toolchain:

```bash
source /usr/local/leicasdk-x86_64/environment-setup-cortexa53-crypto-leicageo-linux
aarch64-leicageo-linux-gcc --version
# Expected: gcc (GCC) 13.4.0
```

Build ARM `DCP06.so` (full plugin, 110 sources):

```bash
source ~/.bashrc
bash "$DCP06_ROOT/Project/Linux/step06_arm_cross/build_wsl.sh"
```

Expected: `file DCP06.so` → `ELF 64-bit LSB shared object, ARM aarch64`; `Start15751` exported.

**Note:** Some Yocto SDK setups are sensitive to WSL. If the installer fails, note the error and ask Leica support — a native Linux VM is the fallback, not dual-boot.

---

## 7. Stage 3 — Copy SDK to Linux disk (when builds get slow)

```bash
mkdir -p ~/leica
rsync -a --info=progress2 \
  "/mnt/c/Users/dell/Desktop/AMS/Development/Captivate_PluginSDK_POSIX_v10.0.0-rc.309/" \
  ~/leica/Captivate_PluginSDK_POSIX_v10.0.0-rc.309/

# Update env vars in ~/.bashrc:
# CAPTIVATE_POSIX_SDK="$HOME/leica/Captivate_PluginSDK_POSIX_v10.0.0-rc.309/Captivate_PluginSDK_POSIX_v10.0.0-rc.309"
```

Keep **DCP06 source** on `/mnt/c/...` for Cursor on Windows, or clone/copy to `~/projects/DCP06` if you prefer building entirely inside WSL.

---

## 8. LibManager (`lm`) — symlink helper

Leica ships `lm` for versioned `.so` symlinks (common in cross/WSL setups):

```bash
LM="$CAPTIVATE_POSIX_SDK/Tools/LibManager/lm"

# Lock a library (replace symlink with real file) — example:
# $LM --lock "$CAPTIVATE_POSIX_LIBS/libCommon.so.10.00.0"

# Restore symlink:
# $LM --release libCommon.so.10.00.0

$LM --help
```

Use when the linker cannot follow `.so` → `.so.10.00.0` symlinks.

---

## 9. Open DCP06 / WSL from Windows tools

> **Note:** `wsl` is a **Windows** command. It does **not** work inside the Ubuntu terminal.

**Cursor / VS Code:** Install the **WSL** extension, then:

- Open folder in WSL: `\\wsl$\Ubuntu-22.04\home\<user>\...`
- Or keep editing `C:\Users\dell\Desktop\AMS\Development\DCP06` on Windows and run builds in a WSL terminal.

**Terminal into Ubuntu from PowerShell:**

```powershell
wsl -d Ubuntu-22.04
```

**Confirm WSL version (PowerShell only):**

```powershell
wsl --list --verbose
```

Expected: `Ubuntu-22.04` with **VERSION 2**.

**Run a single command:**

```powershell
wsl -d Ubuntu-22.04 -- bash -lc 'ls "$CAPTIVATE_POSIX_LIBS" | wc -l'
```

(Ensure env vars are in `~/.bashrc` for login shells.)

---

## 10. Quick sanity checklist

| Step | Command / check | Done |
|------|-----------------|------|
| WSL2 enabled | `wsl --list --verbose` → VERSION 2 | [x] |
| Ubuntu 22.04 running | `lsb_release -a` → 22.04 | [x] |
| GCC 11 | `gcc --version` | [x] |
| SDK libs visible | `ls $CAPTIVATE_POSIX_LIBS \| head` | [x] |
| `lm --help` works | `Tools/LibManager/lm --help` | [x] |
| DCP06 path OK | `ls $DCP06_ROOT/README.md` | [x] |
| Env in `.bashrc` | `echo $CAPTIVATE_POSIX_SDK` after new shell | [x] |
| Yocto SDK installed | `ls /usr/local/leicasdk-x86_64/environment-setup-*` | [x] |
| ARM cross-compiler | `aarch64-leicageo-linux-gcc --version` (after sourcing env) | [x] |
| x86 `DCP06.so` builds | `bash Project/Linux/step05_full_plugin/build_wsl.sh` | [x] |
| ARM `DCP06.so` builds | `bash Project/Linux/step06_arm_cross/build_wsl.sh` | [x] |
| Linux Captivate sim runs | `captivate` from `/usr/bin` (see [Captivate Sim](DCP06_POSIX_Linux_Captivate_Sim.md)) | [x] |
| Step 7 MkEdit staging | `bash Project/Linux/step07_lxx_package/build_lxx.sh package` | [x] |

---

## 11. Linux Captivate simulator (optional — UI testing)

Install from Leica Captivate v10 zip (`Simulator/Simulator/*.deb`). Full guide: **[DCP06_POSIX_Linux_Captivate_Sim.md](DCP06_POSIX_Linux_Captivate_Sim.md)**.

Quick start:

```bash
echo 'DEFAULT_SENSOR_TYPE=2300' | sudo tee -a /etc/captivate/captivate.env
sudo start_ts_simulator_grpc_client -d
captivate    # no sudo; not the LeicaCaptivate binary under /mnt/c/.../Simulator/
```

---

## 12. What stays on Windows (do not move yet)

- **Visual Studio 2008 / 2022** — TS/CS20 and CS35 builds  
- **Captivate TS simulator** — Win32 plugin testing  
- **TextTool, MkEdit, SWXResBuilder** — Windows localization/packaging  
- **`SYSTEM1500_ROOT`** Windows SDK tree  

WSL is for **POSIX SDK exploration and future `DCP06.so` builds**, not a replacement for the current Windows workflow.

---

## 13. Troubleshooting

| Issue | What to try |
|-------|-------------|
| `wsl --install` fails | Enable virtualization in BIOS; run PowerShell as Admin |
| Ubuntu stuck on VERSION 1 | `wsl --set-version Ubuntu-22.04 2` |
| `/mnt/c/...` very slow to compile | Copy SDK/build dir to `~/leica` (Stage 3) |
| `lm`: cannot execute | Must run inside WSL, not cmd/PowerShell |
| `lm`: permission denied | `chmod +x Tools/LibManager/lm` |
| Yocto installer fails in WSL | Capture log; ask Leica; try Hyper-V Linux VM as fallback |
| Line endings break scripts | In repo: `*.sh` LF only; `git config core.autocrlf input` in WSL clone |
| Out of disk in WSL | `wsl --shutdown`, then expand VHD or clean `~/leica` build dirs |
| Captivate GUI flashes / `Killed` | Broken `DCP06.sys` under `Plugin/DCP06/` — see [Captivate Sim §7](DCP06_POSIX_Linux_Captivate_Sim.md#7-dcp06-plugin-crash--important) |
| *“This app may be damaged”* opening DCP06 | Missing `libDCP06.so.sig` — signed `.lxx` from dongled MkEdit; see [§7.1](DCP06_POSIX_Linux_Captivate_Sim.md#71-launch--this-app-may-be-damaged-2026-06-15) |
| Docker `permission denied` | `sudo docker ps` or `newgrp docker` after `usermod -aG docker $USER` |

Reset WSL disk location (advanced): see [Microsoft WSL docs](https://learn.microsoft.com/en-us/windows/wsl/) — only if default install path fills up.

---

## 14. Next steps after WSL is ready

1. ~~Complete Steps 1–7 tooling~~ — **build path done**; dev install shows DCP06 in Linux sim Apps menu.
2. **Signed `DCP06.lxx`** from dongled machine (Pasi) — required to launch the app on Linux sim.
3. Hand off findings to Pasi for **production DCP05** Linux port — see [DCP06_POSIX_SDK_Analysis.md](DCP06_POSIX_SDK_Analysis.md).

---

## 15. Revision history

| Date | Change |
|------|--------|
| 2026-06-13 | Initial WSL2 + Ubuntu 22.04 setup guide (no dual-boot) |
| 2026-06-13 | Stage 2 Yocto SDK installed; Step 6 ARM cross-build verified; checklist and env vars updated |
| 2026-06-14 | Linux Captivate sim guide; Step 7; plugin crash troubleshooting |
| 2026-06-15 | Step 7 Apps menu verified; signed `.lxx` handoff to Pasi |

---

*Windows paths assume user `dell` and AMS Development folder under Desktop. Adjust `CAPTIVATE_POSIX_SDK` and `DCP06_ROOT` if your layout differs.*
