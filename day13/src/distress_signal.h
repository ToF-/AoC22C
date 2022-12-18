enum ELEM_TYPE { INTEGER, LIST };

typedef struct cons {
    void *head;
    struct cons *next;
} CONS;

typedef struct elem {
    enum ELEM_TYPE type;
    union {
        CONS *list;
        int number;
    } as;
} ELEM;

ELEM *packet(char *line);
void destroy_packet(ELEM *elem);
void print_packet(ELEM *elem);
int head_as_number(ELEM *elem);
