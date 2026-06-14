# DCP06 Documentation

This folder contains the planning and reference documentation for the DCP06 project.

## Documents

| Document | Description |
|----------|-------------|
| [DCP05_Summary.md](DCP05_Summary.md) | Detailed technical summary of DCP05 (onboard 3D measurement for Leica total stations) |
| [DCP9_Summary.md](DCP9_Summary.md) | Detailed technical summary of DCP9 (Windows tablet app for Leica laser tracker) |
| [DCP06_Project_Plan.md](DCP06_Project_Plan.md) | Detailed project plan for DCP06 development |
| [DCP06_Architecture_Review.md](DCP06_Architecture_Review.md) | Architecture review and Option B restructure |
| [DCP06_Database_Design.md](DCP06_Database_Design.md) | Database merge from DCP9, shared design, future AMS lib |
| [DCP06_Points_Only_Database_Design.md](DCP06_Points_Only_Database_Design.md) | Points-only DB, PICK/LIST, compiler switches for Circle/Midpoint |
| [DCP06_LIST_PICK_Design.md](DCP06_LIST_PICK_Design.md) | LIST (tag-filtered) vs PICK (all points with actual values) |
| [DCP06_LIST_PICK_Implementation_Plan.md](DCP06_LIST_PICK_Implementation_Plan.md) | Implementation status and phases for LIST/PICK |
| [DCP06_Logging_Trace_Usage.md](DCP06_Logging_Trace_Usage.md) | How to add debug traces (MSVC-safe patterns) |
| [DCP06_Debugging_Crash_Dumps.md](DCP06_Debugging_Crash_Dumps.md) | Capturing and analyzing crash dumps |
| [DCP06_Build_PostBuild_Troubleshooting.md](DCP06_Build_PostBuild_Troubleshooting.md) | MkEdit DLL “file open failed” (exit DCP06 / simulator); dongle vs dev builds |
| [DCP06_POSIX_SDK_Analysis.md](DCP06_POSIX_SDK_Analysis.md) | POSIX SDK analysis + **context/goals** (DCP05 vs DCP06, TS20, handoff to Pasi) |
| [DCP06_POSIX_WSL_Setup.md](DCP06_POSIX_WSL_Setup.md) | WSL2 + Ubuntu 22.04 setup for POSIX SDK (not dual-boot) |
| [DCP06_POSIX_Linux_Build_Steps.md](DCP06_POSIX_Linux_Build_Steps.md) | Step-by-step reconnaissance build (header smoke → full `DCP06.so`); findings feed **DCP05 Linux** |
| [DCP06_POSIX_Linux_Captivate_Sim.md](DCP06_POSIX_Linux_Captivate_Sim.md) | Linux Captivate v10 on WSL: startup, Docker, Step 7, plugin crash recovery |

## Quick Reference

- **Repository:** https://github.com/ams-jari/DCP06
- **Base:** DCP05 (Captivate plugin, total station). Maintained by AMS.
- **Feature sources:** DCP9 (database, job management, alignments), PMP9 (minor reference)
- **Key migration:** ASCII file system (DCP05) → Database (DCP06, like DCP9)

### POSIX / TS20 Linux (reconnaissance)

- **Customer product on TS20:** DCP05 Linux port (Pasi Ojaniemi, `DCP05` repo).
- **This repo’s Linux work:** Learn Captivate POSIX SDK, prove build steps, document pitfalls for DCP05 — **not** a reseller-facing DCP06 Linux release.
- **Start here:** [DCP06_POSIX_SDK_Analysis.md — Context & goals](DCP06_POSIX_SDK_Analysis.md#context--goals) → [WSL setup](DCP06_POSIX_WSL_Setup.md) → [Build steps](DCP06_POSIX_Linux_Build_Steps.md) → [Linux Captivate sim](DCP06_POSIX_Linux_Captivate_Sim.md).
