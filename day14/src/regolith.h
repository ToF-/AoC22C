#include <stdbool.h>

typedef struct cave {
    int width;
    int heigth;
    char *content;
} CAVE;

char at(CAVE *cave, int x, int y);
void set(CAVE *cave, int x, int y, char c);
CAVE *read_puzzle(char *filename);
void destroy_cave(CAVE *cave);
bool sand_fall(CAVE *cave);
