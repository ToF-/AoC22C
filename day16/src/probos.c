#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "probos.h"

const int CAPACITY = 26 * 26 ;

int valve_id(char *name) {
    char t = name[0];
    char u = name[1];
    return (t-'A') * 26 + (u-'A');
}

bool is_valve_id(char *string) {
    return isupper(string[0]) && isupper(string[1]);
}

void scan_device(VALVE *valves, char *line) {
    char *buffer = strdup(line);
    buffer[0] = 'v';
    for(int i=0; i<strlen(buffer); i++) {
        if(!isdigit(buffer[i]) && !isupper(buffer[i]))
            buffer[i] = ' ';
    }
    char *token = strtok(buffer, " ");
    ID id = valve_id(token);
    token = strtok(NULL, " ");
    valves[id].id = id;
    valves[id].predecessor = -1;
    valves[id].closed = true;
    valves[id].flow_rate = atoi(token);
    valves[id].tunnel_count = 0;
    for(int i = 0; i < 5; i++)
        valves[id].tunnels[i] = -1;
    while((token = strtok(NULL, " "))) {
        valves[id].tunnels[valves[id].tunnel_count++] = valve_id(token);
    }
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

int is_higher(MAX_HEAP *heap, int i, int j) {
    return heap->compare(heap->values[i], heap->values[j]) > 0;
}

void swap(MAX_HEAP *heap, int i, int j) {
    void *temp = heap->values[i];
    heap->values[i] = heap->values[j];
    heap->values[j] = temp;
}

void heapify(MAX_HEAP *heap, int i) {
    int left = left_child(i);
    int right = right_child(i);
    int highest = i;
    if(left <= heap->count && is_higher(heap, left, highest)) {
        highest = left;
    }
    if(right <= heap->count && is_higher(heap, right, highest)) {
        highest = right;
    }
    if(highest != i) {
        swap(heap, i, highest);
        heapify(heap, highest);
    }
}

void build_max_heap(MAX_HEAP *heap) {
    for(int i = heap->count / 2; i >= 0; i--) {
        heapify(heap, i);
    }
}

void push_down(MAX_HEAP *heap, int i) {
    if(i == 1) {
        return;
    }
    int parent = i / 2;
    if(is_higher(heap, i, parent)) {
        swap(heap, i, parent);
        push_down(heap, parent);
    }
}
void add(MAX_HEAP *heap, void *value) {
    assert(heap->count < heap->capacity);
    heap->count++;
    heap->values[heap->count] = value;
    push_down(heap, heap->count);
}
void *extract_max(MAX_HEAP *heap) {
    assert(heap->count > 0);
    void *min_value = heap->values[1];
    swap(heap, 1, heap->count);
    heap->count--;
    heapify(heap, 1);
    return min_value;
}

int compare_valves(void *pa, void *pb) {
    VALVE *va = (VALVE *)pa;
    VALVE *vb = (VALVE *)pb;
    return va->flow_rate - vb->flow_rate;
}

MAX_HEAP *new_max_heap(int capacity, int (*compare)(void *, void *)) {
    MAX_HEAP *heap = (MAX_HEAP *)malloc(sizeof(MAX_HEAP));
    heap->compare = compare;
    heap->capacity = capacity;
    heap->values = (void **)malloc(sizeof(void *) * (heap->capacity+1));
    heap->count = 0;
    return heap;
}
void destroy_max_heap(MAX_HEAP *heap, bool destroy_values) {
    if(destroy_values) {
        for(int i=0; i < heap->count; i++)
            free(heap->values[i]);
    }
    free(heap->values);
    free(heap);
}

SOLVER *new_solver(VALVE *valves) {
    SOLVER *solver = (SOLVER *)malloc(sizeof(SOLVER));
    solver->valves = valves;
    solver->max_heap = new_max_heap(CAPACITY, compare_valves);
    return solver;
}

void print_valve(VALVE *valve) {
    assert(valve);
    printf("valve %c%c ", (valve->id/26)+'A', (valve->id%26)+'A');
    printf(" flow rate:%d ",valve->flow_rate);
    if(valve->predecessor != -1)
        printf(" predecessor: %c%c ", (valve->predecessor/26)+'A', (valve->predecessor%26)+'A');
    else
        printf(" no predecessor    ");
    printf("tunnels to: ");
    for(int i = 0; i<valve->tunnel_count; i++) {
        printf("%c%c ", (valve->tunnels[i]/26)+'A', (valve->tunnels[i]%26)+'A');
    }
    printf("\n");
}

void find_adjacent_valves(SOLVER *solver) {
    assert(solver->max_heap->count);
    VALVE *valve=extract_max(solver->max_heap);
    assert(valve);
    print_valve(valve);
    valve->closed = false;
    for(int i = 0; i < valve->tunnel_count; i++) {
        int id = valve->tunnels[i];
        VALVE *adjacent = &solver->valves[id];
        if(adjacent->closed) {
            adjacent->flow_rate += valve->flow_rate;
            add(solver->max_heap, adjacent);
        }
    }
}

void destroy_solver(SOLVER *solver) {
    destroy_max_heap(solver->max_heap, false);
    free(solver);
}


