#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "regolith.h"

TEST_GROUP(regolith);


char *cave;
TEST_SETUP(regolith) {
    cave = new_cave(500);
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(regolith) {
    destroy_cave(cave);
    UnityMalloc_EndTest();
}
TEST(regolith, scan_path) {
    cave = new_cave(500);
    scan_path(cave,"498,4 -> 498,6 -> 496,6");
    TEST_ASSERT_EQUAL_CHAR('.', cave_at(cave, 498,0));
    TEST_ASSERT_EQUAL_CHAR('#', cave_at(cave, 498,4));
    TEST_ASSERT_EQUAL_CHAR('#', cave_at(cave, 498,5));
    TEST_ASSERT_EQUAL_CHAR('#', cave_at(cave, 498,6));
    TEST_ASSERT_EQUAL_CHAR('#', cave_at(cave, 497,6));
    TEST_ASSERT_EQUAL_CHAR('#', cave_at(cave, 496,6));
}
