#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "hill_climbing.h"

TEST_GROUP(hill_climbing);

HEIGHT_MAP *map;

TEST_SETUP(hill_climbing) {
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
    map = read_puzzle("sample.txt");
/*
    Sabqponm
    abcryxxl
    accszExk
    acctuvwj
    abdefghi
*/

}
TEST_TEAR_DOWN(hill_climbing) {
    destroy_height_map(map);
    UnityMalloc_EndTest();
}
TEST(hill_climbing, can_read_a_puzzle){
    TEST_ASSERT_EQUAL_INT(5, map->max_row);
    TEST_ASSERT_EQUAL_INT(8, map->max_col);
    TEST_ASSERT_EQUAL_CHAR('S', square_at(map, (COORD) { .row = 0, .col = 0}));
    TEST_ASSERT_EQUAL_CHAR('m', square_at(map, (COORD) { .row = 0, .col = 7}));
    TEST_ASSERT_EQUAL_CHAR('i', square_at(map, (COORD) { .row = 4, .col = 7}));
    TEST_ASSERT_EQUAL_CHAR('a', square_at(map, (COORD) { .row = 4, .col = 0}));
}
TEST(hill_climbing, finding_adjacent_squares) {
    static COORD squares[4];
    int adj_max = adjacent_squares(map, (COORD) { .row = 0, .col = 0}, squares);
    TEST_ASSERT_EQUAL_INT(2, adj_max);
    TEST_ASSERT_EQUAL(1, squares[0].row);
    TEST_ASSERT_EQUAL(0, squares[0].col);
    TEST_ASSERT_EQUAL(0, squares[1].row);
    TEST_ASSERT_EQUAL(1, squares[1].col);

    adj_max = adjacent_squares(map, (COORD) { .row = 1, .col = 4}, squares);
    TEST_ASSERT_EQUAL_INT(4, adj_max);
}
