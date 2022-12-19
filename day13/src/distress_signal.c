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


LIST *clone_list(LIST *list) {
    if(list == NULL)
        return NULL;
    LIST *clone = (LIST *)malloc(sizeof(LIST));
    clone->head = NULL;
    clone->tail = NULL;
    if(list->head) {
        clone->head = clone_element(list->head);
    }
    if(list->tail) {
        clone->tail = clone_list(list->tail);
    }
    return clone;
}

ELEMENT *clone_element(ELEMENT *element) {
    if(element == NULL)
        return NULL;
    ELEMENT *clone = (ELEMENT *)malloc(sizeof(ELEMENT));
    clone->type = element->type;
    switch(clone->type) {
        case INTEGER_ELEMENT:
            clone->AS.integer = element->AS.integer;
            break;
        case LIST_ELEMENT:
            clone->AS.list = clone_list(element->AS.list);
            break;
    }
    return clone;
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
    LIST *ca = clone_list(a);
    LIST *cb = clone_list(b);
    LIST *left = ca;
    LIST *right = cb;
    printf("++++\n");
    print_packet(a);
    print_packet(b);
    printf("====\n");
    print_packet(ca);
    print_packet(cb);
    printf("----\n");
    int result = right_order_rec(left, right);
    printf("****\n");
    print_packet(a);
    print_packet(b);
    printf("////\n");
    print_packet(ca);
    print_packet(cb);
    destroy_packet(ca);
    destroy_packet(cb);
    return result;
}

int right_order_rec(LIST *left, LIST *right) {
    int result = 0;
    while(result == 0 && left && left->head && right && right->head) {
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
            result = right_order_rec(left->head->AS.list, right->head->AS.list);
        }
        else if(left->head->AS.integer < right->head->AS.integer) {
            printf("T\n");
            result = -1;
            continue;
        }
        else if(left->head->AS.integer > right->head->AS.integer) {
            printf("F\n");
            result = 1;
            continue;
        }
        left = left->tail;
        right = right->tail;
    }
    if(left && right) {
        result = (!left->head && right->head ? -1 : left->head && !right->head ? 1 : 0);
    } else { 
        result = (!left && right ? -1 : left && !right ? 1 : 0);
    }
    return result;
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


int solve_part1(LIST **lists, int count) {
    int sum = 0;
    int pair = 0;
    for(int i=0; i<count; i+= 2) {
        pair = i/2 + 1;
        LIST *left  = lists[i];
        LIST *right = lists[i+1];
        if(right_order(left, right) == -1) {
            sum += pair; 
        }
    }
    return sum;
}

int compare(const void *pa, const void *pb) {
    LIST *left = (LIST *)pa;
    LIST *right= (LIST *)pb;
    if(right_order(left, right)) {
        return -1;
    } else {
        return 1;
    }
}

int solve_part2(LIST **lists, char *filename) {
    int count = read_puzzle(lists, filename);
    int div1_pos, div2_pos;
    LIST *divider1 = packet("[[2]]");
    LIST *divider2 = packet("[[6]]");
    lists[count++] = divider1;
    lists[count++] = divider2;
    for(int i=0; i<count; i++)
        print_packet(lists[i]);
    getchar();
    qsort(lists, count, sizeof(LIST *), compare);
    for(int i=0; i<count; i++)
        print_packet(lists[i]);
    getchar();
    printf("\n");
    for(int i=0; i<count; i++) {
        printf("%d:", i);
        print_packet(lists[i]);
        if(lists[i] == divider1) {
            div1_pos = i+1;
            printf("divider 1 is in pos:%d\n", div1_pos);
        }
        if(lists[i] == divider2) {
            div2_pos = i+1;
            printf("divider 2 is in pos:%d\n", div2_pos);
        }
    }
    return div1_pos * div2_pos;
}
