#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "beacon.h"
#include <limits.h>

int abs(int x) {
    return x < 0 ? -x : x;
}

int md(COORD c0, COORD c1) {
    return abs(c1.x - c0.x) + abs(c1.y - c0.y);
}

int distance(SENSOR sensor) {
    return md(sensor.coord, sensor.beacon);
}
bool excluded(SENSOR sensor, int x, int y) {
    if(x == sensor.beacon.x && y == sensor.beacon.y)
        return false;
    return md(sensor.coord, (COORD) { .x = x, .y = y }) <= distance(sensor);
}
bool excluded_all(SENSOR *sensors, int count, int x, int y) {
    for(int i=0; i<count; i++) {
        if(excluded(sensors[i], x, y))
            return true;
    }
    return false;
}

int min(int a, int b) {
    return a < b ? a : b;
}

int max(int a, int b) {
    return a > b ? a : b;
}

int excluded_positions(SENSOR *sensors, int count, int row) {
    int xmin = INT_MAX;
    int xmax = INT_MIN;
    int ymin = INT_MAX;
    int ymax = INT_MIN;
    for(int i=0; i<count; i++) {
        SENSOR s = sensors[i];
        int d = distance(s);
        xmin = min(xmin, s.coord.x - d);
        ymin = min(ymin, s.coord.y - d);
        xmax = max(xmax, s.coord.x + d);
        ymax = max(ymax, s.coord.y + d);
    }
    printf("%d,%d %d,%d\n", xmin, ymin, xmax, ymax);
    int result = 0;
    for(int x = xmin; x <= xmax; x++) 
        result += excluded_all(sensors, count, x, row);
    return result;
}

SENSOR get_sensor(char *line) {
    char *buffer = strdup(line);
    for(int i=0; i<strlen(buffer); i++) {
        char c = buffer[i];
        if(!isdigit(c) && c != '-')
            buffer[i] = ' ';
    }
    SENSOR sensor;
    char *token = strtok(buffer, " ");
    sensor.coord.x = atoi(token);
    token = strtok(NULL, " ");
    sensor.coord.y = atoi(token);
    token = strtok(NULL, " ");
    sensor.beacon.x = atoi(token);
    token = strtok(NULL, " ");
    sensor.beacon.y = atoi(token);
    free(buffer);
    return sensor;
}

const int LINE_SIZE = 200;

int get_puzzle(SENSOR *sensors, char *filename) {
    FILE *puzzle_file = fopen(filename, "r");
    assert(puzzle_file);
    char line[LINE_SIZE];
    int count = 0;
    while(fgets(line, LINE_SIZE, puzzle_file)) {
        int l =strcspn(line,"\n");
        line[l]='\0';
        sensors[count++] = get_sensor(line);
    }
    fclose(puzzle_file);
    return count;
}


