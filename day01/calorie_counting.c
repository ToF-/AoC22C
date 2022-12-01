#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int LINE_MAX = 10;
const int ROWS_MAX = 2500;

int compare(const void *a, const void *b) {
    long arg1 = *(const long *)a;
    long arg2 = *(const long *)b;
    if(arg1 > arg2) return -1;
    if(arg1 < arg2) return  1;
    return 0;
}

int main(int argc, char *argv[]) {
    char line[LINE_MAX];
    FILE *puzzleFile;
    long totals[ROWS_MAX];
    int  maxTotal = 0;
    long current_total = 0;

    puzzleFile = fopen(argv[1], "r");
    if(puzzleFile == NULL) {
        fprintf(stderr, "can't open file %s\n", argv[1]);
        return 1;
    }

    while(fgets(line, LINE_MAX, puzzleFile)) {
        char *end;
        const long current = strtol(line, &end, 10);
        if(current == 0) {
            totals[maxTotal++] = current_total;
            current_total = 0;
        }
        else {
            current_total += current;
        }
    }
    totals[maxTotal++] = current_total;

    fclose(puzzleFile);

    qsort(totals, maxTotal, sizeof(long), compare);
    printf("max totals:%ld+%ld+%ld=%ld\n", 
            totals[0],
            totals[1],
            totals[2],
            totals[0]+totals[1]+totals[2]);
    return 0;
}
