enum ELEMENT_TYPE { INTEGER_ELEMENT, LIST_ELEMENT };

typedef struct list {
    struct element *head;
    struct list    *tail;
} LIST;

typedef struct element {
    enum ELEMENT_TYPE type;
    union {
        LIST *list;
        int  integer;
    } AS;
} ELEMENT; 

LIST *packet(char *line);
void destroy_packet(LIST *list);
void print_packet(LIST *list);
int right_order(LIST *left, LIST *right);
int read_puzzle(LIST **lists, char *filename);
int solve_part1(LIST **lists, int count);
int solve_part2(LIST **lists, char *filename);
ELEMENT *clone_element(ELEMENT *element);
LIST *clone_list(LIST *list);
int compare_elements(ELEMENT *a, ELEMENT *b);
int compare_lists(LIST *a, LIST *b);
void print_debug_list(LIST *list);
void print_debug_element(ELEMENT *element);
