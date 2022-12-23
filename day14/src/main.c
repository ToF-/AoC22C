#include <stdio.h>
#include "regolith.h"

int main(int argc, char *argv[]) {
    assert(argc == 2);
    CAVE *cave =read_puzzle(argv[1]);
    print_cave(cave);
    printf("%d\n", stopped_sand(cave));
    print_debug_cave(cave);
    destroy_cave(cave);

}

