#include <assert.h>
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "probos.h"

TEST_GROUP(probos);

SOLVER *solver;
/* void scan_sample(VALVE *valves) {
     scan_device(valves,"Valve AA has flow rate=0; tunnels lead to valves DD, II, BB");
     scan_device(valves,"Valve BB has flow rate=13; tunnels lead to valves CC, AA");
     scan_device(valves,"Valve CC has flow rate=2; tunnels lead to valves DD, BB");
     scan_device(valves,"Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE");
     scan_device(valves,"Valve EE has flow rate=3; tunnels lead to valves FF, DD");
     scan_device(valves,"Valve FF has flow rate=0; tunnels lead to valves EE, GG");
     scan_device(valves,"Valve GG has flow rate=0; tunnels lead to valves FF, HH");
     scan_device(valves,"Valve HH has flow rate=22; tunnel leads to valve GG");
     scan_device(valves,"Valve II has flow rate=0; tunnels lead to valves AA, JJ");
     scan_device(valves,"Valve JJ has flow rate=21; tunnel leads to valve II");
 }
*/

TEST_SETUP(probos) {
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
    solver = new_solver();
}
TEST_TEAR_DOWN(probos) {
    destroy_solver(solver);
    UnityMalloc_EndTest();
}
TEST(probos, id) {
    TEST_ASSERT_EQUAL_INT(0, id(solver, "AA"));
}
