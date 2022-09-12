#pragma once

#include <stdio.h>

enum color_enum {
    BLACK       = 0x00,
    BLUE        = 0x01,
    GREEN       = 0x02,
    CYAN        = 0x03,
    RED         = 0x04,    
    MAGENTA     = 0x05,
    YELLOW      = 0x09,
    WHITE       = 0x0F,
    STD         = 0x0F
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
