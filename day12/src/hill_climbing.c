#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stdbool.h>
#include "hill_climbing.h"

const int MAX_ROW = 50;
const int MAX_COL =100;
const int CAPACITY = MAX_ROW * MAX_COL;

int compare(POINT a, POINT b) {
    return a.distance - b.distance;
}

MIN_HEAP *new_min_heap(int capacity, int (*compare)(void *, void *)) {
    MIN_HEAP *heap = (MIN_HEAP *)malloc(sizeof(MIN_HEAP));
    heap->compare = compare;
    heap->capacity = capacity;
    heap->values = (void **)malloc(sizeof(void *) * (heap->capacity+1));
    heap->count = 0;
    return heap;
}
int parent(int i) {
    return i / 2;
}

int left_child(int i) {
    return i * 2;
}

int right_child(int i) {
    return i * 2 + 1;
}

int is_lower(MIN_HEAP *heap, int i, int j) {
    return heap->compare(heap->values[i], heap->values[j]) < 0;
}
int is_upper(MIN_HEAP *heap, int i, int j) {
    return heap->compare(heap->values[i], heap->values[j]) >= 0;
}

void swap(MIN_HEAP *heap, int i, int j) {
    void *temp = heap->values[i];
    heap->values[i] = heap->values[j];
    heap->values[j] = temp;
}

void heapify(MIN_HEAP *heap, int i) {
    int left = left_child(i);
    int right = right_child(i);
    int lowest = i;
    if(left <= heap->count && is_lower(heap, left, lowest)) {
        lowest = left;
    }
    if(right <= heap->count && is_lower(heap, right, lowest)) {
        lowest = right;
    }
    if(lowest != i) {
        swap(heap, i, lowest);
        heapify(heap, lowest);
    }
}

void build_min_heap(MIN_HEAP *heap) {
    for(int i = heap->count / 2; i >= 0; i--) {
        heapify(heap, i);
    }
}

void push_down(MIN_HEAP *heap, int i) {
    if(i == 1) {
        return;
    }
    int parent = i / 2;
    if(is_lower(heap, i, parent)) {
        swap(heap, i, parent);
        push_down(heap, parent);
    }
}
void add(MIN_HEAP *heap, void *value) {
    assert(heap->count < heap->capacity);
    heap->count++;
    heap->values[heap->count] = value;
    push_down(heap, heap->count);
}
void *extract_min(MIN_HEAP *heap) {
    assert(heap->count > 0);
    void *min_value = heap->values[1];
    swap(heap, 1, heap->count);
    heap->count--;
    heapify(heap, 1);
    return min_value;
}

void print_heap(MIN_HEAP *heap) {
    for(int i=0; i<heap->count; i++) {
        printf("%d ", ((POINT *)heap->values[i])->distance);
    }
    printf("\n");
}
void destroy_min_heap(MIN_HEAP *heap, bool destroy_values) {
    if(destroy_values) {
        for(int i=0; i < heap->count; i++)
            free(heap->values[i]);
    }
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
   char lines[MAX_ROW][MAX_COL];
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

COORD find_char(HEIGHT_MAP *map, char c) {
    for(int row=0; row < map->max_row; row++) {
        for(int col=0; col < map->max_col; col++) {
            COORD coord = { .row = row, .col = col };
            if(square_at(map,coord) == c)
                return coord;
        }
    }
    assert(false);
}

POINT *shortest_path(HEIGHT_MAP *map, COORD start, COORD end) {
    POINT *path = (POINT *)malloc(sizeof(POINT));
    path->coord = start;
    path->distance = 0;
    path->predecessor = NULL;
    MIN_HEAP *heap = new_min_heap(map->max_row * map->max_col);


    return path;
}

void destroy_path(POINT *path) {
    if(path == NULL)
        return;
    POINT *predecessor = path->predecessor;
    free(path);
    destroy_path(predecessor);
}
