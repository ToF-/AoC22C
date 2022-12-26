#include <stdbool.h>

typedef struct coord {
    int x;
    int y;
} COORD;

typedef struct {
    COORD location;
    COORD beacon;
    COORD *perimeter;
} SENSOR;

typedef struct coord_list {
    COORD coord;
    struct coord_list *next;
} COORD_LIST;

int manhattan_distance(COORD, COORD);
SENSOR *sensor(COORD, COORD);
COORD coord(int, int);
bool equal_coords(COORD, COORD);
void destroy_sensor(SENSOR *);
COORD_LIST *intersections(SENSOR *, SENSOR *);
void destroy_coord_list(COORD_LIST *);
COORD_LIST *insert_coord(COORD_LIST *, COORD);
COORD_LIST *append_coord_lists(COORD_LIST *, COORD_LIST *);
COORD_LIST *all_intersections(SENSOR **, int);
COORD_LIST *interesting_coords(SENSOR **, int);


