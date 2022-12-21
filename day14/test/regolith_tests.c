#include <assert.h>
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "regolith.h"
#include "stdbool.h"

TEST_GROUP(regolith);


CAVE *cave;

TEST_SETUP(regolith) {
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(regolith) {
    destroy_cave(cave);
    UnityMalloc_EndTest();
}
TEST(regolith, read_puzzle) {
    cave = read_puzzle("sample.txt");
    TEST_ASSERT_EQUAL_INT(1006, cave->width);
    TEST_ASSERT_EQUAL_INT(18,   cave->height);
    TEST_ASSERT_EQUAL_CHAR('.', at(cave, 498,0));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 498,4));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 498,5));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 498,6));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 497,6));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 496,6));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 503,4));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 502,4));
    TEST_ASSERT_EQUAL_CHAR('#', at(cave, 494,9));
    print_cave(cave);
}
TEST(regolith, sand_falling) {
    cave = read_puzzle("sample.txt");
    TEST_ASSERT_TRUE(sand_fall(cave, 500, 0));
    TEST_ASSERT_EQUAL_CHAR('o', at(cave, 500,8));
    TEST_ASSERT_TRUE(sand_fall(cave, 500, 0));
    TEST_ASSERT_EQUAL_CHAR('o', at(cave, 499,8));
    TEST_ASSERT_TRUE(sand_fall(cave, 500, 0));
    TEST_ASSERT_EQUAL_CHAR('o', at(cave, 501,8));
    for(int i=3; i<24; i++) {
        TEST_ASSERT_TRUE(sand_fall(cave, 500, 0));
    }
    print_cave(cave);
    TEST_ASSERT_FALSE(sand_fall(cave, 500, 0));
}
TEST(regolith, solve_part1_sample) {
    cave = read_puzzle("sample.txt");
    TEST_ASSERT_EQUAL_INT(24, stopped_sand(cave));
}
TEST(regolith, solve_part1_puzzle) {
    cave=read_puzzle("puzzle.txt");
    print_cave(cave);
    TEST_ASSERT_EQUAL_INT(817, stopped_sand(cave));
    print_cave(cave);
}
