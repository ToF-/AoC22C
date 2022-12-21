#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "regolith.h"
#include "ctype.h"
#include "stdbool.h"

const int MAX_heigth = 500;
const int MAX_WIDTH  = 500;
const int MAX_ENTRIES= 100;

const char AIR  = '.';
const char ROCK = '#';
const char SAND = 'o';

char at(CAVE *cave, int x, int y);
void set(CAVE *cave, int x, int y, char c);
CAVE *read_puzzle(char *filename);
void destroy_cave(CAVE *cave);
bool sand_fall(CAVE *cave);


void destroy_cave(CAVE *cave) {
    free(cave->content);
    free(cave);
}
char at(CAVE *cave, int x, int y){
    return cave->content[y*cave->width+x];
}

void set(CAVE *cave, int x, int y, char c) {
    cave->content[y*cave->width+x] = c;
}
void scan_segment(CAVE *cave, int x0, int y0, int x1, int y1) {
    if(x0 == x1) {
        if(y1 < y0) {
            int temp = y1;
            y1 = y0;
            y0 = temp;
        }
        for(int y = y0; y <= y1; y++)
            set(cave,x0,y,ROCK);
    } else {
        assert(y0 == y1);
        if(x1 < x0) {
            int temp = x1;
            x1 = x0;
            x0 = temp;
        }
        for(int x = x0; x <= x1; x++)
            set(cave,x,y0,ROCK);
    }
}

const int MAX_ENTRY = 2500;
const int MAX_LINE = 1000;
char buffer[MAX_LINE];

CAVE *read_puzzle(char *filename) {
    int entry[MAX_ENTRY];
    int count = 0;
    int width = 0;
    int heigth = 0;
    count = 0;
    FILE *puzzle_file = fopen(filename, "r");
    char line[MAX_LINE];
    while(fgets(line, MAX_LINE, puzzle_file)) {
        int l = strcspn(line, "\n");
        line[l] = '\0';
        strcpy(buffer, line);
        char *word = strtok(buffer, " ");
        while(word != NULL) {
            if(strcmp(word, "->")) {
                int x,y;
                sscanf(word, "%d,%d", &x, &y);
                entry[count++] = x;
                entry[count++] = y;
                if(width < x)
                    width = x;
                if(heigth < y)
                    heigth = y;
            }
            word = strtok(NULL, " ");
        }
    }
    fclose(puzzle_file);
    CAVE *cave = (CAVE *)malloc(sizeof(cave));
    cave->heigth = heigth;
    cave->width = width;
    cave->content = (char *)malloc(sizeof(char)*width*heigth);
    for(int y=0; y<cave->heigth; y++)
        for(int x=0; x<cave->width; x++) {
            set(cave, x, y, AIR);
        }
    return cave;
}

bool sand_fall(CAVE *cave) {
    int x=500; int y=0;
    return true;
}
