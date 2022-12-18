#include "unity_fixture.h"

TEST_GROUP_RUNNER(distress_signal) {
    RUN_TEST_CASE(distress_signal, reading_empty_packet);
    RUN_TEST_CASE(distress_signal, reading_a_single_integer_packet);
}
