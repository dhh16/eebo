#!/usr/bin/env python3

import json
import re
import unicodedata

NORMALISE = {
    'U': 'V',
    'W': 'V',
    'J': 'I',
    'Y': 'I',
    'E': None,
    'F': 'S',
    'M': None,
    'N': None,
    'C': 'K',
    'O': 'V',
    'D': 'T',
    'Z': 'S',
}

def main():
    with open('eebo-char.json') as f:
        chars = json.load(f)

    total = sum([n for x,n in chars])
    threshold = 1E-6 * total

    wordchar = []
    for x,n in chars:
        if n < threshold:
            continue
        c = chr(x)
        cat = unicodedata.category(c)
        if cat[0] == 'L':
            if cat == 'Lm':
                continue
            name = unicodedata.name(c)
            m = re.fullmatch('LATIN (?:CAPITAL|SMALL) LETTER ([A-Z])(?: WITH.*)?', name)
            if m:
                y = m.group(1)
            elif name == 'LATIN SMALL LETTER LONG S':
                y = 'S'
            else:
                assert name in (
                    'LATIN SMALL LETTER YOGH',
                    'LATIN SMALL LETTER EZH',
                    'LATIN SMALL LETTER MIDDLE-WELSH V',
                    'LATIN SMALL LETTER IS',
                    'LATIN SMALL LETTER CON',
                )
                continue
            if y in NORMALISE:
                y = NORMALISE[y]
            if y is None:
                continue
            wordchar.append((x, y, name))

    print('inline char normalise(uint32_t x) {')
    print('    switch(x) {');
    for x,y,name in wordchar:
        print('        case {:5d}:  // {}'.format(x, name))
        print("            return '{}';".format(y))
    print('        default:')
    print('            return 0;')
    print('    }')
    print('}')

main()
