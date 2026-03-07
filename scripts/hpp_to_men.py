#!/usr/bin/env python3
"""
Generate DCP06.men from DCP06_TOK.HPP for TextTool.
DCP06 has no .men source; this script reverse-engineers it from the HPP.
Run before TextTool to produce DCP06.LEN for simulator localization.
"""
import re
import sys
from pathlib import Path
from datetime import datetime
from typing import List, Tuple

# Token prefix -> TextTool Type
PREFIX_TO_TYPE = {
    'L_': 'Line', 'C_': 'Line', 'P_': 'Prompt', 'T_': 'Title',
    'K_': 'Key', 'V_': 'Value', 'M_': 'Message', 'H_': 'Help', 'A_': 'Line',
}


def infer_type(token_name: str) -> str:
    for prefix, t in PREFIX_TO_TYPE.items():
        if token_name.startswith(prefix):
            return t
    return 'Line'


def parse_hpp(hpp_path: Path) -> List[Tuple[int, str, str]]:
    """Parse DCP06_TOK.HPP, return [(number, name_without_TOK, text), ...]"""
    content = hpp_path.read_text(encoding='utf-8', errors='replace')
    tokens = []
    # Match: TOKEN_NAME = number, // "text"  or  TOKEN_NAME = number; // "text"
    pattern = re.compile(
        r'(\w+_TOK)\s+=\s+(\d+)\s*[;,]\s*//\s*"([^"]*)"',
        re.MULTILINE
    )
    for m in pattern.finditer(content):
        full_name, num_str, text = m.groups()
        name = full_name[:-4] if full_name.endswith('_TOK') else full_name  # strip _TOK
        tokens.append((int(num_str), name, text))
    return sorted(tokens, key=lambda t: t[0])


def write_men(tokens: List[Tuple[int, str, str]], out_path: Path, app_name: str = 'DCP06', app_id: int = 15751) -> None:
    now = datetime.now().strftime('%Y-%m-%dT%H:%M:%S')
    lines = [
        '<?xml version="1.0" encoding="utf-8"?>',
        '<Language>',
        '  <Descriptions>',
        '    <Description><Name>Application Article No</Name><Value>49</Value></Description>',
        f'    <Description><Name>Application ID</Name><Value>{app_id}</Value></Description>',
        f'    <Description><Name>Application Name</Name><Value>{app_name}_Text</Value></Description>',
        '    <Description><Name>Application Prefix</Name><Value>DCP</Value></Description>',
        '    <Description><Name>Application Version</Name><Value>1</Value></Description>',
        '    <Description><Name>Author</Name><Value>AMS</Value></Description>',
        '    <Description><Name>Font Name</Name><Value>Tahoma</Value></Description>',
        '    <Description><Name>Platform</Name><Value>Captivate</Value></Description>',
        '    <Description><Name>Start Token</Name><Value>0</Value></Description>',
        '    <Description><Name>TextToolVersion</Name><Value>3.0.1.0</Value></Description>',
        '    <Description><Name>Text-Type</Name><Value>Application Text</Value></Description>',
        '  </Descriptions>',
        '  <TypesDef>AppTypes</TypesDef>',
        '  <Tokens>',
    ]
    for num, name, text in tokens:
        typ = infer_type(name)
        # Escape XML special chars in text
        text_esc = text.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;').replace('"', '&quot;')
        lines.append(f'    <Token>')
        lines.append(f'      <Number>{num}</Number>')
        lines.append(f'      <Name>{name}</Name>')
        lines.append(f'      <Text>{text_esc}</Text>')
        lines.append(f'      <Type>{typ}</Type>')
        lines.append(f'      <Comment>-</Comment>')
        lines.append(f'      <EditDate>{now}</EditDate>')
        lines.append(f'    </Token>')
    lines.extend(['  </Tokens>', '</Language>'])
    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text('\n'.join(lines), encoding='utf-8')


def main():
    repo = Path(__file__).resolve().parent.parent
    hpp = repo / 'Text' / 'DCP06_TOK.HPP'
    out = repo / 'Text' / 'Languages' / 'en' / 'DCP06.men'
    if not hpp.exists():
        print(f'Error: {hpp} not found', file=sys.stderr)
        sys.exit(1)
    tokens = parse_hpp(hpp)
    if not tokens:
        print('Error: no tokens parsed from HPP', file=sys.stderr)
        sys.exit(1)
    write_men(tokens, out)
    print(f'Generated {out} ({len(tokens)} tokens)')


if __name__ == '__main__':
    main()
