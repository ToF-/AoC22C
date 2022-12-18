#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "distress_signal.h"


int todigit(char c) {
    return c - '0';
}

LIST *new_list() {
    LIST *result;
    result = (LIST *)malloc(sizeof(LIST));
    result->head = NULL;
    result->tail = NULL;
    return result;
}

ELEMENT *new_list_element(LIST *list) {
    ELEMENT *result;
    result = (ELEMENT *)malloc(sizeof(ELEMENT));
    result->type = LIST_ELEMENT;
    result->AS.list = list;
    return result;
}

ELEMENT *new_integer_element(char c) {
    ELEMENT *result;
    result = (ELEMENT *)malloc(sizeof(ELEMENT));
    result->type = INTEGER_ELEMENT;
    result->AS.integer = todigit(c);
    return result;
}

ELEMENT *accumulate_integer(ELEMENT *element, char c) {
    int acc = element->AS.integer;
    acc = acc * 10 + todigit(c);
    element->AS.integer = acc;
    return element;
}

const int MAX_LIST_LEVEL = 100;

LIST *packet(char *entry) {
    char c;
    LIST *result = new_list();
    c = *entry++;
    assert(c == '[');
    c = *entry++;
    bool in_number = false;
    LIST *stack[MAX_LIST_LEVEL];
    int level = 0;
    stack[level++] = result;
    LIST *current = stack[0];
    while(c != '\0') {
        switch(c) {
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                if(!in_number) 
                    current->head = new_integer_element(c);
                else 
                    current->head = accumulate_integer(current->head, c);
                in_number = true;
                break;
            case ',':
                in_number = false;
                current->tail = new_list();
                current = current->tail;
                break;
            case '[':
                current->head = new_list_element(new_list());
                stack[level++] = current;
                current = current->head->AS.list;
                break;
            case ']':
                --level;
                current = stack[level];
                break;
        }
        c = *entry++;
    }
    return result;
}

void print_list(LIST *list, bool is_tail) {
    if(is_tail)
        printf(",");
    else
        printf("[");
    if(list->head != NULL) {
        switch(list->head->type) {
            case LIST_ELEMENT:
                print_list(list->head->AS.list, false);
                break;
            case INTEGER_ELEMENT:
                printf("%d", list->head->AS.integer);
        }
    }
    if(list->tail != NULL)
        print_list(list->tail, true);
    if(!is_tail)
        printf("]");
}

void print_packet(LIST *list) {
    if(list == NULL)
        return;
    print_list(list, false);
    printf("\n");
}
void destroy_packet(LIST *list) {
    if(list == NULL)
        return;
    if(list->head != NULL) {
        if(list->head->type == LIST_ELEMENT) {
            destroy_packet(list->head->AS.list);
        }
        free(list->head);
    }
    if(list->tail != NULL) {
        destroy_packet(list->tail);
    }
    free(list);
}
