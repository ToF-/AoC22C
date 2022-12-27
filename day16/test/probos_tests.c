#include <assert.h>
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "probos.h"

TEST_GROUP(probos);

VALVE valves[676];

TEST_SETUP(probos) {
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(probos) {
    UnityMalloc_EndTest();
}
TEST(probos, valve_id) {
    TEST_ASSERT_EQUAL_INT(0, valve_id("AA"));
    TEST_ASSERT_EQUAL_INT(1, valve_id("AB"));
    TEST_ASSERT_EQUAL_INT(26, valve_id("BA"));
    TEST_ASSERT_EQUAL_INT(675, valve_id("ZZ"));
}
TEST(probos, add_valve) {
    scan_device(valves, "Valve AA has flow rate=0; tunnels lead to valves DD, II, BB");
    int AA = valve_id("AA");
    int DD = valve_id("DD");
    int II = valve_id("II");
    int BB = valve_id("BB");
    TEST_ASSERT_EQUAL_INT(3, valves[AA].tunnel_count);
}

int compare_ints(void *vpa, void *vpb) {
    int *a = (int *)vpa;
    int *b = (int *)vpb;
    return *a - *b;
}

void print_heap_as_ints(MAX_HEAP *heap) {
    for(int i=1; i<=heap->count; i++) {
        int *value = (int *)heap->values[i];
        printf("%d ", *value);
    }
    printf("\n");
}
TEST(probos, max_heap_adding_and_extracting) {
    int a = 42; int b = 17; int c = 4807; int d = 5; int e = 1000;
    MAX_HEAP *heap = new_max_heap(16,compare_ints);
    TEST_ASSERT_EQUAL_INT(0, heap->count);
    add(heap, &a);
    add(heap, &b);
    add(heap, &c);
    add(heap, &d);
    add(heap, &e);
    TEST_ASSERT_EQUAL_INT(5, heap->count);
    int *result;
    result = (int *)extract_max(heap);
    TEST_ASSERT_EQUAL_INT(4807, *result);
    result = (int *)extract_max(heap);
    TEST_ASSERT_EQUAL_INT(1000, *result);
    TEST_ASSERT_EQUAL_INT(3, heap->count);
    result = (int *)extract_max(heap);
    TEST_ASSERT_EQUAL_INT(42, *result);
    result = (int *)extract_max(heap);
    TEST_ASSERT_EQUAL_INT(17, *result);
    result = (int *)extract_max(heap);
    TEST_ASSERT_EQUAL_INT(5, *result);
    destroy_max_heap(heap,false);
}

