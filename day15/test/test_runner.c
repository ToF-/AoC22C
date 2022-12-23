#include "unity_fixture.h"

TEST_GROUP_RUNNER(beacon) {
    RUN_TEST_CASE(beacon, excluded);
    RUN_TEST_CASE(beacon, get_sensor);
}

