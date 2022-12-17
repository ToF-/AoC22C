#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "hill_climbing.h"

const int MAX_ROW = 50;
const int MAX_COL =100;
const int CAPACITY = MAX_ROW * MAX_COL;

typedef struct { 
    int max_row;
    int max_col;
    char *squares;
} MAP;

int node_id(COORD coord) {
    assert(coord.row >= 0 && coord.row < MAX_ROW && coord.col >= 0 && coord.col < MAX_COL);
    return coord.row * MAX_COL + coord.col;
}

COORD coord(int id) {
    assert(id >= 0 && id < MAX_ROW * MAX_COL);
    return (COORD) { .row = id / MAX_COL, .col = id % MAX_COL };
}

void print_value(POINT value) {
    printf("%d (%d,%d) ", value.distance, value.coord.row, value.coord.col);
}
int compare(POINT a, POINT b) {
    return a.distance - b.distance;
}

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


char square_at(HEIGHT_MAP *map, COORD coord) {
    return map->squares[coord.row * map->max_col + coord.col];
}


int process(int argc, char *argv[]) {
    if(argc==1) {
        fprintf(stderr,"missing argument: <puzzle.txt>\n");
        return 1;
    }
    return 0;
}

HEIGHT_MAP *read_puzzle(char *filename) {
   int max_row = 0;
   int max_col = 0;
   int row = 0;
   int col = 0;
   static char lines[MAX_ROW][MAX_COL];
   FILE *puzzle_file = fopen(filename, "r");
   assert(puzzle_file);
   while(fgets(lines[row], MAX_COL, puzzle_file)) {
       int l = strcspn(lines[row], "\n");
       lines[row][l] = '\0';
       if(max_col < l)
           max_col = l;
       row++;
       max_row = row;
   }
   fclose(puzzle_file);
   HEIGHT_MAP *map = (HEIGHT_MAP *)malloc(sizeof(HEIGHT_MAP));
   map->max_row = max_row;
   map->max_col = max_col;
   map->squares = (char *)malloc(sizeof(char) * map->max_row * map->max_col);
    for(int row = 0; row < map->max_row; row++) {
        for(int col = 0; col < map->max_col; col++) {
            map->squares[row*map->max_col + col] = lines[row][col];
        }
    }
    return map;
}

void destroy_height_map(HEIGHT_MAP *map) {
    free(map->squares);
    free(map);
}

int adjacent_squares(HEIGHT_MAP *map, COORD coord, COORD *squares) {
    assert(coord.row >=0 && coord.row < map->max_row && coord.col >= 0 && coord.col < map->max_col);
    int count = 0;
    char from = square_at(map, coord);
    if(from == 'S')
        from = 'a'-1;
    COORD adjacents[4] = 
      { (COORD) { .row = coord.row-1, .col = coord.col },
        (COORD) { .row = coord.row+1, .col = coord.col },
        (COORD) { .row = coord.row,   .col = coord.col-1 },
        (COORD) { .row = coord.row,   .col = coord.col+1 }};
    for(int i=0; i<4; i++) {
        COORD adj = adjacents[i];
        if(adj.row < 0 || adj.row >= map->max_row || adj.col < 0 || adj.col >= map->max_col)
            continue;
        char to = square_at(map, adj);
        if(to - from <= 1) {
            squares[count++] = adj;
        }
    }
    return count;
}

