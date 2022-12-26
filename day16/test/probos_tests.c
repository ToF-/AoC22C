#include <assert.h>
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "probos.h"

TEST_GROUP(probos);

TEST_SETUP(probos) {
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(probos) {
    UnityMalloc_EndTest();
}
TEST(probos, valve_id) {
    TEST_ASSERT_EQUAL_INT(0, valve_id("AA"));
    TEST_ASSERT_EQUAL_INT(1, valve_id("AB"));
    TEST_ASSERT_EQUAL_INT(26, valve_id("BA"));
    TEST_ASSERT_EQUAL_INT(675, valve_id("ZZ"));
}
