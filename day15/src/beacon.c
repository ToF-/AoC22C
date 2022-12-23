#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "beacon.h"

int abs(int x) {
    return x < 0 ? -x : x;
}

int md(int x0, int y0, int x1, int y1) {
    return abs(x1-x0)+abs(y1-y0);
}

bool excluded(int sx, int sy, int bx, int by, int x, int y) {
    return md(sx,sy,x,y) <= md(sx,sy,bx,by);
}

void get_sensor(char *line, int *sx, int *sy, int *bx, int *by) {
    char *buffer = strdup(line);
    for(int i=0; i<strlen(buffer); i++) {
        char c = buffer[i];
        if(!isdigit(c) && c != '-')
            buffer[i] = ' ';
    }
    printf("%s\n", buffer);
    char *token = strtok(buffer, " ");
    *sx = atoi(token);
    token = strtok(NULL, " ");
    *sy = atoi(token);
    token = strtok(NULL, " ");
    *bx = atoi(token);
    token = strtok(NULL, " ");
    *by = atoi(token);
    free(buffer);
}
