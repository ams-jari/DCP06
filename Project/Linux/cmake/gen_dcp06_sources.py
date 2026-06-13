#!/usr/bin/env python3
"""Generate Dcp06Sources.cmake from DCP06-VS2008.vcproj Win32 source list."""
import re
from pathlib import Path

root = Path(__file__).resolve().parents[3]
vcproj = root / "Project" / "MSVS" / "DCP06-VS2008.vcproj"
out = Path(__file__).resolve().parent / "Dcp06Sources.cmake"

text = vcproj.read_text(encoding="latin-1")
paths = sorted(set(re.findall(r'RelativePath="\.\.\\\.\.\\([^"]+\.cpp)"', text)))
paths = [p.replace("\\", "/") for p in paths]

lines = [
    "# Auto-generated from DCP06-VS2008.vcproj — do not edit by hand.",
    "# Regenerate: python3 Project/Linux/cmake/gen_dcp06_sources.py",
    "set(DCP06_VCPROJ_SOURCES",
]
for p in paths:
    lines.append(f'    "${{DCP06_ROOT}}/{p}"')
lines.append(")")

out.write_text("\n".join(lines) + "\n", encoding="utf-8")
print(f"Wrote {len(paths)} sources to {out}")
