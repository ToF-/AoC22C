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

void convert_to_list(ELEMENT *element) {
    assert(element->type == INTEGER_ELEMENT);
    int integer = element->AS.integer;
    ELEMENT *head_element = (ELEMENT *)malloc(sizeof(ELEMENT));
    head_element->type = INTEGER_ELEMENT;
    head_element->AS.integer = integer;
    LIST *list = new_list();
    list->head = head_element;
    list->tail = NULL;
    element->type = LIST_ELEMENT;
    element->AS.list = list;
}

void print_element(ELEMENT *element) {
    switch(element->type) {
        case INTEGER_ELEMENT:
            printf("%d ", element->AS.integer);
            break;
        case LIST_ELEMENT:
            print_packet(element->AS.list);
            break;
    }
}

int right_order(LIST *a, LIST *b) {
    LIST *left = a;
    LIST *right = b;
    while(left && left->head && right && right->head) {
        print_element(left->head);
        print_element(right->head);
        printf("\t");
        if(right->head->type == INTEGER_ELEMENT && left->head->type == LIST_ELEMENT) {
            printf("C\n");
            convert_to_list(right->head);
        }else if(left->head->type == INTEGER_ELEMENT && right->head->type == LIST_ELEMENT) {
            printf("C\n");
            convert_to_list(left->head);
        }
        if(left->head->type == LIST_ELEMENT && right->head->type == LIST_ELEMENT) {
            printf("R\n");
            int r = right_order(left->head->AS.list, right->head->AS.list);
            if(r)
                return r;
        }
        else if(left->head->AS.integer < right->head->AS.integer) {
            printf("T\n");
            return -1;
        }
        else if(left->head->AS.integer > right->head->AS.integer) {
            printf("F\n");
            return +1;
        }
        left = left->tail;
        right = right->tail;
    }
    if(left && right) {
        return (!left->head && right->head ? -1 : left->head && !right->head ? 1 : 0);
    } 
    return (!left && right ? -1 : left && !right ? 1 : 0);
}

const int MAX_LINE = 500;
int read_puzzle(LIST **lists, char *filename) {
    char line[MAX_LINE];
    int count = 0;
    FILE *puzzle_file = fopen(filename, "r");
    assert(puzzle_file != NULL);

   while(fgets(line, MAX_LINE, puzzle_file)) {
       int l = strcspn(line, "\n");
       line[l] = '\0';
       if(l > 0) {
            lists[count++] = packet(line);
       }
   } 
   fclose(puzzle_file);
   return count;
}
