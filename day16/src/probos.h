#include <stdbool.h>

typedef int ID;
typedef struct {
    bool closed;
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

ID valve_id(char *);
void scan_device(VALVE *, char *);
MAX_HEAP *new_max_heap(int capacity, int (* compare)(void *,void *));
void add(MAX_HEAP *heap, void *value);
void destroy_max_heap(MAX_HEAP *heap, bool destroy_values);
void *extract_max(MAX_HEAP *heap);
