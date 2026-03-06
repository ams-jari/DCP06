#!/usr/bin/env python3
"""Update file headers: Leica -> AMS, Pluto/Venus -> DCP06."""
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_DIRS = [os.path.join(ROOT, "Src"), os.path.join(ROOT, "include")]
EXTENSIONS = (".cpp", ".hpp", ".h", ".c")

REPLACEMENTS = [
    (r"Copyright 2002 by Leica Geosystems AG, Heerbrugg", "Copyright (c) AMS. Based on Leica Captivate plugin framework."),
    (r"Copyright by Leica Geosystems AG, Heerbrugg 2002", "Copyright (c) AMS. Based on Leica Captivate plugin framework."),
    (r"Project  : Pluto/Venus Onboard Applications SW", "Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)"),
    (r"Project  : Pluto/Venus", "Project  : DCP06 - Onboard 3D measurement (Leica Captivate plugin)"),
]

def main():
    count = 0
    for src_dir in SRC_DIRS:
        if not os.path.isdir(src_dir):
            continue
        for root, _, files in os.walk(src_dir):
            for f in files:
                if f.lower().endswith(EXTENSIONS):
                    path = os.path.join(root, f)
                    try:
                        with open(path, "r", encoding="utf-8", errors="replace") as fp:
                            content = fp.read()
                        orig = content
                        for pattern, repl in REPLACEMENTS:
                            content = content.replace(pattern, repl)
                        if content != orig:
                            with open(path, "w", encoding="utf-8", newline="") as fp:
                                fp.write(content)
                            count += 1
                            print(path)
                    except Exception as e:
                        print(f"Error {path}: {e}")
    print(f"\nUpdated {count} files.")

if __name__ == "__main__":
    main()
