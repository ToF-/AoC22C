#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "probos.h"

SOLVER *new_solver() {
    return (SOLVER *)malloc(sizeof(SOLVER));
}

int id(SOLVER *solver, char *tag) {
    int index;
    for(index = 0; index < solver->max_id; index++) {
        if(!strcmp(tag, solver->ids[index]))
                return index;
    }
    solver->ids[solver->max_id] = (char *)malloc(strlen(tag)+1);
    strcpy(solver->ids[solver->max_id], tag);
    return solver->max_id++;
}

void destroy_solver(SOLVER *solver) {
    for(int i=0; i<solver->max_id; i++) 
        free(solver->ids[i]);
    free(solver);
}
