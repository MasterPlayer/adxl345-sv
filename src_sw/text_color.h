#pragma once

#include <stdio.h>

enum color_enum {
    BLACK       = 0,
    RED         = 1,
    GREEN       = 2,
    YELLOW      = 3,
    BLUE        = 4,
    MAGENTA     = 5,
    CYAN        = 6,
    WHITE       = 7,
    STD         = 9
};

enum style_enum {
    DEFAULT   = 0,
    BRIGHT    = 1,
    DIM       = 2,
    UNDERLINE = 4,
    BLINK     = 4,
    REVERSE   = 7,
    HIDDEN    = 8
};


void textcolor(enum style_enum style, enum color_enum foreground, enum color_enum background);
