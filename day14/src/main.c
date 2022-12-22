#include <stdio.h>
#include "regolith.h"

int main(int argc, char *argv[]) {
    CAVE *cave =read_puzzle(argv[1]);
    printf("%ld %p %p %p %p %p %p %p %p\n", sizeof(int),
            (void *)cave,
            (void *)&cave->width,
            (void *)&cave->height,
            (void *)&cave->xmin,
            (void *)&cave->xmax,
            (void *)&cave->ymin,
            (void *)&cave->ymax,
            (void *)cave->content);
    print_cave(cave);
    printf("%d\n", stopped_sand(cave));
    destroy_cave(cave);

}

