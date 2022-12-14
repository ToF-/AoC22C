#include "unity_fixture.h"

TEST_GROUP_RUNNER(distress_signal) {
    RUN_TEST_CASE(distress_signal, reading_empty_packet);
    RUN_TEST_CASE(distress_signal, reading_a_single_integer_packet);
    RUN_TEST_CASE(distress_signal, reading_several_integers);
    RUN_TEST_CASE(distress_signal, reading_a_sublist);
    RUN_TEST_CASE(distress_signal, reading_a_complex_list);
    RUN_TEST_CASE(distress_signal, compare_lists_left_smaller_integer);
    RUN_TEST_CASE(distress_signal, compare_lists_mixed_types);
    RUN_TEST_CASE(distress_signal, compare_lists_out_of_item);
    RUN_TEST_CASE(distress_signal, read_puzzle);
    RUN_TEST_CASE(distress_signal, solve_part1_sample);
    RUN_TEST_CASE(distress_signal, solve_part1_puzzle);
    RUN_TEST_CASE(distress_signal, solve_part2_sample);
    RUN_TEST_CASE(distress_signal, solve_part2_puzzle);
}
