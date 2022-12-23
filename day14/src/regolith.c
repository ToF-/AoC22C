#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "regolith.h"
#include "ctype.h"
#include "stdbool.h"
#include "limits.h"

const int MAX_height = 500;
const int MAX_WIDTH  = 500;
const int MAX_ENTRIES= 100;

const char AIR  = '.';
const char ROCK = '#';
const char SAND = 'o';

void destroy_cave(CAVE *cave) {
    printf("destroy_cave\n");
    print_debug_cave(cave);
//    free(cave->content);
    cave->content = NULL;
    free(cave);
    cave = NULL;
}
char at(CAVE *cave, int x, int y){
    if(cave->has_floor && y >= cave->ymax+2)
        return ROCK;
    return cave->content[(y*cave->width)+x];
}

void set(CAVE *cave, int x, int y, char c) {
    assert(x >= 0 && x < cave->width);
    assert(y >= 0 && y < cave->height);
    assert(cave->width > 0);
    assert(cave->height > 0);
    assert((y*cave->width+x) < (cave->width*cave->height));
    cave->content[y*cave->width+x] = c;
    if(c == ROCK) {
        cave->xmin = x < cave->xmin ? x : cave->xmin;
        cave->xmax = x > cave->xmax ? x : cave->xmax;
        cave->ymin = y < cave->ymin ? y : cave->ymin;
        cave->ymax = y > cave->ymax ? y : cave->ymax;
    }
}
void scan_segment(CAVE *cave, int x0, int y0, int x1, int y1) {
    assert(x0 >= 0 && x0 < cave->width);
    assert(y0 >= 0 && y0 < cave->height);
    assert(x1 >= 0 && x1 < cave->width);
    assert(y1 >= 0 && y1 < cave->height);
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

const int MAX_ENTRY = 10000;
const int LINE_SIZE = 1000;

CAVE *read_puzzle(char *filename) {
    printf("scanning %s\n", filename);
    int entry[MAX_ENTRY];
    int count = 0;
    int width = 0;
    int height = 0;
    count = 0;
    char buffer[LINE_SIZE];
    char line[LINE_SIZE];
    FILE *puzzle_file = fopen(filename, "r");
    int lineno=0;
    while(fgets(line, LINE_SIZE, puzzle_file)) {
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
                if(height < y)
                    height = y;
            }
            word = strtok(NULL, " ");
        }
        entry[count++] = -1;
        entry[count++] = -1;
    }
    fclose(puzzle_file);
    CAVE *cave = (CAVE *)malloc(sizeof(cave));
    cave->height = height*2;
    cave->width = width*2;
    int total_size = cave->width * cave->height;
    assert(total_size > 0);
    cave->content = (char *)malloc(sizeof(char)*total_size);
    memset(cave->content, AIR, sizeof(char)*total_size);
    cave->xmin = 10000;
    cave->ymin = 10000;
    cave->xmax = -10000;
    cave->ymax = -10000;
    cave->has_floor = false;
    assert(cave->content !=NULL);
    for(int i = 0; i < count-2; i+=2) {
        if(entry[i] != -1 && entry[i+2] != -1)
            scan_segment(cave, entry[i], entry[i+1], entry[i+2], entry[i+3]);
        else
            i+=2;
    }
    print_debug_cave(cave);
    return cave;
}
void print_debug_cave(CAVE *cave) {
    printf("@cave:\t\t%p\n", (void *)cave);
    printf("@cave->width:\t%p\t%d\n", (void *)&cave->width, cave->width);
    printf("@cave->height:\t%p\t%d\n", (void *)&cave->height, cave->height);
    printf("@cave->xmin:\t%p\t%d\n", (void *)&cave->xmin, cave->xmin);
    printf("@cave->xmax:\t%p\t%d\n", (void *)&cave->xmax, cave->xmax);
    printf("@cave->ymin:\t%p\t%d\n", (void *)&cave->ymin, cave->ymin);
    printf("@cave->ymax:\t%p\t%d\n", (void *)&cave->ymax, cave->ymax);
    printf("@cave->content:\t%p\n", (void *)cave->content);
}

bool free_to_fall(CAVE *cave, int x, int y) {
    if(cave->has_floor)
        return true;
    return x >= cave->xmin && x < cave->xmax && y < cave->ymax;
}

bool sand_fall(CAVE *cave, int x, int y) {
    if(at(cave, x, y) != AIR)
        return false;
    while(free_to_fall(cave, x, y)) {
        if(at(cave, x, y+1) == AIR) {
            y++;
        } else {
            if(at(cave, x-1, y+1) == AIR) 
                return sand_fall(cave, x-1, y+1);
            if(at(cave, x+1, y+1) == AIR)
                return sand_fall(cave, x+1, y+1);
            set(cave, x, y, SAND);
            return true;
        }
    }
    return false;
}

void print_cave(CAVE *cave) {
    for(int y = cave->ymin; y <= cave->ymax; y++) {
        for(int x = cave->xmin; x <= cave->xmax; x++) 
            printf("%c", at(cave, x, y));
        printf("\n");
    }
}

int stopped_sand(CAVE *cave) {
    int count = 0;
    while(sand_fall(cave, 500, 0))
        count++;

    return count;
}

int stopped_sand_with_floor(CAVE *cave) {
    cave->has_floor = true;
    int count = 0;
    while(sand_fall(cave, 500, 0))
        count++;
    return count;
}
