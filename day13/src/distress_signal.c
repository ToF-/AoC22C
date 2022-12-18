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
    switch(elem->type) {
        case INTEGER:
            elem->as.number = 0;
            break;
        case LIST:
            elem->as.list = NULL;
            break;
    }
    return elem;
}

void convert(ELEM *elem, char c) {
    elem->as.number *= 10;
    elem->as.number += c-'0';
}

int head_as_number(ELEM *elem){
    assert(elem->type == LIST);
    ELEM *head = (ELEM *)elem->as.list->head;
    assert(head->type == INTEGER);
    return head->as.number;
}

void print_packet(ELEM *elem) {
    if(elem==NULL)
        return;
    switch(elem->type) {
        case INTEGER:
            printf("%d",elem->as.number);
            break;
        case LIST:
            printf("[");
            ELEM *head = elem->as.list.head;
            ELEM *next = elem->as.list.next;
            if(head) {
                print_packet(head);
            }
            if(next) {
                printf(",");
                print_packet(next);
            }
            printf("]");
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
            case '[':
                assert(level < MAX_LIST_LEVELS);
                stack[level] = new_elem(LIST);
                current_list = stack[level];
                level++;
                break;
            case ',':
                current_list->as.list->next = new_elem(LIST);
                current_list = current_list->as.list->next;
                break;
            case ']':
                in_number = false;
                --level;
                assert(level >= 0);
                current_list = stack[level];
                break;
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
                    current_list->as.list->head = new_elem(INTEGER);
                    convert(current_list->as.list->head, c);
                    in_number = true;
                } else {
                    convert(current_lsit->as.lisst->head, c);
                }
                in_number = true;
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
            destroy_packet(elem->as.head);
            break;
    }
    destroy_packet(elem->next);
    free(elem);
}
