#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>

const int MAX_INSTRUCTIONS = 2000;

const int MAX_KNOTS = 10;

struct instruction{
    char direction;
    int length;
};

struct location{
    int x;
    int y;
};

int compare(const void *a, const void *b) {
    struct location arg1 = *(struct location *)a;
    struct location arg2 = *(struct location *)b;
    if(arg1.x > arg2.x) return 1;
    if(arg1.x < arg2.y) return -1;
    return(arg1.y - arg2.y);
}

void move(struct location target, struct location [MAX_KNOTS] rope) {
    struct location previous = rope[0];
    rope[0].x = target.x;
    rope[0].y = target.y;
    int k = 0;
    do {
        int diffx = rope[k]->x - rope[k+rope[k+1->x;
        int diffy = rope[k]->y - previous->y;
        int diffx2 = diffx * diffx;
        int diffy2 = diffy * diffy;
        if(diffx2 > 1 || diffy2 > 1) {
            rope[k+1]->x = rope[k].x;
            rope[k+1]->y = rope[k].y;
    }
}

int main(int argc, char *argv[]) {
    FILE *puzzle_file;
    char line[LINE_MAX];
    if(argc==1) {
        fprintf(stderr,"missing argument: <puzzle.txt>\n");
        return 1;
    }
    struct instruction instructions[MAX_INSTRUCTIONS];
    int max_instructions = 0;
    int max_locations = 0;

    puzzle_file = fopen(argv[1],"r");
    while(fgets(line, LINE_MAX, puzzle_file)) {
        line[strcspn(line, "\n")] = 0;
        char *token =strtok(line, " ");
        char direction = token[0];
        token = strtok(NULL, " ");
        int length = atoi(token);
        instructions[max_instructions].direction = direction;
        instructions[max_instructions].length = length;
        max_locations += length;
        max_instructions++;
    }
    fclose(puzzle_file);
    printf("%d instructions\n", max_instructions);
    struct location *locations = (struct location*)malloc(sizeof(struct location) * max_locations);
    struct location rope[MAX_KNOTS];
    for(int k=0; k<MAX_KNOTS; k++) {
        rope[k].x = 0;
        rope[k].y = 0;
    }
    int location_max = 0;
    int distinct_locations = 0;
    for(int i=0; i<max_instructions; i++) {
        int length = instructions[i].length;
        char direction = instructions[i].direction;
        for(int l = 0; l < length; l++) {
            printf("location:%d head:(%d,%d), tail:(%d,%d)\n", 
                    location_max, 
                    rope[0].x, 
                    rope[0].y, 
                    rope[MAX_KNOTS-1].x, 
                    rope[MAX_KNOTS-1].y);
            int dx = 0;
            int dy = 0;
            switch(direction) {
                case 'U' : dy = 1; break;
                case 'D' : dy = -1; break;
                case 'L' : dx = -1; break;
                case 'R' : dx = 1; break;
            }
            struct location target;
            target.x = rope[0].x + dx;
            target.y = rope[0].y + dy;
            move(target, rope, MAX_KNOTS);
            }
            int index = -1;
            for(int j=0; j<location_max; j++) {
                if(rope[MAX_KNOTS-1].x == locations[j].x && rope[MAX_KNOTS-1].y == locations[j].y) {
                    index = j;
                    break;
                }
            }
            if(index == -1) {
                locations[location_max] = rope[MAX_KNOTS-1];
                location_max++;
            }
        }
    }
    printf("max distinct locations:%d\n", location_max);
    return 0;
}
