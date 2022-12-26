#include <assert.h>
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "beacon.h"

TEST_GROUP(beacon);

SENSOR *sensors[14];
int count = 14;

TEST_SETUP(beacon) {
    sensors[ 0]=sensor(coord(2, 18),coord(-2, 15));
    sensors[ 1]=sensor(coord(9, 16),coord(10, 16));
    sensors[ 2]=sensor(coord(13, 2),coord(15, 3));
    sensors[ 3]=sensor(coord(12, 14),coord(10, 16));
    sensors[ 4]=sensor(coord(10, 20),coord(10, 16));
    sensors[ 5]=sensor(coord(14, 17),coord(10, 16));
    sensors[ 6]=sensor(coord(8, 7),coord(2, 10));
    sensors[ 7]=sensor(coord(2, 0),coord(2, 10));
    sensors[ 8]=sensor(coord(0, 11),coord(2, 10));
    sensors[ 9]=sensor(coord(20, 14),coord(25, 17));
    sensors[10]=sensor(coord(17, 20),coord(21, 22));
    sensors[11]=sensor(coord(16, 7),coord(15, 3));
    sensors[12]=sensor(coord(14, 3),coord(15, 3));
    sensors[13]=sensor(coord(20, 1),coord(15, 3));
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(beacon) {
    for(int i=0; i<count; i++)
        destroy_sensor(sensors[i]);

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

void print_sensors(SENSOR **sensors, int count) {
    printf("\n");
    for(int row = -15; row < 35; row++) {
        for(int col = -15; col < 35; col++) {
            char c = '.';
            for(int i=0; i < count; i++) {
                SENSOR *s = sensors[i];
                COORD pos = coord(col, row);
                if(equal_coords(pos, s->location))
                    c = 'S';
                else if(equal_coords(pos, s->beacon))
                    c = 'B';
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
    print_sensors(sensors, 14);
    COORD_LIST *l = all_intersections(sensors, count);
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
    COORD_LIST *l = interesting_coords(sensors, count);
    COORD_LIST *c = l;
    printf("interesting:\n");
    while(c) {
        printf("(%d,%d) ", c->coord.x, c->coord.y);
        c = c->next;
    }
    printf("\n");
    destroy_coord_list(l);
}
