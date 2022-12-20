#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "regolith.h"
#include "ctype.h"
#include "stdbool.h"

const int MAX_HEIGHT = 500;
const int MAX_WIDTH  = 500;
const int MAX_ENTRIES= 100;

const char AIR  = '.';
const char ROCK = '#';
const char SAND = 'o';

char *new_cave(int size) {
    char *cave = (char *)malloc(size * size);
    for(int i = 0; i < size * size; i++)
        cave[i] = AIR;
    return cave;
}

void destroy_cave(char *cave) {
    free(cave);
}
char at(char *cave, int x, int y){
    return cave[y*MAX_WIDTH+x];
}

void set(char *cave, int x, int y, char c) {
    cave[y*MAX_WIDTH+x] = c;
}
void scan_segment(char *cave, int x0, int y0, int x1, int y1) {
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
void scan_path(char *cave, char *line) {
    char c;
    int entry[MAX_ENTRIES];
    bool in_number = false;
    int count = 0;
    entry[count] = 0;
    do {
        c = *line++;
        switch(c) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                if(!in_number) {
                    in_number = true;
                    entry[count] = c - '0';
                } else {
                    entry[count] = entry[count] * 10 + c-'0';
                }
                break;
            default:
                if(in_number) {
                    in_number = false;
                    count++;
                    if((count % 2 == 0) && (count >= 4)) {
                        scan_segment(cave, entry[count-4],entry[count-3],entry[count-2],entry[count-1]);
                    }
                }
        }
    }while(c != '\0');
};

const int MAX_LINE = 1000;
char *read_puzzle(char *filename) {
    FILE *puzzle_file = fopen(filename, "r");
    assert(puzzle_file);
    char *cave = new_cave(MAX_WIDTH);
    char line[MAX_LINE];
    while(fgets(line, MAX_LINE, puzzle_file)) {
        int l = strcspn(line, "\n");
        line[l] = '\0';
        scan_path(cave, line);
    }
    fclose(puzzle_file);
    return cave;
}

bool sand_fall(char *cave) {
    int x=500; int y=0;

    while(at(cave, x,y+1) == AIR)
        y++;
    if(at(cave, x-1, y+1) == AIR)) {
        x=x-1;
        y=y+1;

    while(at(cave, x-1,y+1) == AIR)
    set(cave, x,y, SAND);


    return true;
}
