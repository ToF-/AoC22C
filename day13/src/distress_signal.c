#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "distress_signal.h"


LIST *packet(char *entry) {
    LIST *result;
    result = (LIST *)malloc(sizeof(LIST));
    result->head = NULL;
    result->tail = NULL;
    char c;
    c = *entry++;
    assert(c == '[');
    c = *entry++;
    if(isdigit(c)) {
        result->head = (ELEMENT *)malloc(sizeof(ELEMENT));
        result->head->type = INTEGER_ELEMENT;
        result->head->AS.integer = c-'0';
    }
    return result;
}

void destroy_packet(LIST *list) {
    free(list);
}
