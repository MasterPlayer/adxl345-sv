#include "text_color.h"


void textcolor(enum style_enum style, enum color_enum foreground, enum color_enum background) {
	char command[13];
	sprintf(command, "%c[%d;%d;%dm", 0x1B, style, foreground + 30, background + 40);
	printf("%s", command);
}
