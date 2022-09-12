#include "text_color.h"
#include <windows.h>

void textcolor(enum style_enum style, enum color_enum foreground, enum color_enum background) {
	//printf("%c[%d;%d;%dm", 0x1B, style, foreground + 30, background + 40);
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (foreground == STD && background == STD) {
		SetConsoleTextAttribute(hConsole, WHITE + (BLACK << 4));
	}
	else {
		SetConsoleTextAttribute(hConsole, foreground + (background << 4));
	}

	if (background == STD) {
		SetConsoleTextAttribute(hConsole, foreground + (BLACK << 4));
	}

}
