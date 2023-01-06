#include "unity_fixture.h"

TEST_GROUP_RUNNER(probos) {
    RUN_TEST_CASE(probos, id);
    RUN_TEST_CASE(probos, scan);
    RUN_TEST_CASE(probos, scan_file);
    RUN_TEST_CASE(probos, pressure);
    RUN_TEST_CASE(probos, max_pressure);
}

