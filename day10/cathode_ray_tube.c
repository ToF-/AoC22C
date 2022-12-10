#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

const int MAX_INSTRUCTIONS = 1000;

enum opcode { NOOP, ADDX };

struct instruction {
    enum opcode op;
    int  value;
};

int read_instructions(FILE *file, struct instruction *instructions) {
    char line[LINE_MAX];
    int max_instruction = 0;
    while(fgets(line, LINE_MAX, file)) {
        line[strcspn(line, "\n")] = 0;
        char *token =strtok(line, " ");
        struct instruction ins;
        enum opcode op;
        if(!strcmp(token,"noop")) {
            ins.op = NOOP;
            ins.value = -1;
        } else if(!strcmp(token, "addx")) {
            ins.op = ADDX;
            token = strtok(NULL, " ");
            ins.value = atoi(token);
        }
        instructions[max_instruction] = ins;
        max_instruction++;
    }
    return max_instruction;
}

bool collect(int *sum, int cycle, int x) {
    int points[6] = { 20, 60, 100, 140, 180, 220 };
    for(int p = 0; p < 6; p++) {
        if(cycle == points[p]) {
            *sum += cycle * x;
            return true;
        }
    }
    return false;
}

const int MAX_POS = 40;

void pos_sprite(char sprite[MAX_POS], int x) {
    for(int i=0; i<MAX_POS; i++)
        sprite[i] = '.';
    for(int i=x-1; i<x+2; i++)
        sprite[i] = '#';
}

const int CRT_SIZE = 40 * 6;

void draw_crt(char crt[CRT_SIZE], int cycle, char sprite[MAX_POS]) {
    int raster = (cycle-1)%240;
    int row = raster / 40;
    int col = raster % 40;
    crt[row*40+col] = sprite[col];
}

void print_crt(char crt[CRT_SIZE]) {
    for(int r=0; r<6; r++) {
        for(int c=0; c<40; c++)
            printf("%c", crt[r*40+c]);
        printf("\n");
    }
    printf("\n");
}

void init_crt(char crt[CRT_SIZE]) {
    for(int r=0; r<6; r++) {
        for(int c=0; c<40; c++)
            crt[r*40+c] = '.';
    }
}
int main(int argc, char *argv[1]) {
    FILE *puzzle_file;

    if(argc==1) {
        fprintf(stderr,"missing argument: <puzzle.txt>\n");
        return 1;
    }
    struct instruction instructions[MAX_INSTRUCTIONS];
    puzzle_file = fopen(argv[1],"r");
    assert(puzzle_file != NULL);
    int max_instructions = read_instructions(puzzle_file, instructions);
    fclose(puzzle_file);
    int x = 1;
    int cycle = 0;
    int sum = 0;
    char crt[CRT_SIZE];
    init_crt(crt);
    char sprite[MAX_POS];
    pos_sprite(sprite, x);
    for(int i=0; i<max_instructions; i++) {
        struct instruction ins = instructions[i];
        switch(ins.op) {
            case NOOP: 
                cycle++; 
                if(collect(&sum, cycle, x))
                    print_crt(crt);
                pos_sprite(sprite, x);
                draw_crt(crt, cycle, sprite);
                break;
            case ADDX:
                cycle++;
                if(collect(&sum, cycle, x))
                    print_crt(crt);
                pos_sprite(sprite, x);
                draw_crt(crt, cycle, sprite);
                cycle++;
                if(collect(&sum, cycle, x))
                    print_crt(crt);
                pos_sprite(sprite, x);
                draw_crt(crt, cycle, sprite);
                x += ins.value;
                break;
        }
    }
    printf("%d\n", sum);
    pos_sprite(sprite, x);
    draw_crt(crt, cycle, sprite);
    return 0;
}


