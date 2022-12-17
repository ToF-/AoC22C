#include "unity_fixture.h"

TEST_GROUP_RUNNER(hill_climbing) {
    RUN_TEST_CASE(hill_climbing, can_read_a_puzzle);
    RUN_TEST_CASE(hill_climbing, finding_adjacent_squares);
    RUN_TEST_CASE(hill_climbing, min_heap_adding_and_extracting);
    RUN_TEST_CASE(hill_climbing, finding_start_and_end_of_map);
    RUN_TEST_CASE(hill_climbing, set_paths);
    RUN_TEST_CASE(hill_climbing, puzzle_part_1);
    RUN_TEST_CASE(hill_climbing, distance_from_best_start);
    RUN_TEST_CASE(hill_climbing, puzzle_part_2);
}
