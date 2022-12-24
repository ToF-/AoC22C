#include <stdbool.h>

typedef struct coord {
    int x;
    int y;
} COORD;

typedef struct {
    COORD coord;
    COORD beacon;
} SENSOR;

bool excluded(SENSOR sensor, int x, int y);
int  distance(SENSOR sensor);
SENSOR get_sensor(char *line);
int get_puzzle(SENSOR *sensors, char *filename);
int excluded_positions(SENSOR *sensors, int count, int row);
