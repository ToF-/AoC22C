#include <stdbool.h>
#define MAX_IDS (26*26)

typedef struct {
    char *ids[MAX_IDS];
    int  max_id;
} SOLVER;

SOLVER *new_solver();
int id(SOLVER *, char *tag);
void destroy_solver(SOLVER *);
