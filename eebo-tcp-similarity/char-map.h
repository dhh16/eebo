inline char normalise(uint32_t x) {
    switch(x) {
        case    65:  // LATIN CAPITAL LETTER A
            return 'A';
        case    66:  // LATIN CAPITAL LETTER B
            return 'B';
        case    67:  // LATIN CAPITAL LETTER C
            return 'C';
        case    68:  // LATIN CAPITAL LETTER D
            return 'D';
        case    70:  // LATIN CAPITAL LETTER F
            return 'S';
        case    71:  // LATIN CAPITAL LETTER G
            return 'G';
        case    72:  // LATIN CAPITAL LETTER H
            return 'H';
        case    73:  // LATIN CAPITAL LETTER I
            return 'I';
        case    74:  // LATIN CAPITAL LETTER J
            return 'I';
        case    75:  // LATIN CAPITAL LETTER K
            return 'K';
        case    76:  // LATIN CAPITAL LETTER L
            return 'L';
        case    77:  // LATIN CAPITAL LETTER M
            return 'M';
        case    78:  // LATIN CAPITAL LETTER N
            return 'N';
        case    79:  // LATIN CAPITAL LETTER O
            return 'O';
        case    80:  // LATIN CAPITAL LETTER P
            return 'P';
        case    81:  // LATIN CAPITAL LETTER Q
            return 'Q';
        case    82:  // LATIN CAPITAL LETTER R
            return 'R';
        case    83:  // LATIN CAPITAL LETTER S
            return 'S';
        case    84:  // LATIN CAPITAL LETTER T
            return 'T';
        case    85:  // LATIN CAPITAL LETTER U
            return 'V';
        case    86:  // LATIN CAPITAL LETTER V
            return 'V';
        case    87:  // LATIN CAPITAL LETTER W
            return 'V';
        case    88:  // LATIN CAPITAL LETTER X
            return 'X';
        case    89:  // LATIN CAPITAL LETTER Y
            return 'I';
        case    90:  // LATIN CAPITAL LETTER Z
            return 'Z';
        case    97:  // LATIN SMALL LETTER A
            return 'A';
        case    98:  // LATIN SMALL LETTER B
            return 'B';
        case    99:  // LATIN SMALL LETTER C
            return 'C';
        case   100:  // LATIN SMALL LETTER D
            return 'D';
        case   102:  // LATIN SMALL LETTER F
            return 'S';
        case   103:  // LATIN SMALL LETTER G
            return 'G';
        case   104:  // LATIN SMALL LETTER H
            return 'H';
        case   105:  // LATIN SMALL LETTER I
            return 'I';
        case   106:  // LATIN SMALL LETTER J
            return 'I';
        case   107:  // LATIN SMALL LETTER K
            return 'K';
        case   108:  // LATIN SMALL LETTER L
            return 'L';
        case   109:  // LATIN SMALL LETTER M
            return 'M';
        case   110:  // LATIN SMALL LETTER N
            return 'N';
        case   111:  // LATIN SMALL LETTER O
            return 'O';
        case   112:  // LATIN SMALL LETTER P
            return 'P';
        case   113:  // LATIN SMALL LETTER Q
            return 'Q';
        case   114:  // LATIN SMALL LETTER R
            return 'R';
        case   115:  // LATIN SMALL LETTER S
            return 'S';
        case   116:  // LATIN SMALL LETTER T
            return 'T';
        case   117:  // LATIN SMALL LETTER U
            return 'V';
        case   118:  // LATIN SMALL LETTER V
            return 'V';
        case   119:  // LATIN SMALL LETTER W
            return 'V';
        case   120:  // LATIN SMALL LETTER X
            return 'X';
        case   121:  // LATIN SMALL LETTER Y
            return 'I';
        case   122:  // LATIN SMALL LETTER Z
            return 'Z';
        case   224:  // LATIN SMALL LETTER A WITH GRAVE
            return 'A';
        case   225:  // LATIN SMALL LETTER A WITH ACUTE
            return 'A';
        case   226:  // LATIN SMALL LETTER A WITH CIRCUMFLEX
            return 'A';
        case   231:  // LATIN SMALL LETTER C WITH CEDILLA
            return 'C';
        case   237:  // LATIN SMALL LETTER I WITH ACUTE
            return 'I';
        case   238:  // LATIN SMALL LETTER I WITH CIRCUMFLEX
            return 'I';
        case   242:  // LATIN SMALL LETTER O WITH GRAVE
            return 'O';
        case   243:  // LATIN SMALL LETTER O WITH ACUTE
            return 'O';
        case   244:  // LATIN SMALL LETTER O WITH CIRCUMFLEX
            return 'O';
        case   249:  // LATIN SMALL LETTER U WITH GRAVE
            return 'V';
        case   250:  // LATIN SMALL LETTER U WITH ACUTE
            return 'V';
        case   251:  // LATIN SMALL LETTER U WITH CIRCUMFLEX
            return 'V';
        case   373:  // LATIN SMALL LETTER W WITH CIRCUMFLEX
            return 'V';
        case   375:  // LATIN SMALL LETTER Y WITH CIRCUMFLEX
            return 'I';
        case   383:  // LATIN SMALL LETTER LONG S
            return 'S';
        case   434:  // LATIN CAPITAL LETTER V WITH HOOK
            return 'V';
        case  7693:  // LATIN SMALL LETTER D WITH DOT BELOW
            return 'D';
        case  7909:  // LATIN SMALL LETTER U WITH DOT BELOW
            return 'V';
        case  7935:  // LATIN SMALL LETTER Y WITH LOOP
            return 'I';
        case 42833:  // LATIN SMALL LETTER P WITH STROKE THROUGH DESCENDER
            return 'P';
        case 42835:  // LATIN SMALL LETTER P WITH FLOURISH
            return 'P';
        case 42841:  // LATIN SMALL LETTER Q WITH DIAGONAL STROKE
            return 'Q';
        default:
            return 0;
    }
}
