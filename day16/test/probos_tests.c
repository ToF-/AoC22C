#include <assert.h>
#include "unity_fixture.h"
#include "unity.h"
#include "unity_memory.h"
#include "probos.h"

TEST_GROUP(probos);

SOLVER *solver;

void scan_sample(SOLVER *solver) {
     scan_device(solver, "Valve AA has flow rate=0; tunnels lead to valves DD, II, BB");
     scan_device(solver, "Valve BB has flow rate=13; tunnels lead to valves CC, AA");
     scan_device(solver, "Valve CC has flow rate=2; tunnels lead to valves DD, BB");
     scan_device(solver, "Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE");
     scan_device(solver, "Valve EE has flow rate=3; tunnels lead to valves FF, DD");
     scan_device(solver, "Valve FF has flow rate=0; tunnels lead to valves EE, GG");
     scan_device(solver, "Valve GG has flow rate=0; tunnels lead to valves FF, HH");
     scan_device(solver, "Valve HH has flow rate=22; tunnel leads to valve GG");
     scan_device(solver, "Valve II has flow rate=0; tunnels lead to valves AA, JJ");
     scan_device(solver, "Valve JJ has flow rate=21; tunnel leads to valve II");
 }

TEST_SETUP(probos) {
    UnityMalloc_StartTest(); // see unity/extras/memory/readme.md
}
TEST_TEAR_DOWN(probos) {
    UnityMalloc_EndTest();
}
TEST(probos, id) {
    solver = new_solver();
    scan_sample(solver);
    TEST_ASSERT_EQUAL_INT(0, get_index(solver, "AA"));
    destroy_solver(solver);
}

TEST(probos, scan) {
    solver = new_solver();
    scan_sample(solver);
    calc_distances(solver);
    for(int i=0; i<solver->max_id; i++) {
        for(int j=0; j<solver->max_id; j++) {
            printf("(%s,%s=%d)\t", solver->valves[i]->tag, solver->valves[j]->tag, solver->dist[i][j]);
        }
        printf("\n");
    }
    TEST_ASSERT_EQUAL_INT(10, solver->max_id);
    TEST_ASSERT_EQUAL_INT(0, get_index(solver, "AA"));
    TEST_ASSERT_EQUAL_INT(0, solver->valves[get_index(solver, "AA")]->rate);
    for(int i=0; i<solver->max_id; i++) {
        VALVE *v = solver->valves[i];
        char *tag = v->tag;
        TEST_ASSERT_EQUAL_INT(get_index(solver, tag), i);
        printf("%d:%s\t", i, solver->valves[i]->tag);
    }
    printf("\n");
    TEST_ASSERT_EQUAL_INT(1, solver->dist[get_index(solver, "AA")][get_index(solver,"DD")]);
    TEST_ASSERT_EQUAL_INT(1, solver->dist[get_index(solver, "DD")][get_index(solver,"AA")]);
    TEST_ASSERT_EQUAL_INT(2, solver->dist[get_index(solver, "AA")][get_index(solver,"JJ")]);
    for(int i=0; i<solver->max_non_zero; i++) {
        printf("%s:%d\t", solver->valves[solver->non_zero[i]]->tag, solver->valves[solver->non_zero[i]]->rate);
    }
    printf("\n");
    destroy_solver(solver);
}
TEST(probos, scan_file) {
    solver = new_solver();
    scan_file(solver, "puzzle.txt");
    calc_distances(solver);
    for(int i=0; i<solver->max_id; i++) {
        for(int j=0; j<solver->max_id; j++) {
            printf("%2d ", solver->dist[i][j]);
        }
        printf("\n");
    }
    for(int i=0; i<solver->max_non_zero; i++) {
        printf("%s:%d\t", solver->valves[solver->non_zero[i]]->tag, solver->valves[solver->non_zero[i]]->rate);
    }
    printf("\n");
    TEST_ASSERT_EQUAL_INT(15, solver->max_non_zero);
    destroy_solver(solver);
}
TEST(probos, pressure) {
    solver = new_solver();
    scan_file(solver, "sample.txt");
    calc_distances(solver);
    TEST_ASSERT_EQUAL_INT(6, solver->max_non_zero);
    TEST_ASSERT_EQUAL_STRING("BB",  solver->valves[next_to_open(solver, 0, 0)]->tag);
    TEST_ASSERT_EQUAL_STRING("CC",  solver->valves[next_to_open(solver, 0, 1)]->tag);
    TEST_ASSERT_EQUAL_STRING("DD",  solver->valves[next_to_open(solver, 0, 2)]->tag);
    TEST_ASSERT_EQUAL_STRING("EE",  solver->valves[next_to_open(solver, 0, 3)]->tag);
    TEST_ASSERT_EQUAL_STRING("HH",  solver->valves[next_to_open(solver, 0, 4)]->tag);
    TEST_ASSERT_EQUAL_STRING("JJ",  solver->valves[next_to_open(solver, 0, 5)]->tag);
    TEST_ASSERT_EQUAL_STRING("CC",  solver->valves[next_to_open(solver, 1, 0)]->tag);
    TEST_ASSERT_EQUAL_STRING("CC",  solver->valves[next_to_open(solver, 1, 1)]->tag);
    TEST_ASSERT_EQUAL_STRING("DD",  solver->valves[next_to_open(solver, 1, 2)]->tag);
    TEST_ASSERT_EQUAL_STRING("BB",  solver->valves[next_to_open(solver, 2, 0)]->tag);
    TEST_ASSERT_EQUAL_STRING("DD",  solver->valves[next_to_open(solver, 2, 1)]->tag);
    TEST_ASSERT_EQUAL_STRING("DD",  solver->valves[next_to_open(solver, 3, 0)]->tag);
    TEST_ASSERT_EQUAL_STRING("HH",  solver->valves[next_to_open(solver, 15, 0)]->tag);
    TEST_ASSERT_EQUAL_STRING("BB",  solver->valves[next_to_open(solver, 14, 0)]->tag);
    TEST_ASSERT_EQUAL_INT(-1, next_to_open(solver, 65535, 0));
    destroy_solver(solver);
}
