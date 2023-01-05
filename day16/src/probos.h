#include <stdbool.h>
#define MAX_IDS (26*26)

typedef struct {
    int  rate;
    bool closed;
} VALVE;

typedef struct {
    int ids[MAX_IDS];
    int max_id;
    VALVE *valves[MAX_IDS];
    int dist[MAX_IDS][MAX_IDS];
} SOLVER;

SOLVER *new_solver();
int get_index(SOLVER *, char *);
void scan_device(SOLVER *, char *);
void calc_distances(SOLVER *);
void destroy_solver(SOLVER *);
