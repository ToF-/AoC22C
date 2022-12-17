typedef struct {
    int row;
    int col;
} COORD;

typedef struct {
    int distance;
    COORD coord;
} POINT;

typedef struct {
    POINT *values;
    int count;
    int capacity;
} HEAP;

typedef struct {
    int max_row;
    int max_col;
    char *squares;
} HEIGHT_MAP;

void destroy_heap(HEAP *heap);
int process(int argc, char *argv[]);
HEIGHT_MAP *read_puzzle(char *filename);
void destroy_height_map(HEIGHT_MAP *map);
char square_at(HEIGHT_MAP *map, COORD coord);
int adjacent_squares(HEIGHT_MAP *map, COORD coord, COORD *squares);
