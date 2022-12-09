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
/*
 *   TH. -> T.H -> .TH
 *
 *           H
 *   TH. -> T.
 *
 */
void pull(struct location *target, struct location *head, struct location *tail) {
    printf("(%d,%d)", target->x, target->y);
    struct location temp = *head;
    head->x = target->x;
    head->y = target->y;
    int dx = head->x - tail->x;
    int dy = head->y - tail->y;
    int dx2 = dx * dx;
    int dy2 = dy * dy;
    if(dx2 > 1 || dy2 > 1)
    {
        tail->x = temp.x;
        tail->y = temp.y;
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
            pull(&target, &rope[0], &rope[1]);
            int k;
            for(k=0; k<MAX_KNOTS-2; k++)
                pull(&rope[k],&rope[k+1],&rope[k+2]);
            int index = 0;
            for(index = 0; index < location_max; index++) {
                if(locations[index].x == rope[MAX_KNOTS-1].x && locations[index].y == rope[MAX_KNOTS-1].y) {
                    printf("same location as #%d(%d,%d)\n",index, locations[index].x, locations[index].y);
                    break;
                }
            }
            if(index == location_max) {
                locations[index] = rope[MAX_KNOTS-1];
                printf("new location#%d %d:(%d,%d)\n", index, location_max, locations[index].x, locations[index].y);
                location_max++;
            }
        }
    }
    printf("max distinct locations:%d\n", location_max);
    return 0;
}
