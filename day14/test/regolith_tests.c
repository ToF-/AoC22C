#include <assert.h>
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "regolith.h"
#include "stdbool.h"

TEST_GROUP(regolith);


CAVE *cave;

TEST_SETUP(regolith) {
    cave = read_puzzle("sample.txt");
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(regolith) {
    destroy_cave(cave);
    UnityMalloc_EndTest();
}
TEST(regolith, read_puzzle) {
    TEST_ASSERT_EQUAL_INT(503, cave->width);
    TEST_ASSERT_EQUAL_INT(9,   cave->heigth);
    TEST_ASSERT_EQUAL_CHAR('.', at(cave, 498,0));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 498,4));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 498,5));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 498,6));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 497,6));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 496,6));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 503,4));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 502,4));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 494,9));
}
TEST(regolith, sand_falling) {
    TEST_ASSERT_TRUE(sand_fall(cave));
    TEST_ASSERT_EQUAL_CHAR('o', at(cave, 500,8));
    TEST_ASSERT_TRUE(sand_fall(cave));
    TEST_ASSERT_EQUAL_CHAR('o', at(cave, 499,8));
    
}
