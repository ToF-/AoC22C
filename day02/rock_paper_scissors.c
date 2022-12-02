#include <stdio.h>
#include <errno.h>
#include <string.h>
const int LINE_MAX = 5;
// match = your opp' choice * 10 + your choice
// score = your choice + 0:loss, 3:draw, 6:win
int part1Scorer(int match) {
    int score;
    switch(match) { 
        case 11: score = 1 + 3; break;
        case 12: score = 2 + 6; break;
        case 13: score = 3 + 0; break;
        case 21: score = 1 + 0; break;
        case 22: score = 2 + 3; break;
        case 23: score = 3 + 6; break;
        case 31: score = 1 + 6; break;
        case 32: score = 2 + 0; break;
        case 33: score = 3 + 3; break;
        default: fprintf( stderr, "error: %d \n", score);
                 return 1;
    }
    return score;
}
// match = your opp' choice * 10 + your wanted result (0 : loss, 1: draw, 2: win)
// complete the score with required choice in order to obtain result
// score = required choice + 0:loss, 3:draw, 6:win
int part2Scorer(int match) {
    int score;
    switch(match) { 
        case 11: score = 3 + 0; break;
        case 12: score = 1 + 3; break;
        case 13: score = 2 + 6; break;
        case 21: score = 1 + 0; break;
        case 22: score = 2 + 3; break;
        case 23: score = 3 + 6; break;
        case 31: score = 2 + 0; break;
        case 32: score = 3 + 3; break;
        case 33: score = 1 + 6; break;
        default: fprintf( stderr, "error: %d \n", score);
                 return 1;
    }
    return score;
}
int main(int argc, char *argv[]) {
    char line[LINE_MAX];
    FILE *puzzleFile;

    puzzleFile = fopen(argv[1], "r");
    if(puzzleFile == NULL) {
        fprintf(stderr, "can't open file %s\n", argv[1]);
        return 1;
    }
    char opp, you;
    int total = 0;
    int soughtTotal = 0;
    while(fgets(line, LINE_MAX, puzzleFile)) {
        if ( ferror( puzzleFile ) ) {
            fprintf( stderr, "Reading error with code %d\n", errno );
            break;
        }
        line[strcspn(line, "\n")] = 0;
        sscanf(line,"%c %c", &opp, &you);
        int match = 0;
        printf("%s ", line);
        switch(opp) {
            case 'A': match = 10; break;
            case 'B': match = 20; break;
            case 'C': match = 30; break;
            default: fprintf( stderr, "error: %c \n", opp);
                     return 1;
        }
        switch(you) {
            case 'X': match += 1; break;
            case 'Y': match += 2; break;
            case 'Z': match += 3; break;
            default: fprintf( stderr, "error: %c \n", you);
                     return 1;
        }
        int score = part1Scorer(match);
        int soughtScore = part2Scorer(match);
        total += score;
        soughtTotal += soughtScore;
        printf("%d, %d, %d, %d, %d\n", match, score, total, soughtScore, soughtTotal);
    }
    return 0;
}
