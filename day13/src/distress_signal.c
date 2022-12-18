#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "distress_signal.h"

const int MAX_LIST_LEVELS = 100;

ELEM *new_elem(enum ELEM_TYPE type) {
    ELEM *elem = (ELEM *)malloc(sizeof(ELEM));
    elem->type = type;
    elem->next = NULL;
    switch(elem->type) {
        case INTEGER:
            elem->value.number = 0;
            break;
        case LIST:
            elem->value.head = NULL;
            break;
    }
    return elem;
}

void convert(ELEM *elem, char c) {
    elem->value.number *= 10;
    elem->value.number += c-'0';
}

void print_packet(ELEM *elem) {
    if(elem==NULL)
        return;
    switch(elem->type) {
        case INTEGER:
            printf("%d",elem->value.number);
            break;
        case LIST:
            printf("(");
            print_packet(elem->value.head);
            printf(":");
            if(elem->next) {
                printf("[");
                print_packet(elem->next);
                printf("]");
            }
            else {
                printf("[]");
            }
            printf(")");
            break;
    }
}

ELEM *packet(char *line) {
    ELEM *current = NULL;
    ELEM *stack[MAX_LIST_LEVELS];
    int level = 0;
    bool in_number = false;
    while(*line) {
        char c = *line++;
        printf(">%c",c);
        switch(c) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if(!in_number) {
                    current->value.head = new_elem(INTEGER);
                    convert(current->value.head, c);
                    in_number = true;
                } else {
                    convert(current->value.head, c);
                }
                in_number = true;
                break;
            case '[':
                in_number = false;
                assert(level<MAX_LIST_LEVELS);
                stack[level] = new_elem(LIST);
                if(current != NULL)
                    current->next = stack[level];
                current = stack[level];
                level++;
                printf("(level %d)",level);
                break;
            case ',':
                in_number = false;
                current->next = new_elem(LIST);
                current = current->next;
                break;
            case ']':
                in_number = false;
                level--;
                assert(level>=0);
                current = stack[level];
                break;
        }
    }
    printf("\n");
    return stack[0];
}

void destroy_packet(ELEM *elem) {
    if(elem == NULL)
        return;
    switch(elem->type) {
        case INTEGER:
            break;
        case LIST:
            destroy_packet(elem->value.head);
            break;
    }
    destroy_packet(elem->next);
    free(elem);
}
