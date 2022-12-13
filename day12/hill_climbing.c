#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>

const int MAX_ROW = 50;
const int MAX_COL =100;
const int CAPACITY = MAX_ROW * MAX_COL;

typedef struct { 
    int max_row;
    int max_col;
    char squares[MAX_ROW][MAX_COL];
} MAP;

typedef struct {
    int row;
    int col;
} COORD;

int node_id(COORD coord) {
    assert(coord.row >= 0 && coord.row < MAX_ROW && coord.col >= 0 && coord.col < MAX_COL);
    return coord.row * MAX_COL + coord.col;
}

COORD coord(int id) {
    assert(id >= 0 && id < MAX_ROW * MAX_COL);
    return (COORD) { .row = id / MAX_COL, .col = id % MAX_COL };
}

typedef struct {
    int distance;
    COORD coord;
} VALUE;

void print_value(VALUE value) {
    printf("%d (%d,%d) ", value.distance, value.coord.row, value.coord.col);
}
int compare(VALUE a, VALUE b) {
    return a.distance - b.distance;
}

typedef struct {
    VALUE *values;
    int count;
    int capacity;
} HEAP;

void print_heap(HEAP *);
HEAP *new_min_heap(int capacity) {
    HEAP *heap = (HEAP *)malloc(sizeof(HEAP));
    heap->capacity = capacity;
    heap->count = 0;
    heap->values = (VALUE *)malloc((heap->capacity) * sizeof(VALUE));
    return heap;
}

int parent(int i) {
    return ((i+1)/2)-1;
}

int left_child(int i) {
    return ((i+1)*2)-1;
}

int right_child(int i) {
    return ((i+1)*2);
}

int is_lower(HEAP *heap, int i, int j) {
    return compare(heap->values[i], heap->values[j]) < 0;
}
int is_upper(HEAP *heap, int i, int j) {
    return compare(heap->values[i], heap->values[j]) >= 0;
}

void swap_values(HEAP *heap, int i, int j) {
    VALUE temp = heap->values[i];
    heap->values[i] = heap->values[j];
    heap->values[j] = temp;
}

void insert(HEAP *heap, VALUE value) {
    assert(heap->count < heap->capacity);
    heap->values[heap->count] = value;
    heap->count++;

    if(heap->count>1) {
        for(int i = heap->count-1; i>0; i--) {
            if(is_upper(heap, parent(i), i)) {
                swap_values(heap, parent(i), i);
            }
        }
    }
}

void heapify(HEAP *heap, int i) {
    int left = left_child(i);
    int right = right_child(i);
    int lowest = i;
    if(left < heap->count && is_lower(heap, left, lowest)) {
        lowest = left;
    }
    if(right < heap->count && is_lower(heap, right, lowest)) {
        lowest = right;
    }
    if(lowest != i) {
        swap_values(heap, i, lowest);
        heapify(heap, lowest);
    }
}

void build_min_heap(HEAP *heap) {
    for(int i = heap->count / 2; i >= 0; i--) {
        heapify(heap, i);
    }
}

VALUE extract_min(HEAP *heap) {
    assert(heap->count > 0);
    VALUE min_value = heap->values[0];
    heap->values[0] = heap->values[heap->count-1];
    heap->count--;
    heapify(heap, 0);
    return min_value;
}

void print_heap(HEAP *heap) {
    for(int i=0; i<heap->count; i++) {
        printf("%d ", heap->values[i].distance);
    }
    printf("\n");
}
void destroy_heap(HEAP *heap) {
    free(heap->values);
    free(heap);
}

void read_puzzle(FILE *puzzle_file, MAP *map) {
    char line[MAX_COL];
    int col;
    int row = 0;
    while(fgets(line, MAX_COL, puzzle_file)) {
        line[strcspn(line, "\n")] = 0;
        for(col = 0; col < strlen(line); col++) {
            map->squares[row][col] = line[col];
        }
        row++;
    }
    map->max_row = row;
    map->max_col = col;
}

int possible_steps(MAP *map, int row, int col, COORD steps[4]) {
    int step_count = 0;
    char from = map->squares[row][col];
    for(int i = row-1; i<=row+1; i++) {
        for(int j = col-1; j<=col+1; j++) {
            if(i >= 0 && i < map->max_row && j >= 0 && j < map->max_col) {
                int dist = (row-i)*(row-i) + (col-j)*(col-j);
                if((i == row && j == col) || dist != 1)
                    continue;
                char to = map->squares[i][j];
                int diff = to-from;
                if(diff <= 1) {
                    COORD coord;
                    coord.row = i;
                    coord.col = j;
                    steps[step_count++] = coord;
                }
            }
        }
    }
    return step_count;
}

int main(int argc, char *argv[]) {
    FILE *puzzle_file;

    if(argc==1) {
        fprintf(stderr,"missing argument: <puzzle.txt>\n");
        return 1;
    }
    puzzle_file = fopen(argv[1],"r");
    int max_row, max_col;
    MAP map;
    read_puzzle(puzzle_file, &map);
    fclose(puzzle_file);
    COORD steps[4];
    int max=possible_steps(&map, 2, 7, steps);
    for(int s=0; s<max; s++) {
        int i = steps[s].row;
        int j = steps[s].col;
        char c = map.squares[i][j];
        printf("%d %d (%c)\n", i, j, c);
    }
    HEAP *heap = new_min_heap(CAPACITY);
    VALUE vs[6] = { 
    (VALUE){ .distance = 42, .coord = (COORD){ .row = 3, .col = 5 }},
    (VALUE){ .distance = 17, .coord = (COORD){ .row = 3, .col = 5 }},
    (VALUE){ .distance = 4807, .coord = (COORD){ .row = 3, .col = 5 }},
    (VALUE){ .distance = 21, .coord = (COORD){ .row = 3, .col = 5 }},
    (VALUE){ .distance = 22000, .coord = (COORD){ .row = 3, .col = 5 }},
    (VALUE){ .distance = 1, .coord = (COORD){ .row = 3, .col = 5 }},
    };
    for(int i=0; i<6; i++) 
        insert(heap, vs[i]);
    print_heap(heap);
    while(heap->count > 0) {
        VALUE v = extract_min(heap);
        printf("%d ", v.distance);
    }
    return 0;
}

