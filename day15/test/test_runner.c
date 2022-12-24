#include "unity_fixture.h"

TEST_GROUP_RUNNER(beacon) {
    RUN_TEST_CASE(beacon, excluded);
    RUN_TEST_CASE(beacon, get_sensor);
    RUN_TEST_CASE(beacon, get_puzzle);
    RUN_TEST_CASE(beacon, excluded_positions);
    RUN_TEST_CASE(beacon, solve_puzzle_part1);
}

