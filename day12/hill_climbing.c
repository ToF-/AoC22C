#include <stdio.h>
#include <string.h>
#include <limits.h>

const int MAX_ROW = 50;
const int MAX_COL =100;

typedef struct { 
    int max_row;
    int max_col;
    char squares[MAX_ROW][MAX_COL];
} MAP;

typedef struct {
    int row;
    int col;
} COORD;

void read_puzzle(FILE *puzzle_file, MAP *map) {
    char line[MAX_COL];
    int col;
    int row = 0;
    while(fgets(line, MAX_COL, puzzle_file)) {
        line[strcspn(line, "\n")] = 0;
        for(col = 0; col < strlen(line); col++) {
            map->squares[row][col] = line[col];
        }
        row++;
    }
    map->max_row = row;
    map->max_col = col;
}

int possible_steps(MAP *map, int row, int col, COORD steps[4]) {
    int step_count = 0;
    char from = map->squares[row][col];
    for(int i = row-1; i<=row+1; i++) {
        for(int j = col-1; j<=col+1; j++) {
            if(i >= 0 && i < map->max_row && j >= 0 && j < map->max_col) {
                int dist = (row-i)*(row-i) + (col-j)*(col-j);
                if((i == row && j == col) || dist != 1)
                    continue;
                char to = map->squares[i][j];
                int diff = to-from;
                if(diff <= 1) {
                    COORD coord;
                    coord.row = i;
                    coord.col = j;
                    steps[step_count++] = coord;
                }
            }
        }
    }
    return step_count;
}

int main(int argc, char *argv[]) {
    FILE *puzzle_file;

    if(argc==1) {
        fprintf(stderr,"missing argument: <puzzle.txt>\n");
        return 1;
    }
    puzzle_file = fopen(argv[1],"r");
    int max_row, max_col;
    MAP map;
    read_puzzle(puzzle_file, &map);
    fclose(puzzle_file);
    COORD steps[4];
    int max=possible_steps(&map, 2, 7, steps);
    for(int s=0; s<max; s++) {
        int i = steps[s].row;
        int j = steps[s].col;
        char c = map.squares[i][j];
        printf("%d %d (%c)\n", i, j, c);
    }
    return 0;
}

