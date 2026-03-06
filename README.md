# DCP06

**Onboard 3D measurement software for Leica total stations** (Leica Captivate plugin).

Maintained by AMS. Based on DCP05, with planned database-driven architecture and DCP9-inspired features.

## Repository

- **GitHub:** https://github.com/ams-jari/DCP06

## Project Structure (Option B layout)

```
DCP06/
├── Docs/                    # Documentation (project plan, summaries)
├── include/dcp06/           # Public headers (domain subdirs)
│   ├── core/               # Core types, models, UI base
│   ├── init/               # Initialization, user, units
│   ├── orientation/        # 321 Alignment, Best Fit, CHST, plane/line
│   ├── measurement/        # 3D meas, circle, scan
│   ├── calculation/        # Calc dist/angle, view CDF/AGF
│   ├── file/               # File I/O, ADF, select file
│   └── application/        # Application, shaft, line setting
├── src/                     # C++ source (same domain subdirs)
│   ├── core/               # stdafx, CS35, common
│   ├── init/               # Initialization, user, unit
│   ├── orientation/        # 321 Alignment, Best Fit, CHST, define plane/line
│   ├── measurement/       # 3D meas, circle, scan
│   ├── calculation/       # Calc dist/angle, view
│   ├── application/       # Application entry, shaft, line setting
│   └── math/              # Math library (unchanged)
├── tests/unit/             # Unit tests (placeholder)
├── scripts/                # Build and restructure scripts
├── Project/                 # Build configuration
│   ├── DCP06.dat           # Version/build metadata
│   ├── DCP06.sys           # Captivate app registration (App.Id=15751)
│   └── MSVS/               # Visual Studio projects
├── SWXRes/                  # UI resources (PNG, built from SWXRes_Source)
├── SWXRes_Source/           # SVG sources, DCP06_logo.svg
├── Text/                    # Localization tokens
└── README.md
```

## Build Targets (same as DCP05)

| Platform | Solution | Output |
|----------|----------|--------|
| MS60, TS16, CS20 (physical) | DCP06-VS2008.sln (VS 2008) | DCP06.dll (ARM) |
| Win32 desktop | DCP06-VS2008.sln (VS 2008) | DCP06.dll (x86) |
| CS35 Simulator | DCP06-VS2022.sln (VS 2022) | DCP06.dll (x86/x64) |

## Prerequisites

- Leica Captivate System 1500 SDK
- Visual Studio 2008 (instrument) and/or VS 2022 (CS35)
- Environment variables: `SYSTEM1500_ROOT`, `SYSTEM1500_ROOT_x64`

## Build Setup

1. Set `SYSTEM1500_ROOT` and `SYSTEM1500_ROOT_x64` to your Leica SDK paths.
2. For MkEdit packaging: ensure DCP06 is available at  
   `%SYSTEM1500_ROOT%\Applications\Common\DCP06\`  
   (copy or symlink from your development folder).
3. Open `Project/MSVS/DCP06-VS2022.sln` (CS35) or `Project/MSVS/DCP06-VS2008.sln` (instrument).
4. Build Release|Win32 or Release|x64 (CS35) or Release|Stellar_EC7_3P (instrument).

## Documentation

See [Docs/README.md](Docs/README.md) for DCP05/DCP9 summaries and DCP06 project plan.
