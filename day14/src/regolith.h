#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct cave {
    int width;
    int height;
    int xmin;
    int xmax;
    int ymin;
    int ymax;
    bool has_floor;
    char *content;
} CAVE;

char at(CAVE *cave, int x, int y);
void set(CAVE *cave, int x, int y, char c);
CAVE *read_puzzle(char *filename);
void destroy_cave(CAVE *cave);
bool sand_fall(CAVE *cave, int x, int y);
void print_cave(CAVE *cave);
int stopped_sand(CAVE *cave);
void print_debug_cave(CAVE *cave);
int stopped_sand_with_floor(CAVE *cave);
