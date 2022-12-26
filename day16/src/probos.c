#include "probos.h"

int valve_id(char *name) {
    char t = name[0];
    char u = name[1];
    return (t-'A') * 26 + (u-'A');
}
