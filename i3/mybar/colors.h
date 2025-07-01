#include <stdio.h>

#define START_COLOR(color) printf("{\"color\":\"#" color "\",\"separator\":false,\"separator_block_width\":0,\"full_text\":\"")
#define END_COLOR printf("\"},")
#define END_COLOR_FINAL printf("\"}")

#define START_RED    START_COLOR("FF0000")
#define START_YELLOW START_COLOR("FFFF00")
#define START_GREEN  START_COLOR("00FF00")
#define START_GREY   START_COLOR("888888")
#define START_WHITE  START_COLOR("FFFFFF")

#define print_separator START_GREY; printf(" | "); END_COLOR;
#define print_separator_final START_GREY; printf(" |"); END_COLOR_FINAL;
