#include "assert.h"
#include "stdbool.h"
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "distress_signal.h"
#include "limits.h"

TEST_GROUP(distress_signal);

LIST *list;
LIST *left;
LIST *right;

TEST_SETUP(distress_signal) {
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(distress_signal) {
    UnityMalloc_EndTest();
}
TEST(distress_signal, reading_empty_packet){
    list = packet("[]");
    TEST_ASSERT_NOT_NULL(list);
    print_packet(list);
    destroy_packet(list);
}
TEST(distress_signal, reading_a_single_integer_packet) {
    list = packet("[4]");
    TEST_ASSERT_NOT_NULL(list->head);
    TEST_ASSERT_EQUAL_INT(4,list->head->AS.integer);
    print_packet(list);
    destroy_packet(list);

    list = packet("[42]");
    TEST_ASSERT_NOT_NULL(list->head);
    TEST_ASSERT_EQUAL_INT(42,list->head->AS.integer);
    print_packet(list);
    destroy_packet(list);
}
TEST(distress_signal, reading_several_integers) {
    list = packet("[17,23]");
    TEST_ASSERT_EQUAL_INT(17, list->head->AS.integer);
    TEST_ASSERT_EQUAL_INT(23, list->tail->head->AS.integer);
    print_packet(list);
    destroy_packet(list);
}
TEST(distress_signal, reading_a_sublist) {
    list = packet("[[4807]]");
    TEST_ASSERT_EQUAL_INT(4807, list->head->AS.list->head->AS.integer);
    print_packet(list);
    destroy_packet(list);
}
TEST(distress_signal, reading_a_complex_list) {
    list = packet("[17,[23,81],42]");
    TEST_ASSERT_EQUAL_INT(17, list->head->AS.integer);
    TEST_ASSERT_EQUAL_INT(23, list->tail->head->AS.list->head->AS.integer);
    TEST_ASSERT_EQUAL_INT(81, list->tail->head->AS.list->tail->head->AS.integer);
    TEST_ASSERT_NOT_NULL(     list->tail->tail);
    TEST_ASSERT_EQUAL_INT(42, list->tail->tail->head->AS.integer);
    print_packet(list);
    destroy_packet(list);
}
TEST(distress_signal, compare_lists_left_smaller_integer) {
    left = packet("[1,1,3,1,1]"); right=packet("[1,1,5,1,1]");
    TEST_ASSERT_EQUAL(-2, compare_lists(left, right));
    TEST_ASSERT_EQUAL(2, compare_lists(right, left));
    destroy_packet(left); destroy_packet(right);
}

TEST(distress_signal, compare_lists_mixed_types) {
    left = packet("[[1],[2,3,4]]"); right=packet("[[1],4]");
    TEST_ASSERT_EQUAL(-2, compare_lists(left, right));
    print_packet(right);
    destroy_packet(left); destroy_packet(right);

    left = packet("[9]"); right=packet("[[8,7,6]]");
    TEST_ASSERT_EQUAL(1, compare_lists(left, right));
    destroy_packet(left); destroy_packet(right);
}
TEST(distress_signal, compare_lists_out_of_item) {
    left = packet("[[4,4],4,4]"); right=packet("[[4,4],4,4,4]");
    TEST_ASSERT_EQUAL(-1, compare_lists(left, right));
    destroy_packet(left); destroy_packet(right);

    left = packet("[7,7,7,7]"); right=packet("[7,7,7]");
    TEST_ASSERT_EQUAL(1, compare_lists(left, right));
    destroy_packet(left); destroy_packet(right);

    left = packet("[]"); right=packet("[3]");
    TEST_ASSERT_EQUAL(-1, compare_lists(left, right));
    destroy_packet(left); destroy_packet(right);

    left = packet("[[[]]]"); right=packet("[[]]");
    print_packet(left);
    print_packet(right);

    TEST_ASSERT_EQUAL(1, compare_lists(left, right));
    destroy_packet(left); destroy_packet(right);

    left = packet("[1,[2,[3,[4,[5,6,7]]]],8,9]"); right=packet("[1,[2,[3,[4,[5,6,0]]]],8,9]");
    TEST_ASSERT_EQUAL(7, compare_lists(left, right));
    destroy_packet(left); destroy_packet(right);
}
TEST(distress_signal, read_puzzle) {
    int count;
    LIST *packets[50];
    count = read_puzzle(packets, "sample.txt");
    TEST_ASSERT_EQUAL_INT(16, count);
    left = packets[14];
    right = packets[15];
    TEST_ASSERT_EQUAL(7, compare_lists(left, right));
    left = packets[0];
    right = packets[1];
    TEST_ASSERT_EQUAL(-2, compare_lists(left, right));
    for(int i=0; i<count; i++) {
        destroy_packet(packets[i]); 
    }
}
TEST(distress_signal, solve_part1_sample) {
    int count;
    LIST *packets[50];
    count = read_puzzle(packets, "sample.txt");
    TEST_ASSERT_EQUAL(13, solve_part1(packets, count));
    for(int i=0; i<count; i++) {
        destroy_packet(packets[i]); 
    }
}
TEST(distress_signal, solve_part1_puzzle) {
    int count;
    LIST *packets[500];
    count = read_puzzle(packets, "puzzle.txt");
    TEST_ASSERT_EQUAL(5682, solve_part1(packets, count));
    for(int i=0; i<count; i++) {
        destroy_packet(packets[i]); 
    }
}
TEST(distress_signal, solve_part2_sample) {
    int count;
    LIST *packets[50];
    TEST_ASSERT_EQUAL(140, solve_part2(packets, "sample.txt"));
    for(int i=0; i<count+2; i++) {
        destroy_packet(packets[i]); 
    }
}
TEST(distress_signal, solve_part2_puzzle) {
    int count;
    LIST *packets[500];
    TEST_ASSERT_EQUAL(20304, solve_part2(packets, "puzzle.txt"));
    for(int i=0; i<count+2; i++) {
        destroy_packet(packets[i]); 
    }
}

