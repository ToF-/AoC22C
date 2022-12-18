enum ELEM_TYPE { INTEGER, LIST };

typedef struct elem {
    enum ELEM_TYPE type;
    union {
        struct elem *head;
        int number;
    } value;
    struct elem *next; 
} ELEM;

ELEM *packet(char *line);

void destroy_packet(ELEM *elem);
void print_packet(ELEM *elem);
