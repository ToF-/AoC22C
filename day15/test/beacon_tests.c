#include <assert.h>
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "beacon.h"

TEST_GROUP(beacon);


SENSOR sensor;

TEST_SETUP(beacon) {
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(beacon) {
    UnityMalloc_EndTest();
}
/*
               1    1    2    2
     0    5    0    5    0    5
-2 ..........#.................
-1 .........###................
 0 ....S...#####...............
 1 .......#######........S.....
 2 ......#########S............
 3 .....###########SB..........
 4 ....#############...........
 5 ...###############..........
 6 ..#################.........
 7 .#########S#######S#........
 8 ..#################.........
 9 ...###############..........
10 ....B############...........
11 ..S..###########............
12 ......#########.............
13 .......#######..............
14 ........#####.S.......S.....
15 B........###................
16 ..........#SB...............
17 ................S..........B
18 ....S.......................
19 ............................
20 ............S......S........
21 ............................
22 .......................B....

*/
TEST(beacon, excluded) {
    sensor  = (SENSOR) { .coord = (COORD){ .x = 8, .y = 7}, .beacon = (COORD) { .x = 2, .y = 10}};
    TEST_ASSERT_FALSE(excluded(sensor,2,10));
    TEST_ASSERT_FALSE(excluded(sensor,1,11));
    TEST_ASSERT_TRUE(excluded(sensor,1,9));
    TEST_ASSERT_TRUE(excluded(sensor,16,8));
    TEST_ASSERT_TRUE(excluded(sensor,8,7));
}

TEST(beacon, get_sensor) {
    sensor = get_sensor("Sensor at x=2, y=18: closest beacon is at x=-2, y=15");
    TEST_ASSERT_EQUAL_INT(2,  sensor.coord.x);
    TEST_ASSERT_EQUAL_INT(18, sensor.coord.y);
    TEST_ASSERT_EQUAL_INT(-2, sensor.beacon.x);
    TEST_ASSERT_EQUAL_INT(15, sensor.beacon.y);
}

TEST(beacon, get_puzzle) {
    SENSOR sensors[14];
    TEST_ASSERT_EQUAL_INT(14, get_puzzle(sensors, "sample.txt"));
    TEST_ASSERT_EQUAL_INT(2,  sensors[0].coord.x);
    TEST_ASSERT_EQUAL_INT(18, sensors[0].coord.y);
    TEST_ASSERT_EQUAL_INT(-2, sensors[0].beacon.x);
    TEST_ASSERT_EQUAL_INT(15, sensors[0].beacon.y);
    TEST_ASSERT_EQUAL_INT(20, sensors[13].coord.x);
    TEST_ASSERT_EQUAL_INT(1,  sensors[13].coord.y);
    TEST_ASSERT_EQUAL_INT(15, sensors[13].beacon.x);
    TEST_ASSERT_EQUAL_INT(3,  sensors[13].beacon.y);
}

TEST(beacon, excluded_positions) {
    SENSOR sensors[14];
    int count = get_puzzle(sensors, "sample.txt");
    TEST_ASSERT_EQUAL_INT(26,excluded_positions(sensors, count, 10));
}
TEST(beacon, solve_puzzle_part1) {
    SENSOR sensors[28];
    int count = get_puzzle(sensors, "puzzle.txt");
    TEST_ASSERT_EQUAL_INT(28, count);
    TEST_ASSERT_EQUAL_INT(5832528,excluded_positions(sensors, count, 2000000));
}
