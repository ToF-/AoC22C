#include <stdbool.h>
#define MAX_IDS (26*26)
#define MAX_COMBINATION (32768)
#define MAX_TIME (32)

typedef struct {
    int  rate;
    bool closed;
    char tag[3];
} VALVE;

typedef struct {
    int ids[MAX_IDS];
    int max_id;
    VALVE *valves[MAX_IDS];
    int non_zero[MAX_IDS];
    int max_non_zero;
    int dist[MAX_IDS][MAX_IDS];
    int memo_pressure[MAX_COMBINATION][MAX_TIME];
} SOLVER;

SOLVER *new_solver();
int get_index(SOLVER *, char *);
void scan_device(SOLVER *, char *);
void calc_distances(SOLVER *);
void destroy_solver(SOLVER *);
void scan_file(SOLVER *, char *);
int next_to_open(SOLVER *, int, int, int *);
int max_pressure(SOLVER *, int, int, int, int, int);
