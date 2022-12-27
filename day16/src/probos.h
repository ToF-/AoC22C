#include <stdbool.h>

typedef int ID;
typedef struct {
    ID id;
    bool closed;
    ID predecessor;
    int flow_rate;
    int tunnel_count;
    ID tunnels[5];
} VALVE;

typedef struct {
    void **values;
    int (* compare)(void *, void *);
    int count;
    int capacity;
} MAX_HEAP;

typedef struct {
    VALVE *valves;
    MAX_HEAP *max_heap;
}SOLVER;

ID valve_id(char *);
void scan_device(VALVE *, char *);
MAX_HEAP *new_max_heap(int capacity, int (* compare)(void *,void *));
void add(MAX_HEAP *heap, void *value);
void destroy_max_heap(MAX_HEAP *heap, bool destroy_values);
void *extract_max(MAX_HEAP *heap);
int compare_valves(void *, void *);
SOLVER *new_solver(VALVE *);
void find_adjacent_valves(SOLVER *);
void destroy_solver(SOLVER *);
void print_valve(VALVE *);
