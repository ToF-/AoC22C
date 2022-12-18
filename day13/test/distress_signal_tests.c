#include "assert.h"
#include "stdbool.h"
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "distress_signal.h"
#include "limits.h"

TEST_GROUP(distress_signal);

LIST *list;

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
