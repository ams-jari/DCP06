#!/usr/bin/env python3
"""Fix outdated comments and remove commented-out legacy code."""
import os
import re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SRC_DIR = os.path.join(ROOT, "Src")

def main():
    updated = 0
    for root, _, files in os.walk(SRC_DIR):
        for f in files:
            if f.lower().endswith(".cpp"):
                path = os.path.join(root, f)
                try:
                    with open(path, "r", encoding="utf-8", errors="replace") as fp:
                        content = fp.read()
                    orig = content

                    # Fix "Hello World model" -> "DCP06 model"
                    content = content.replace("// Description: Hello World model", "// Description: DCP06 model")

                    # Remove commented-out dynamic_cast lines (legacy from pModel fix)
                    content = re.sub(
                        r'^\s*// DCP::\w+\* pModel = dynamic_cast<[^>]+>\( pModel \);\s*\n',
                        '',
                        content,
                        flags=re.MULTILINE
                    )

                    # Remove "// only accept hello world Model" type comments
                    content = content.replace("// Description: only accept hello world Model objects", "// Description: only accept DCP06 Model objects")

                    if content != orig:
                        with open(path, "w", encoding="utf-8", newline="") as fp:
                            fp.write(content)
                        updated += 1
                        print(path)
                except Exception as e:
                    print(f"Error {path}: {e}")

    print(f"\nUpdated {updated} files.")

if __name__ == "__main__":
    main()
