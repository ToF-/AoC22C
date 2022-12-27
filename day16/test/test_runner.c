#include "unity_fixture.h"

TEST_GROUP_RUNNER(probos) {
    RUN_TEST_CASE(probos, valve_id);
    RUN_TEST_CASE(probos, add_valve);
    RUN_TEST_CASE(probos, max_heap_adding_and_extracting); 
    RUN_TEST_CASE(probos, adjacent_valves);
}

