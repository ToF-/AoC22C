#include <stdio.h>

const int LINE_MAX = 20;

int within(int x, int a, int b) {
    return (x >= a && x <= b);
}
int main(int argc, char *argv[]) {
    char line[LINE_MAX];
    FILE *puzzleFile;

    puzzleFile = fopen(argv[1], "r");
    if(puzzleFile == NULL) {
        fprintf(stderr, "can't open file %s\n", argv[1]);
        return 1;
    }
    int fullyContained = 0;
    int overalapping = 0;
    while(fgets(line, LINE_MAX, puzzleFile)) {
        int a0, b0, a1, b1;
        sscanf(line, "%d-%d,%d-%d", &a0, &b0, &a1, &b1);
        if( within(a0, a1, b1) 
                || within(a1, a0, b0)
                || within(b0, a1, b1)
                || within(b1, a0, b0))
            overalapping++;
        if(( within(a0, a1, b1)
                    && within(b0, a1, b1))
                || (within(a1, a0, b0)
                    && within(b1, a0, b0)))
            fullyContained++;
    }
    fclose(puzzleFile);
    printf("%d %d\n", fullyContained, overalapping);
    return 0;
}
