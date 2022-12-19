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
    ELEMENT *element;
    element = (ELEMENT *)malloc(sizeof(ELEMENT));
    element->type = LIST_ELEMENT;
    element->AS.list = list;
    assert(element->type == INTEGER_ELEMENT || element->type == LIST_ELEMENT);
    return element;
}

ELEMENT *new_integer_element(char c) {
    ELEMENT *element;
    element = (ELEMENT *)malloc(sizeof(ELEMENT));
    element->type = INTEGER_ELEMENT;
    element->AS.integer = todigit(c);
    assert(element->type == INTEGER_ELEMENT || element->type == LIST_ELEMENT);
    return element;
}


ELEMENT *accumulate_integer(ELEMENT *element, char c) {
    assert(element->type == INTEGER_ELEMENT || element->type == LIST_ELEMENT);
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
                assert(current->head->type == INTEGER_ELEMENT || current->head->type == LIST_ELEMENT);
                stack[level++] = current;
                current = current->head->AS.list;
                break;
            case ']':
                --level;
                assert(level>=0);
                current = stack[level];
                break;
        }
        c = *entry++;
    }
    assert(result);
    assert(result->head);
    ELEMENT *element = result->head;
    assert(element->type == INTEGER_ELEMENT || element->type == LIST_ELEMENT);
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


LIST *new_singleton(int integer) {
    LIST *singleton;
    ELEMENT *element = (ELEMENT *)malloc(sizeof(ELEMENT));
    element->type = INTEGER_ELEMENT;
    element->AS.integer = integer;
    singleton = new_list();
    singleton->head = element;
    singleton->tail = NULL;
    return singleton;
}

int compare_elements(ELEMENT *a, ELEMENT *b) {
    assert(a != NULL);
    assert(b != NULL);
    int types = a->type * 10 + b->type;
    switch(types) {
        case 00: return a->AS.integer - b->AS.integer;
        case 11: return compare_lists(a->AS.list, b->AS.list);
        case 01: return compare_lists(new_singleton(a->AS.integer), b->AS.list);
        case 10: return compare_lists(a->AS.list, new_singleton(b->AS.integer));
    }
    return 0;
}

int compare_lists(LIST *a, LIST *b) {
    assert(a != NULL);
    assert(b != NULL);
    int lists;
    lists = (a->head == NULL ? 0 : 10) + (b->head == NULL ? 0 : 1);
    switch(lists) {
        case 00: return 0;
        case 01: return -1;
        case 10: return 1;
    }
    assert(a->head != NULL);
    assert(b->head != NULL);
    int cmp_element = compare_elements(a->head, b->head);
    if(cmp_element == 0) {
        lists = (a->tail == NULL ? 0 : 10) + (b->tail == NULL ? 0 : 1);
        switch(lists) {
            case 00: return 0;
            case 01: return -1;
            case 10: return 1;
            case 11: return compare_lists(a->tail, b->tail);
        }
        return 0;
    } else
        return cmp_element;
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
        if(compare_lists(left, right) < 0) {
            sum += pair; 
        }
    }
    return sum;
}

void print_debug_element(ELEMENT *element) {
    if(element == NULL) {
        printf("$");
        return;
    }
    if((element->type != INTEGER_ELEMENT) && (element->type != LIST_ELEMENT)) {
        printf("%p", (void *)element);
    }
    switch(element->type) {
        case INTEGER_ELEMENT:
            printf("%d", element->AS.integer);
            return;
        case LIST_ELEMENT:
            print_debug_list(element->AS.list);
            return;
    }
    printf("?");
    return;
}

void print_debug_list(LIST *list) {
    if(list == NULL) {
        printf("_");
        return;
    }
    // printf("%p\t", (void *)list);
    if(list->head == NULL) {
        printf("#");
        return;
    }
    printf("(");
    print_debug_element(list->head);
    printf(":");
    print_debug_list(list->tail);
    printf(")");
}

int compare(const void *pa, const void *pb) {
    LIST *a = (LIST *)pa;
    LIST *b = (LIST *)pb;
    printf("\ncomparing\n");
    print_debug_list(a);
    printf("\nand\n");
    print_debug_list(b);
    printf("\n");
    return compare_lists(a, b);
}

int solve_part2(LIST **lists, char *filename) {
    int count = read_puzzle(lists, filename);
    int div1_pos, div2_pos;
    LIST *divider1 = packet("[[2]]");
    LIST *divider2 = packet("[[6]]");
    lists[count++] = divider1;
    lists[count++] = divider2;
    for(int j=0; j<count-2; j++) {
        for(int i=j+1; i<count; i++) {
            assert(lists[j]);
            assert(lists[i]);
            if(compare_lists(lists[i],lists[j]) < 0) {
                LIST *temp = lists[i];
                lists[i] = lists[j];
                lists[j] = temp;
            }
        }
    }
    for(int i=0; i<count; i++) {
        print_debug_list(lists[i]);
        printf("\n");
    }
    printf("-----------\n");
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
