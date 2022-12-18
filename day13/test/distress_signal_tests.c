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
    destroy_packet(list);
}
TEST(distress_signal, reading_a_single_integer_packet) {
    list = packet("[4]");
    TEST_ASSERT_NOT_NULL(list->head);
    destroy_packet(list);
}
