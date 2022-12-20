#include <stdbool.h>
char *new_cave(int size);
void scan_path(char *cave, char *line);
char at(char *cave, int x, int y);
void set(char *cave, int x, int y, char c);
char *read_puzzle(char *filename);
void destroy_cave(char *cave);
bool sand_fall(char *cave);
