#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#ifdef SAMPLE
#else
#endif
#define PART2
#define WORRY_LEVEL 3

#ifdef SAMPLE
const long MCD = 13 * 17 * 19 * 23;
#else
const long MCD = 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19;
#endif


const int MAX_ITEMS = 50;

typedef struct number {
    long factor;
    long remainder;
} NUMBER;

typedef struct item {
    void *value;
    struct item *next;
} ITEM;

typedef struct list {
    int count;
    ITEM *items;
} LIST;

void add(LIST *list, void *value) {
    ITEM *item = malloc(sizeof(ITEM));
    item->value = value;
    item->next = NULL;
    if(list->count) {
        ITEM *p = list->items;
        while(p->next != NULL)
            p = p->next;
        p->next = item;
    } else {
        list->items = item;
    }
    list->count++;
}

void push(LIST *list, void *value) {
    ITEM *item = malloc(sizeof(ITEM));
    item->value = value;
    item->next = list->items;
    list->items = item;
    list->count++;
}

bool pop(LIST *list, void **value) {
    assert(list);
    *value = NULL;
    if(list->count == 0)
        return false;
    *value = list->items->value;
    ITEM *to_remove = list->items;
    list->items = list->items->next;
    list->count--;
    assert(to_remove);
    free(to_remove);
    return true;
}

LIST *new_list(int count, long values[MAX_ITEMS]) {
    LIST *list = malloc(sizeof(LIST));
    for(int i=0; i<count; i++) {
        long *cell = (long *)malloc(sizeof(long));
        *cell = values[i];
        add(list, cell);
    }
    return list;
}

void destroy_list(LIST *list) {
    assert(list);
    void *value;
    while(pop(list, &value)) {
        if(value)
            free(value);
    }
    free(list);
}

enum operation { PLUS, TIMES, SQUARE };

typedef struct monkey {
    struct monkey **circus;
    int number;
    LIST *list;
    enum operation operation;
    long  term;
    long divisor;
    int dest_if_true;
    int dest_if_false;
    long business;
} MONKEY;

MONKEY *new_monkey(
        MONKEY *circus[],
        int number, 
        int item_count, 
        long values[MAX_ITEMS], 
        enum operation operation, 
        long term, 
        long divisor,
        int dest_if_true,
        int dest_if_false) {
    MONKEY *monkey = malloc(sizeof(MONKEY));
    monkey->circus = circus;
    monkey->number = number;
    monkey->list = new_list(item_count, values);
    monkey->operation = operation;
    monkey->term = term;
    monkey->divisor = divisor;
    monkey->dest_if_true = dest_if_true;
    monkey->dest_if_false = dest_if_false;
    monkey->business = 0;
    return monkey;
}

void destroy_monkey(MONKEY *monkey) {
    assert(monkey);
    destroy_list(monkey->list);
    free(monkey);
}


MONKEY *part1_compute(MONKEY *monkey, void *pointer) {
    long value = *((long *)pointer);
    long product;
    switch(monkey->operation) {
        case PLUS: product = value + monkey->term; break;
        case TIMES: product = value * monkey->term; break;
        case SQUARE: product = value * value; break;
    }
    long result = product / WORRY_LEVEL;
    long *cell = (long *)malloc(sizeof(long));
    *cell = result;
    push(monkey->list, cell);
    return (result % monkey->divisor == 0) ?
        monkey->circus[monkey->dest_if_true] :
        monkey->circus[monkey->dest_if_false];
}

MONKEY *part2_compute(MONKEY *monkey, void *pointer) {
    long value = *((long *)pointer);
    long result;
    switch(monkey->operation) {
        case PLUS: result = (value + monkey->term) % MCD; break;
        case TIMES: result = (value * monkey->term) % MCD; break;
        case SQUARE: result = ((value % MCD) * (value % MCD)) % MCD; break; 
    }
    long *cell = (long *)malloc(sizeof(long));
    *cell = result;
    push(monkey->list, cell);
    return (result % monkey->divisor == 0) ?
        monkey->circus[monkey->dest_if_true] :
        monkey->circus[monkey->dest_if_false];
}

bool process(MONKEY *monkey) {
    void *pointer;
    if(!pop(monkey->list, &pointer))
        return false;
#ifdef PART1
    MONKEY *dest_monkey = part1_compute(monkey, pointer);
#else
    MONKEY *dest_monkey = part2_compute(monkey, pointer);
#endif
    pop(monkey->list, &pointer);
    add(dest_monkey->list, pointer);
    monkey->business++;
    return true;
}

void print_monkey(MONKEY *monkey) {
    ITEM *item;
    printf("%d (%ld)", monkey->number, monkey->business);
    if(! monkey->list->count) {
        printf("[]\n");
        return;
    }
    printf("[");
    item = monkey->list->items;
    while(item) {
        long *value = (long *)item->value;
        printf("%ld", *value);
        printf("%c", item->next ? ',' : ']');
        item = item->next;
    }
    printf("\n");
}
void a_turn(MONKEY *monkey) {
    while(process(monkey));
}

void a_round(int count, MONKEY *circus[]) {
    for(int m=0; m<count; m++) {
        a_turn(circus[m]);
    }
}


int main(int argc, char *argv[]) {
#ifdef SAMPLE
    const int COUNT = 4;
    MONKEY *circus[COUNT];
    circus[0] = new_monkey(circus, 0, 2, (long []){ 79, 98         }, TIMES, 19, 23, 2, 3);
    circus[1] = new_monkey(circus, 1, 4, (long []){ 54, 65, 75, 74 }, PLUS,   6, 19, 2, 0);
    circus[2] = new_monkey(circus, 2, 3, (long []){ 79, 60, 97     }, SQUARE, 0, 13, 1, 3);
    circus[3] = new_monkey(circus, 3, 1, (long []){ 74             }, PLUS,   3, 17, 0, 1);
#else
    const int COUNT = 8;
    MONKEY *circus[COUNT];
    circus[0] = new_monkey(circus, 0, 6, (long []) { 50, 70, 89, 75, 66, 66         }, TIMES,  5,  2, 2, 1);
    circus[1] = new_monkey(circus, 1, 1, (long []) { 85                             }, SQUARE, 0,  7, 3, 6);
    circus[2] = new_monkey(circus, 2, 8, (long []) { 66, 51, 71, 76, 58, 55, 58, 60 }, PLUS,   1, 13, 1, 3);
    circus[3] = new_monkey(circus, 3, 4, (long []) { 79, 52, 55, 51                 }, PLUS,   6,  3, 6, 4);
    circus[4] = new_monkey(circus, 4, 2, (long []) { 69, 92                         }, TIMES, 17, 19, 7, 5);
    circus[5] = new_monkey(circus, 5, 7, (long []) { 71, 76, 73, 98, 67, 79, 99     }, PLUS,   8,  5, 0, 2);
    circus[6] = new_monkey(circus, 6, 5, (long []) { 82, 76, 69, 69, 57             }, PLUS,   7, 11, 7, 4);
    circus[7] = new_monkey(circus, 7, 3, (long []) { 65, 79, 86                     }, PLUS,   5, 17, 5, 0);
#endif
    int max_round = atoi(argv[1]);
    for(int i=0; i<max_round; i++) {
        a_round(COUNT, circus);
    }
    for(int m=0; m<COUNT; m++) print_monkey(circus[m]); printf("\n"); 

    long first = 0;
    long second = 0;
    for(int m=0; m<COUNT; m++) if(first<circus[m]->business) first = circus[m]->business;
    for(int m=0; m<COUNT; m++) if(second <circus[m]->business && circus[m]->business != first) second = circus[m]->business;
    printf("monkey business:%ld *%ld = %ld\n", first, second, first * second);
    for(int m=0; m<COUNT; m++)
        destroy_monkey(circus[m]);
    return 0;
}



