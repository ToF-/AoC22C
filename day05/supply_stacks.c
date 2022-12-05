#include <stdio.h>
#include <string.h>
#include <stdbool.h>

const int MAX_STACKS = 9;
const int STACK_SIZE = 100;
const int LINE_MAX = 50;

void append(char stack[STACK_SIZE], char c) {
    int l = strlen(stack);
    stack[l] = c;
    stack[l+1] = '\0';
}

int pop(char stack[STACK_SIZE]) {
    int result = stack[0];
    int l = strlen(stack);
    for(int i=1; i<l; i++) {
        stack[i-1] = stack[i];
    }
    stack[l] = '\0';
    return result;
}

void push(char stack[STACK_SIZE], char c) {
    int l = strlen(stack);
    for(int i=l; i>0; i--) {
        stack[i] = stack[i-1];
    }
    stack[l+1] = '\0';
    stack[0] = c;
}

int main(int argc, char *argv[]) {
    FILE *puzzleFile;
    char line[LINE_MAX];

    char stacks[MAX_STACKS][STACK_SIZE] = { "","","","","","","","","" };

    puzzleFile = fopen(argv[1], "r");
    if(puzzleFile == NULL) {
        fprintf(stderr, "can't open file %s\n", argv[1]);
        return 1;
    }
    int initialized = false;
    while(fgets(line, LINE_MAX, puzzleFile)) {
        line[strcspn(line, "\n")] = 0;
        if(strlen(line) == 0)
            continue;
        if(!strcmp(" 1   2   3   4   5   6   7   8   9 ", line)) {
            initialized = true;
            continue;
        }
        if(!initialized) {
            for(int i=1; i<strlen(line); i+=4) {
                char c = line[i];
                int  s = (i-1)/4;
                if(c != ' ') {
                    append(stacks[s], c);
                }
            }
        }
        else {
            int n, srce, dest;
            if(sscanf(line, "move %d from %d to %d", &n, &srce, &dest) != 3) {
                fprintf(stderr,"sorry can't parse %s\n", line);
            }
            printf("%s %s\n", stacks[srce], stacks[dest]);
            for(int i=0; i<n; i++) {
                int v = pop(stacks[srce]);
                push(stacks[dest],v);
            }
            printf("%s %s\n", stacks[srce], stacks[dest]);
            getchar();
        }
    }
    fclose(puzzleFile);
    return 0;
}

