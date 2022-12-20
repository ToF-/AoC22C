#include "unity_fixture.h"

TEST_GROUP_RUNNER(regolith) {
    RUN_TEST_CASE(regolith, scan_path);
    RUN_TEST_CASE(regolith, read_puzzle);
    RUN_TEST_CASE(regolith, sand_falling);
}

