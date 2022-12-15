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
    char *squares;
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
} POINT;

void print_value(POINT value) {
    printf("%d (%d,%d) ", value.distance, value.coord.row, value.coord.col);
}
int compare(POINT a, POINT b) {
    return a.distance - b.distance;
}

typedef struct {
    POINT *values;
    int count;
    int capacity;
} HEAP;

void print_heap(HEAP *);
HEAP *new_min_heap(int capacity) {
    HEAP *heap = (HEAP *)malloc(sizeof(HEAP));
    heap->capacity = capacity;
    heap->count = 0;
    heap->values = (POINT *)malloc((heap->capacity) * sizeof(POINT));
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
    POINT temp = heap->values[i];
    heap->values[i] = heap->values[j];
    heap->values[j] = temp;
}

void insert(HEAP *heap, POINT value) {
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

POINT extract_min(HEAP *heap) {
    assert(heap->count > 0);
    POINT min_value = heap->values[0];
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
    static char line[MAX_ROW][MAX_COL];
    int row = 0;
    map->max_col = 0;
    while(fgets(line[row], MAX_COL, puzzle_file)) {
        int l = strcspn(line[row], "\n");
        line[row][l] = 0;
        row++;
        if (map->max_col < l)
            map->max_col = l;
    }
    map->max_row = row;
    map->squares = malloc(sizeof(char)*map->max_row*map->max_col);
    for(int row = 0; row < map->max_row; row++) {
        for(int col = 0; col < map->max_col; col++) {
            map->squares[row*map->max_col + col] = line[row][col];
        }
    }
}

char square_at(MAP *map, int row, int col) {
    return map->squares[row * map->max_col + col];
}

int possible_steps(MAP *map, int row, int col, COORD steps[4]) {
    assert(row >=0 && row < map->max_row);
    int step_count = 0;
    char from = square_at(map, row, col);
    COORD adjacents[4] = 
      { (COORD) { .row = row-1, .col = col },
        (COORD) { .row = row+1, .col = col },
        (COORD) { .row = row, .col = col-1 },
        (COORD) { .row = row, .col = col+1 }};
    for(int i=0; i<4; i++) {
        COORD adj = adjacents[i];
        if(adj.row < 0 || adj.row >= map->max_row || adj.col < 0 || adj.col >= map->max_col)
            continue;
        char to = square_at(map, adj.row, adj.col);
        if(to - from <= 1) {
            steps[step_count++] = adj;
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
    MAP *map =(MAP *)malloc(sizeof(MAP));
    read_puzzle(puzzle_file, map);
    for(int i = 0; i < map->max_row; i++) {
        for(int j = 0; j < map->max_col; j++) {
            char c = square_at(map, i, j);
            printf("%c",(char)c);
        }
        printf("\n");
    }
    fclose(puzzle_file);
    COORD steps[4];
    int max=possible_steps(map, 3, 7, steps);
    for(int s=0; s<max; s++) {
        int i = steps[s].row;
        int j = steps[s].col;
        char c = square_at(map, i, j);
        printf("%d %d (%c)\n", i, j, (char)c);
    }
    return 0;
}

