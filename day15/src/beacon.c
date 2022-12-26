#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "beacon.h"
#include <limits.h>

int manhattan_distance(COORD a, COORD b) {
    return abs(b.x - a.x) + abs(b.y - a.y);
}
int compare_coords(const void *p, const void *q) {
    COORD a = *((COORD *) p);
    COORD b = *((COORD *) q);
    if(a.x == b.x)
        return a.y - b.y;
    else
        return a.x - b.x;
}

SENSOR *sensor(COORD p, COORD b) {
    assert(! equal_coords(p, b));
    SENSOR *s = (SENSOR *)malloc(sizeof(SENSOR));
    s->location = p;
    s->beacon = b;
    int r = manhattan_distance(s->location, s->beacon);
    s->perimeter = (COORD *)malloc(sizeof(COORD) * r * 4);
    int count = 0;
    for(int x = -r, y = 0; x < 0; x++, y++)
        s->perimeter[count++] = coord(s->location.x + x, s->location.y + y);
    for(int x = 0, y = r; x < r; x++, y--)
        s->perimeter[count++] = coord(s->location.x + x, s->location.y + y);
    for(int x = r, y = 0; x > 0; x--, y--)
        s->perimeter[count++] = coord(s->location.x + x, s->location.y + y);
    for(int x = 0, y =-r; x >-r; x--, y++) 
        s->perimeter[count++] = coord(s->location.x + x, s->location.y + y);
    qsort(s->perimeter, r * 4, sizeof(COORD), compare_coords);
    return s;
}

COORD coord(int x, int y) {
    return (COORD) { .x = x, .y = y };
}

bool equal_coords(COORD a, COORD b) {
    return (a.x == b.x) && (a.y == b.y);
}

void destroy_sensor(SENSOR *s) {
    free(s->perimeter);
    free(s);
}

COORD_LIST *intersections(SENSOR *s, SENSOR *t) {
    assert(s);
    assert(t);
    COORD_LIST *list = NULL;
    COORD_LIST *current = NULL;
    int sp_count = manhattan_distance(s->location, s->beacon) * 4;
    int tp_count = manhattan_distance(t->location, t->beacon) * 4;
    for(int i=0, j=0; i < sp_count && j < tp_count; ) {
        int cmp = compare_coords(&s->perimeter[i], &t->perimeter[j]);
        if(cmp == 0) {
            list = insert_coord(list, s->perimeter[i]);
            i++;
            j++;
        } else if(cmp < 0)
            i++;
        else 
            j++;
    }
    return list;
}

void destroy_coord_list(COORD_LIST *list) {
    if(!list)
        return;
    destroy_coord_list(list->next);
    free(list);
}

COORD_LIST *insert_coord(COORD_LIST *list, COORD coord) {
    COORD_LIST *new = (COORD_LIST *)malloc(sizeof(COORD_LIST));
    new->coord = coord;
    new->next = NULL;
    if(list) 
        new->next = list;
    return new;
}

COORD_LIST *append_coord_lists(COORD_LIST *l, COORD_LIST *m) {
    if(!l)
        return m;
    if(!m)
        return l;
    COORD_LIST *cl = l;
    while(cl->next)
        cl = cl->next;
    cl->next = m;
    return l;
}

COORD_LIST *all_intersections(SENSOR ** sensors, int count) {
    COORD_LIST *l = NULL;
    for(int i = 0; i < count - 1; i++) {
        for(int j = i+1; j < count; j++) {
            COORD_LIST *is = intersections(sensors[i], sensors[j]);
            l = append_coord_lists(l, is);
        }
    }
    return l;
}

bool is_a_sensor_or_beacon(COORD pos, SENSOR **sensors, int count) {
    for(int i = 0; i < count; i++) {
        SENSOR *s = sensors[i];
        if(equal_coords(pos, s->location))
            return true;
        if(equal_coords(pos, s->beacon))
            return true;
    }
    return false;
}
bool can_be_excluded(COORD pos, SENSOR **sensors, int count) {
    if(is_a_sensor_or_beacon(pos, sensors, count))
        return true;
    for(int i = 0; i < count; i++) {
        SENSOR *s = sensors[i];
        int r = manhattan_distance(s->location, s->beacon);
        int p = manhattan_distance(pos, s->location);
        if(p <= r)
            return true;
    }
    return false;
}
bool interesting(COORD pos, SENSOR **sensors, int count) {
    if(pos.x < 0 || pos.x > 4000000 || pos.y < 0 || pos.y > 4000000)
        return false;
    COORD adjacent[4] = {
        (COORD) { .x = pos.x, .y = pos.y+1 },
        (COORD) { .x = pos.x, .y = pos.y-1 },
        (COORD) { .x = pos.x+1, .y = pos.y },
        (COORD) { .x = pos.x-1, .y = pos.y }
    };
    int adjs = 0;
    for(int i = 0; i < 4; i++) {
        if(can_be_excluded(adjacent[i], sensors, count))
            adjs++;
        if(is_a_sensor_or_beacon(adjacent[i], sensors, count))
            adjs++;
    }
    return ! can_be_excluded(pos, sensors, count) && adjs == 4;
}
COORD_LIST *interesting_coords(SENSOR **sensors, int count) {
    COORD_LIST *l = all_intersections(sensors, count);
    COORD_LIST *i = NULL;
    while(l) {
        COORD pos = l->coord;
        COORD n = coord(pos.x, pos.y+1);
        COORD s = coord(pos.x, pos.y-1);
        COORD e = coord(pos.x+1, pos.y);
        COORD w = coord(pos.x-1, pos.y);
        if(interesting(n, sensors, count))
            i = insert_coord(i, n);
        if(interesting(s, sensors, count))
            i = insert_coord(i, s);
        if(interesting(e, sensors, count))
            i = insert_coord(i, e);
        if(interesting(w, sensors, count))
            i = insert_coord(i, w);
        l = l->next;
    }
    return i;
}

void limits(SENSOR **sensors, int count, COORD *cmin, COORD *cmax) {
    int xmin = INT_MAX;
    int ymin = INT_MAX;
    int xmax = INT_MIN;
    int ymax = INT_MIN;
    for(int i=0; i<count; i++) {
        SENSOR *s = sensors[i];
        int x = s->location.x;
        int y = s->location.y;
        int r = manhattan_distance(s->location, s->beacon);
        if(x-r < xmin)
            xmin = x-r;
        if(x+r > xmax)
            xmax = x+r;
        if(y-r < ymin)
            ymin = y-r;
        if(y+r > ymax)
            ymax = y+r;
    }
    cmin->x = xmin;
    cmin->y = ymin;
    cmax->x = xmax;
    cmax->y = ymax;
}

int excluded_in_row(SENSOR **sensors, int count, int row) {
    COORD cmin, cmax;
    limits(sensors, count, &cmin, &cmax);
    int excl = 0;
    for(int col = cmin.x; col < cmax.x; col++) {
        COORD pos = coord(col, row);
        if(can_be_excluded(pos, sensors, count))
            excl++;
        if(is_a_sensor_or_beacon(pos, sensors, count))
            excl--;
    }
    return excl;
}
