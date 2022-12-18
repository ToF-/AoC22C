#include "assert.h"
#include "stdbool.h"
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "distress_signal.h"
#include "limits.h"

TEST_GROUP(distress_signal);

ELEM *elem;

TEST_SETUP(distress_signal) {
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(distress_signal) {
    UnityMalloc_EndTest();
}
TEST(distress_signal, reading_empty_packet){
    elem = packet("[]");
    TEST_ASSERT_NOT_NULL(elem);
    TEST_ASSERT_EQUAL(LIST, elem->type);
    TEST_ASSERT_NULL(elem->value.head);
    print_packet(elem);
    printf("\n");
}
TEST(distress_signal, reading_a_single_integer){
    elem = packet("[4807]");
    TEST_ASSERT_NOT_NULL(elem);
    TEST_ASSERT_EQUAL(LIST, elem->type);
    TEST_ASSERT_NOT_NULL(elem->value.head);
    TEST_ASSERT_EQUAL(INTEGER, elem->value.head->type);
    TEST_ASSERT_EQUAL_INT(4807, elem->value.head->value.number);
    print_packet(elem);
    printf("\n");
}
TEST(distress_signal, reading_several_integers) {
    elem = packet("[42,17]");
    TEST_ASSERT_EQUAL_INT(42, elem->value.head->value.number);
    TEST_ASSERT_EQUAL_INT(17, elem->next->value.head->value.number);
    print_packet(elem);
    printf("\n");

}
TEST(distress_signal, reading_nested_lists) {
    elem = packet("[23,[17,42]]");
    print_packet(elem);
    // TEST_ASSERT_EQUAL_INT(23, elem->value.head->value.number);
    // TEST_ASSERT_EQUAL_INT(17, elem->next->value.head->value.head->value.number);
    printf("\n");
}