#include <stdbool.h>
typedef struct {
    int row;
    int col;
} COORD;

typedef struct square {
    int distance;
    char height;
    COORD coord;
    struct square *predecessor;
} SQUARE;

typedef struct {
    void **values;
    int (* compare)(void *, void *);
    int count;
    int capacity;
} MIN_HEAP;

typedef struct {
    int max_row;
    int max_col;
    SQUARE *squares;
} HEIGHT_MAP;

int process(int argc, char *argv[]);
HEIGHT_MAP *read_puzzle(char *filename);
void destroy_height_map(HEIGHT_MAP *map);
SQUARE *square_at(HEIGHT_MAP *map, COORD coord);
int adjacent_squares(HEIGHT_MAP *map, COORD coord, SQUARE **squares);
MIN_HEAP *new_min_heap(int capacity, int (* compare)(void *,void *));
void add(MIN_HEAP *heap, void *value);
void destroy_min_heap(MIN_HEAP *heap, bool destroy_values);
void *extract_min(MIN_HEAP *heap);
COORD find_char(HEIGHT_MAP *map, char c);
void set_paths(HEIGHT_MAP *map, COORD start);
