#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const int LINE_MAX = 10000;
const int LENGTH = 14;


int marker_after(char chars[LINE_MAX]) {
    for(int k=LENGTH+1; k<=strlen(chars); k++) {
        int all_diff = true;
        for(int i=k-LENGTH; i<k-1; i++) {
            for(int j=i+1; j<k; j++) {
                all_diff &= (chars[i] != chars[j]);
            }
        }
        if(all_diff)
            return k;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    FILE *puzzleFile;
    char line[LINE_MAX];

    if(argc == 3) {
        char *line = argv[1];
        int expected = atoi(argv[2]);
        int result = marker_after(line);
        if(result != expected) {
            fprintf(stderr, "expected: %d and got %d\n", expected, result);
            return 1;
        }
    }
    else if(argc == 2) {
        puzzleFile = fopen(argv[1], "r");
        if(puzzleFile == NULL) {
            fprintf(stderr, "can't open file %s\n", argv[1]);
            return 1;
        }
        while(fgets(line, LINE_MAX, puzzleFile)) {
            printf("%d\n", marker_after(line));
        }
        fclose(puzzleFile);
    }
    return 0;
}
