#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

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

void compute_distance(struct location a, struct location b, int *dxx, int *dyy) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    *dxx = dx * dx;
    *dyy = dy * dy;
}

int read_instructions(FILE *file, struct instruction *instructions) {
    char line[LINE_MAX];
    int i = 0;
    while(fgets(line, LINE_MAX, file)) {
        line[strcspn(line, "\n")] = 0;
        char *token =strtok(line, " ");
        char direction = token[0];
        token = strtok(NULL, " ");
        int length = atoi(token);
        instructions[i].direction = direction;
        instructions[i].length = length;
        i++;
    }
    return i;
}

struct location pull(struct location target, struct location coord) {
    struct location result = coord;
    int dxx;
    int dyy;
    compute_distance(target, coord, &dxx, &dyy);
    assert(dxx == 0 || dxx == 1 || dxx == 4);
    assert(dyy == 0 || dyy == 1 || dyy == 4);
        if(dxx == 4 && dyy == 4) {
            int dx = target.x - coord.x;
            result.x += dx / 2;
            int dy = target.y - coord.y;
            result.y += dy / 2;
        }
        else if(dxx > dyy) {
            int dx = target.x - coord.x;
            result.x += dx / 2;
            result.y = target.y;
        }
        else if(dyy > dxx) {
            int dy = target.y - coord.y;
            result.y += dy / 2;
            result.x = target.x;
        }
    return result;
}

struct location move(struct location coord, int dx, int dy) {
    struct location result = coord;
    result.x += dx;
    result.y += dy;
    return result;
}

bool same(struct location a, struct location b) {
    return a.x == b.x && a.y == b.y;
}

bool assertpull(struct location target, struct location coord, struct location expected) {
    struct location result = pull(target, coord);
    // printf("(%d,%d)<-(%d,%d) = (%d,%d) ? (%d,%d)\n",target.x, target.y, coord.x, coord.y, expected.x, expected.y, result.x, result.y);
    assert(same(result, expected));
    return true;
}

bool assertmove(struct location coord, int dx, int dy, struct location expected) {
    struct location result = move(coord, dx, dy);
    assert(same(result, expected));
    return true;
}
bool tests() {

    assertpull((struct location){0,0}, (struct location){0,0}, (struct location){0,0});
    assertpull((struct location){1,0}, (struct location){0,0}, (struct location){0,0});
    assertpull((struct location){-1,0}, (struct location){0,0}, (struct location){0,0});
    assertpull((struct location){0,1}, (struct location){0,0}, (struct location){0,0});
    assertpull((struct location){0,-1}, (struct location){0,0}, (struct location){0,0});
    assertpull((struct location){1,-1}, (struct location){0,0}, (struct location){0,0});
    assertpull((struct location){-1,-1}, (struct location){0,0}, (struct location){0,0});
    assertpull((struct location){-1,1}, (struct location){0,0}, (struct location){0,0});
    assertpull((struct location){1,1}, (struct location){0,0}, (struct location){0,0});
    assertpull((struct location){2,0}, (struct location){0,0}, (struct location){1,0});
    assertpull((struct location){-2,0}, (struct location){0,0}, (struct location){-1,0});
    assertpull((struct location){0,2}, (struct location){0,0}, (struct location){0,1});
    assertpull((struct location){0,-2}, (struct location){0,0}, (struct location){0,-1});
    assertpull((struct location){2,1}, (struct location){0,0}, (struct location){1,1});
    assertpull((struct location){1,2}, (struct location){0,0}, (struct location){1,1});
    assertpull((struct location){-2,1}, (struct location){0,0}, (struct location){-1,1});
    assertpull((struct location){-1,2}, (struct location){0,0}, (struct location){-1,1});
    assertpull((struct location){-1,-2}, (struct location){0,0}, (struct location){-1,-1});
    assertpull((struct location){1,-2}, (struct location){0,0}, (struct location){1,-1});
    assertpull((struct location){2,2}, (struct location){0,0}, (struct location){1,1});
    
    assertmove((struct location){0,0}, 1, 0, (struct location) {1,0});
    assertmove((struct location){0,0}, -1, 0, (struct location) {-1,0});
    assertmove((struct location){0,0}, 0, 1, (struct location) {0, 1});
    assertmove((struct location){0,0}, 0, -1, (struct location) {0,-1});
    return true;
}
int main(int argc, char *argv[]) {
    FILE *puzzle_file;
    if(argc==1) {
        fprintf(stderr,"missing argument: <puzzle.txt>\n");
        return 1;
    }
    if(!strcmp(argv[1],"test")) {
        if(! tests()) {
            return 1;
        }
        return 0;
    }
    struct instruction instructions[MAX_INSTRUCTIONS];
    int max_instructions;

    puzzle_file = fopen(argv[1],"r");
    max_instructions = read_instructions(puzzle_file, instructions);
    printf("%d instructions\n", max_instructions);
    fclose(puzzle_file);


    int max_locations = 0;
    for(int i = 0; i < max_instructions; i++) {
        max_locations += instructions[i].length;
    }
    printf("%d locations\n", max_locations);

    struct location *locations = (struct location*)malloc(sizeof(struct location) * max_locations);

    struct location rope[MAX_KNOTS];
    const struct location START = { 0, 0 };
    for(int k=0; k<MAX_KNOTS; k++) {
        rope[k] = START;
    }
    int max_visited = 0;
    for(int i=0; i<max_instructions; i++) {
        int length = instructions[i].length;
        char direction = instructions[i].direction;
        int dx = 0;
        int dy = 0;
        switch(direction) {
            case 'U' : dy = 1; break;
            case 'D' : dy = -1; break;
            case 'L' : dx = -1; break;
            case 'R' : dx = 1; break;
        }
        for(int l = 0; l < length; l++) {
            rope[0] = move(rope[0], dx, dy);
            for(int k=1; k<MAX_KNOTS; k++) {
                rope[k] = pull(rope[k-1],rope[k]);
            }
            for(int k=0; k<MAX_KNOTS; k++)
                printf("(%d,%d)", rope[k].x, rope[k].y);
            printf("\n");
            struct location visited = rope[MAX_KNOTS-1];
            int index; 
            for(index = 0; index < max_visited; index++) {
                if(same(locations[index], visited))
                    break;
            }
            if(index == max_visited) {
                locations[max_visited++] = visited;
            }
        }
    }
    printf("max distinct locations:%d\n", max_visited);
    return 0;
}
