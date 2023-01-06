#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "probos.h"

const int MAX_DISTANCE = 10000;

SOLVER *new_solver() {
    SOLVER *solver = (SOLVER *)malloc(sizeof(SOLVER));
    solver->max_id = 0;
    solver->max_non_zero = 0;
    for(int i=0; i < MAX_IDS; i++)
        for(int j=0; j < MAX_IDS; j++)
            solver->dist[i][j] = MAX_DISTANCE;
    return solver;
}

int tagtoid(char *tag) {
    assert(strlen(tag)==2);
    int h = tag[0] - 'A';
    int l = tag[1] - 'A';
    return h * 26 + l;
}

int get_index(SOLVER *solver, char *tag) {
    int id = tagtoid(tag);
    int i;
    for(int i=0; i<solver->max_id; i++) {
        if(solver->ids[i] == id)
            return i; 
    }
    solver->ids[solver->max_id] = id;
    solver->max_id++;
    return solver->max_id-1;
}

void remove_non_upper_nor_digits(char *buffer) {
    buffer[0] = tolower(buffer[0]);
    for(int i=0; i<strlen(buffer); i++)
        if(!isdigit(buffer[i]) && !isupper(buffer[i]))
            buffer[i] = ' ';
}
const char *SEPS = " ";

void scan_device(SOLVER *solver, char *line) {
    assert(strlen(line) > 0);
    assert(solver);
    char *buffer = strdup(line);
    remove_non_upper_nor_digits(buffer);
    char *token = strtok(buffer, SEPS);
    int v = get_index(solver, token);
    solver->valves[v] = (VALVE *)malloc(sizeof(VALVE));
    strcpy(solver->valves[v]->tag, token);
    int rate = atoi(strtok(NULL, SEPS));
    solver->valves[v]->closed = true;
    solver->valves[v]->rate   = rate;
    if(rate > 0) 
        solver->non_zero[solver->max_non_zero++] = v;
    solver->dist[v][v] = 0;
    while((token = strtok(NULL, SEPS))!=NULL) {
        int w = get_index(solver, token);
        solver->dist[v][w] = 1;
        solver->dist[w][v] = 1;
    }
}

int min(int a, int b) {
    return a < b ? a : b;
}
void calc_distances(SOLVER *solver) {
    int v,w,x;
    for(int k=0; k<solver->max_id; k++)
        for(int i=0; i<solver->max_id; i++)
            for(int j=0; j<solver->max_id; j++)
                solver->dist[i][j] = min(solver->dist[i][j], solver->dist[i][k] + solver->dist[k][j]);
}

void destroy_solver(SOLVER *solver) {
    for(int i=0; i<solver->max_id; i++) {
        free(solver->valves[i]);
    }
    free(solver);
}

void scan_file(SOLVER *solver, char *filename) {
    assert(solver);
    FILE *file = fopen(filename, "r");
    assert(file);
    static char line[1000];
    while(fgets(line, 1000, file)) {
        int l = strcspn(line, "\n");
        line[l] = '\0';
        char *buffer = strdup(line);
        scan_device(solver, buffer);
        free(buffer);
    }
    fclose(file);
}
