#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "hill_climbing.h"

TEST_GROUP(hill_climbing);

HEIGHT_MAP *map;

TEST_SETUP(hill_climbing) {
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
    map = read_puzzle("sample.txt");
/*
    Sabqponm
    abcryxxl
    accszExk
    acctuvwj
    abdefghi
*/

}
TEST_TEAR_DOWN(hill_climbing) {
    destroy_height_map(map);
    UnityMalloc_EndTest();
}
TEST(hill_climbing, can_read_a_puzzle){
    TEST_ASSERT_EQUAL_INT(5, map->max_row);
    TEST_ASSERT_EQUAL_INT(8, map->max_col);
    TEST_ASSERT_EQUAL_CHAR('S', square_at(map, (COORD) { .row = 0, .col = 0}));
    TEST_ASSERT_EQUAL_CHAR('m', square_at(map, (COORD) { .row = 0, .col = 7}));
    TEST_ASSERT_EQUAL_CHAR('i', square_at(map, (COORD) { .row = 4, .col = 7}));
    TEST_ASSERT_EQUAL_CHAR('a', square_at(map, (COORD) { .row = 4, .col = 0}));
}
TEST(hill_climbing, finding_adjacent_squares) {
    static COORD squares[4];
    int adj_max = adjacent_squares(map, (COORD) { .row = 0, .col = 0}, squares);
    TEST_ASSERT_EQUAL_INT(2, adj_max);
    TEST_ASSERT_EQUAL(1, squares[0].row);
    TEST_ASSERT_EQUAL(0, squares[0].col);
    TEST_ASSERT_EQUAL(0, squares[1].row);
    TEST_ASSERT_EQUAL(1, squares[1].col);

    adj_max = adjacent_squares(map, (COORD) { .row = 1, .col = 4}, squares);
    TEST_ASSERT_EQUAL_INT(4, adj_max);
}

int compare_ints(void *vpa, void *vpb) {
    int *a = (int *)vpa;
    int *b = (int *)vpb;
    return *a - *b;
}

void print_heap_as_ints(MIN_HEAP *heap) {
    for(int i=1; i<=heap->count; i++) {
        int *value = (int *)heap->values[i];
        printf("%d ", *value);
    }
    printf("\n");
}
TEST(hill_climbing, min_heap_adding_and_extracting) {
    int a = 42; int b = 17; int c = 4807; int d = 5; int e = 1000;
    MIN_HEAP *heap = new_min_heap(16,compare_ints);
    TEST_ASSERT_EQUAL_INT(0, heap->count);
    add(heap, &a);
    add(heap, &b);
    add(heap, &c);
    add(heap, &d);
    add(heap, &e);
    TEST_ASSERT_EQUAL_INT(5, heap->count);
    int *result;
    result = (int *)extract_min(heap);
    TEST_ASSERT_EQUAL_INT(5, *result);
    TEST_ASSERT_EQUAL_INT(4, heap->count);
    result = (int *)extract_min(heap);
    TEST_ASSERT_EQUAL_INT(17, *result);
    result = (int *)extract_min(heap);
    TEST_ASSERT_EQUAL_INT(42, *result);
    result = (int *)extract_min(heap);
    TEST_ASSERT_EQUAL_INT(1000, *result);
    result = (int *)extract_min(heap);
    destroy_min_heap(heap,false);
}

TEST(hill_climbing, finding_start_and_end_of_map) {
    COORD start = find_char(map, 'S');
    COORD end = find_char(map, 'E');
    TEST_ASSERT_EQUAL_INT(0, start.row);
    TEST_ASSERT_EQUAL_INT(0, start.col);
    TEST_ASSERT_EQUAL_INT(2, end.row);
    TEST_ASSERT_EQUAL_INT(5, end.col);
}
TEST(hill_climbing, shortest_path) {
    COORD start = { .row = 0, .col = 0 };
    COORD end = { .row = 0, .col = 1 };
    POINT *path = shortest_path(map, start, end);
    TEST_ASSERT(path != NULL);
    TEST_ASSERT_EQUAL_INT(1, path->distance);
    TEST_ASSERT_EQUAL_INT(0, path->coord.row);
    TEST_ASSERT_EQUAL_INT(1, path->coord.col);
    destroy_path(path);
}
