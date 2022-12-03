#include <stdio.h>
#include <string.h>

const int LINE_MAX = 100;
const int LETTER_MAX = 52;

int lettertoint(char letter) {
    if(letter >= 'a' && letter <= 'z')
        return letter - 'a';
    else
        return letter - 'A' + 26;
}


void initTable(int table[LETTER_MAX]) {
    for(int i=0; i<52; table[i++]=0); 
}

void setLetter(int table[LETTER_MAX], char c, int mask) {
    table[lettertoint(c)] |= mask;
}

int common(int table[LETTER_MAX], int mask) {
    int result = 0;
    for(int i=0; i<LETTER_MAX; i++) {
        if(table[i] == mask)
            result = i+1;
    }
    initTable(table);
    return result;
}
int main(int argc, char *argv[]) {
    char line[LINE_MAX];
    FILE *puzzleFile;

    puzzleFile = fopen(argv[1], "r");
    if(puzzleFile == NULL) {
        fprintf(stderr, "can't open file %s\n", argv[1]);
        return 1;
    }
    int shared[LETTER_MAX];
    int badges[LETTER_MAX];
    initTable(shared);
    initTable(badges);
    int part1sum = 0;
    int part2sum = 0;
    int lineNumber = 0;
    while(fgets(line, LINE_MAX, puzzleFile)) {
        if(lineNumber % 3 == 0) 
            part2sum+= common(badges, 1|2|4);
        line[strcspn(line, "\n")] = 0;
        int l = strlen(line);

        for(int i=0; i<l; i++) {
            setLetter(shared, line[i], i < (l/2) ? 1 : 2);
            setLetter(badges,  line[i], 1 << (lineNumber % 3));
        }
        part1sum += common(shared, 1|2);
        lineNumber++;
    }
    part2sum+= common(badges, 1|2|4);
    printf("%d %d\n",part1sum, part2sum);
    return 0;
}
