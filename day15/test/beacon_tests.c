#include <assert.h>
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "beacon.h"

TEST_GROUP(beacon);

SENSOR *sample[14];
int sample_count = 14;
SENSOR *puzzle[28];
int puzzle_count = 28;


TEST_SETUP(beacon) {
    sample[ 0]=sensor(coord(2, 18),coord(-2, 15));
    sample[ 1]=sensor(coord(9, 16),coord(10, 16));
    sample[ 2]=sensor(coord(13, 2),coord(15, 3));
    sample[ 3]=sensor(coord(12, 14),coord(10, 16));
    sample[ 4]=sensor(coord(10, 20),coord(10, 16));
    sample[ 5]=sensor(coord(14, 17),coord(10, 16));
    sample[ 6]=sensor(coord(8, 7),coord(2, 10));
    sample[ 7]=sensor(coord(2, 0),coord(2, 10));
    sample[ 8]=sensor(coord(0, 11),coord(2, 10));
    sample[ 9]=sensor(coord(20, 14),coord(25, 17));
    sample[10]=sensor(coord(17, 20),coord(21, 22));
    sample[11]=sensor(coord(16, 7),coord(15, 3));
    sample[12]=sensor(coord(14, 3),coord(15, 3));
    sample[13]=sensor(coord(20, 1),coord(15, 3));
    
    puzzle[ 0]=sensor(coord(193758,2220950),coord(652350,2000000));
    puzzle[ 1]=sensor(coord(3395706,3633894),coord(3404471,3632467));
    puzzle[ 2]=sensor(coord(3896022,3773818),coord(3404471,3632467));
    puzzle[ 3]=sensor(coord(1442554,1608100),coord(652350,2000000));
    puzzle[ 4]=sensor(coord(803094,813675),coord(571163,397470));
    puzzle[ 5]=sensor(coord(3491072,3408908),coord(3404471,3632467));
    puzzle[ 6]=sensor(coord(1405010,486446),coord(571163,397470));
    puzzle[ 7]=sensor(coord(3369963,3641076),coord(3404471,3632467));
    puzzle[ 8]=sensor(coord(3778742,2914974),coord(4229371,3237483));
    puzzle[ 9]=sensor(coord(1024246,3626229),coord(2645627,3363491));
    puzzle[10]=sensor(coord(3937091,2143160),coord(4229371,3237483));
    puzzle[11]=sensor(coord(2546325,2012887),coord(2645627,3363491));
    puzzle[12]=sensor(coord(3505386,3962087),coord(3404471,3632467));
    puzzle[13]=sensor(coord(819467,239010),coord(571163,397470));
    puzzle[14]=sensor(coord(2650614,595151),coord(3367919,-1258));
    puzzle[15]=sensor(coord(3502942,6438),coord(3367919,-1258));
    puzzle[16]=sensor(coord(3924022,634379),coord(3367919,-1258));
    puzzle[17]=sensor(coord(2935977,2838245),coord(2645627,3363491));
    puzzle[18]=sensor(coord(1897626,7510),coord(3367919,-1258));
    puzzle[19]=sensor(coord(151527,640680),coord(571163,397470));
    puzzle[20]=sensor(coord(433246,1337298),coord(652350,2000000));
    puzzle[21]=sensor(coord(2852855,3976978),coord(3282750,3686146));
    puzzle[22]=sensor(coord(3328398,3645875),coord(3282750,3686146));
    puzzle[23]=sensor(coord(3138934,3439134),coord(3282750,3686146));
    puzzle[24]=sensor(coord(178,2765639),coord(652350,2000000));
    puzzle[25]=sensor(coord(3386231,3635056),coord(3404471,3632467));
    puzzle[26]=sensor(coord(3328074,1273456),coord(3367919,-1258));
    puzzle[27]=sensor(coord(268657,162438),coord(571163,397470));
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(beacon) {
    for(int i=0; i<sample_count; i++)
        destroy_sensor(sample[i]);

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
 4 ....############*...........
 5 ...###############..........
 6 ..#################.........
 7 .#########S#######S#........
 8 ..#################.........
 9 ...###############..........
10 ....B###########*...........
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
TEST(beacon, sensor_characteristics) {
    SENSOR *s = sensor(coord(0,0), coord(1,1));
    TEST_ASSERT_TRUE(equal_coords(s->perimeter[0], coord(-2,0)));
    TEST_ASSERT_TRUE(equal_coords(s->perimeter[7], coord(2, 0)));
    destroy_sensor(s);
}

TEST(beacon, intersections_between_two_sensors) {
    SENSOR *s = sensor(coord(8,7),coord(2,10));
    SENSOR *t = sensor(coord(14,3),coord(15,3));
    COORD_LIST *is = intersections(s, t);
    TEST_ASSERT_NOT_NULL(is);
    equal_coords(coord(13,3), is->coord);
    equal_coords(coord(14,4), is->next->coord);
    TEST_ASSERT_NULL(is->next->next);
    destroy_coord_list(is);
}
TEST(beacon, append_two_coordlists) {
    COORD_LIST *l = NULL;
    COORD_LIST *m = NULL;
    l = insert_coord(l, coord(23,17));
    l = insert_coord(l, coord(42,65));
    m = insert_coord(m, coord(48, 7));
    m = insert_coord(m, coord(11,19));
    l = append_coord_lists(l, m);
    TEST_ASSERT_TRUE(equal_coords(coord(42,65), l->coord));
    TEST_ASSERT_TRUE(equal_coords(coord(23,17), l->next->coord));
    TEST_ASSERT_TRUE(equal_coords(coord(11,19), l->next->next->coord));
    TEST_ASSERT_TRUE(equal_coords(coord(48, 7), l->next->next->next->coord));
    destroy_coord_list(l);
}

bool include(COORD_LIST *list, COORD coord) {
    while(list) {
        if(equal_coords(coord, list->coord))
            return true;
        list = list->next;
    }
    return false;
}
void print_sample(SENSOR **sample, int count) {
    COORD_LIST *is = interesting_coords(sample, count);
    COORD cmin, cmax;
    limits(sample, count, &cmin, &cmax);
    printf("\n");
    for(int row = cmin.y; row < cmax.y; row++) {
        for(int col = cmin.x; col < cmax.x; col++) {
            char c = '.';
            for(int i=0; i < count; i++) {
                SENSOR *s = sample[i];
                COORD pos = coord(col, row);
                if(equal_coords(pos, s->location))
                    c = 'S';
                else if(equal_coords(pos, s->beacon))
                    c = 'B';
                else if(include(is, pos))
                    c = '*';
                else {
                    int r = manhattan_distance(s->location, s->beacon);
                    int p = manhattan_distance(pos, s->location);
                    if((p <= r) && c == '.')
                        c = '#';
                }
            }
            printf("%c", c);
        }
        printf("\n");
    }

}
TEST(beacon, intersections_between_all_sensors) {
    print_sample(sample, 14);
    COORD_LIST *l = all_intersections(sample, sample_count);
    COORD_LIST *c = l;
    printf("intersections:\n");
    while(c) {
        printf("(%d,%d) ", c->coord.x, c->coord.y);
        c = c->next;
    }
    printf("\n");
    destroy_coord_list(l);
}
TEST(beacon, interesting_intersections) {
    COORD_LIST *l = interesting_coords(sample, sample_count);
    COORD_LIST *c = l;
    printf("interesting:\n");
    while(c) {
        printf("(%d,%d) ", c->coord.x, c->coord.y);
        c = c->next;
    }
    printf("\n");
    destroy_coord_list(l);
}
TEST(beacon, find_limits) {
    COORD cmin, cmax;
    limits(sample, sample_count, &cmin, &cmax);
    printf("(%d,%d) (%d,%d)\n", cmin.x, cmin.y, cmax.x, cmax.y);
    limits(puzzle, puzzle_count, &cmin, &cmax);
    printf("(%d,%d) (%d,%d)\n", cmin.x, cmin.y, cmax.x, cmax.y);
}
TEST(beacon, solution_1) {
    TEST_ASSERT_EQUAL_INT(26, excluded_in_row(sample, sample_count, 10));
    TEST_ASSERT_EQUAL_INT(5832528, excluded_in_row(puzzle, puzzle_count, 2000000));
}
