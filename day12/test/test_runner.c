#include "unity_fixture.h"

TEST_GROUP_RUNNER(hill_climbing) {
    RUN_TEST_CASE(hill_climbing, can_read_a_puzzle);
    RUN_TEST_CASE(hill_climbing, finding_adjacent_squares);
}
