#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

const int LENGTH = 99;

int main(int argc, char *argv[]) {

    FILE *puzzle_file;
    char line[LENGTH+1];
    int forest[LENGTH][LENGTH];
    bool visible[LENGTH][LENGTH];
    int  scenic[LENGTH][LENGTH];
    if(argc==1) {
        fprintf(stderr,"missing argument: <puzzle.txt>\n");
        return 1;
    }
    puzzle_file = fopen(argv[1], "r");
    int max_row = 0;
    int max_col = 0;
    while(fgets(line, LINE_MAX, puzzle_file)) {
        line[strcspn(line, "\n")] = 0;
        int l = strlen(line);
        for(int i=0; i<l; i++)
            forest[max_row][i] = line[i]-'0';
        max_row++;
        max_col = l > max_col ? l : max_col;
    }
    fclose(puzzle_file);
    for(int row=0; row<LENGTH; row++)
        for(int col=0; col<LENGTH; col++) {
            visible[row][col] = false;
            scenic[row][col] = 0;
        }
    for(int row=0; row<max_row; row++)
        for(int col=0; col<max_col; col++)
            visible[row][col] = (row == 0) || (row == max_row-1) || (col == 0) || (col == max_col-1);
    int max;
    for(int row = 1; row < max_row-1; row++) {
        max = forest[row][0];
        for(int col = 1; col < max_col-1; col++) {
            if(forest[row][col] > max) {
                visible[row][col] = true;
                max = forest[row][col];
            }
        }
        max = forest[row][max_col-1];
        for(int col = max_col-2; col > 0; col--) {
            if(forest[row][col] > max) {
                visible[row][col] = true;
                max = forest[row][col];
            }
        }
    }
    for(int col = 1; col < max_col-1; col++) {
        max = forest[0][col];
        for(int row = 1; row < max_row-1; row++) {
            if(forest[row][col] > max) {
                visible[row][col] = true;
                max = forest[row][col];
            }
        }
        max = forest[max_row-1][col];
        for(int row = max_row-2; row > 0; row--) {
            if(forest[row][col] > max) {
                visible[row][col] = true;
                max = forest[row][col];
            }
        }
    }
    int visibles = 0;
    for(int row=0; row<LENGTH; row++)
        for(int col=0; col<LENGTH; col++)
            if(visible[row][col]) 
                visibles++;
    printf("%d\n", visibles);
    for(int row=0; row<max_row; row++) {
        for(int col=0; col<max_col; col++) {
            int tree;
            int start;
            int up = 0; 
            int down = 0;
            int left = 0;
            int right = 0;
            if(row == 0 || col == 0 || row == max_row-1 || col == max_col-1) 
                scenic[row][col] = 0;
            else {
                start = forest[row][col];
                printf("start: (%d,%d) = %d\n", row, col, start);
                for(int r = row-1; r>=0; r--) {
                    tree = forest[r][col];
                    printf("\tup: (%d,%d) = %d\n", r, col, tree);
                    up++;
                    if(tree >= start)
                        break;
                }
                for(int r = row+1; r<max_row; r++) {
                    tree = forest[r][col];
                    printf("\tdown: (%d,%d) = %d\n", r, col, tree);
                    down++;
                    if(tree >= start)
                        break;
                }
                for(int c = col-1; c>=0; c--) {
                    tree = forest[row][c];
                    printf("\tleft: (%d,%d) = %d\n", row, c, tree);
                    left++;
                    if(tree >= start)
                        break;
                }
                for(int c = col+1; c<max_row; c++) {
                    tree = forest[row][c];
                    printf("\tright: (%d,%d) = %d\n", row, c, tree);
                    right++;
                    if(tree >= start)
                        break;
                }
                printf("\n(%d,%d): %d %d %d %d\n", row, col, up, down, left, right);
            }
            scenic[row][col] = up * down * left * right;
        }
    }
    int max_scenic = 0;
    for(int row=0; row<max_row; row++)
        for(int col=0; col<max_col; col++)
            if(scenic[row][col] > max_scenic)
                max_scenic = scenic[row][col];
    printf("%d\n", max_scenic);
    return 0;
}
