#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int LINE_MAX = 50;

enum node_type { DIR_NODE, FILE_NODE };

struct node {
    char *name;
    enum node_type type;
    long size;
    struct node *parent;
    struct node *siblings;
    struct node *children;
};

void print_file_system(struct node* node, int level) {
    char info[LINE_MAX];
    for(int i=0; i < level; i++)
        printf("\t");
    switch(node->type) {
        case DIR_NODE:
            strcpy(info, "(dir)");
            break;
        case FILE_NODE:
            sprintf(info, "(file, size=%ld)", node->size);
            break;
    }
    printf("- %s %s\n", node->name, info);
    if(node->type == DIR_NODE && node->children != NULL) 
        print_file_system(node->children, level+1);
    if(node->siblings != NULL)
        print_file_system(node->siblings, level);
}

struct node *new_dir(struct node* parent, char *dir_name) {
    struct node *node = malloc(sizeof(struct node));
    node->name = malloc(strlen(dir_name)+1);
    strcpy(node->name, dir_name);
    node->type = DIR_NODE;
    node->size = -1;
    node->parent = parent;
    node->siblings = NULL;
    node->children = NULL;
    return node;
}

struct node *add_node(struct node *parent, char *name, long size) {
    struct node *node = malloc(sizeof(struct node));
    node->name = malloc(strlen(name)+1);
    strcpy(node->name, name);
    node->type = size == -1 ? DIR_NODE : FILE_NODE;
    node->size = size;
    node->parent = parent;
    node->siblings = NULL;
    node->children = NULL;
    if(node->parent->children == NULL) {
        node->parent->children = node;
    } else {
        struct node *last_child = node->parent->children;
        while(last_child->siblings != NULL)
            last_child = last_child->siblings;
        last_child->siblings = node;
    }
    return node;
}

struct node *change_dir(struct node *current, char *name) {
    struct node *node;
    if(current->type != DIR_NODE)
        return NULL;
    node = current->children;
    while(node != NULL) {
        if(!strcmp(node->name, name))
            return node;
        else
            node = node->siblings;
    }
    return NULL;
}
void destroy_node(struct node* node) {
    if(node == NULL)
        return;
    free(node->name);
    destroy_node(node->siblings);
    destroy_node(node->children);
    free(node);
}

int main(int argc, char *argv[]) {
    FILE *puzzleFile;
    char line[LINE_MAX];

    struct node *file_system = NULL;
    struct node *current;

    if(argc == 1) {
        file_system = new_dir(file_system, "/");
        current = file_system;
        current = add_node(current, "a", -1);
        struct node *dir = current;
        current = add_node(dir, "b.txt", 14848514);
        current = add_node(dir, "c.dat", 8504156);
        current = add_node(dir, "d", -1);
        current = file_system;
        current = change_dir(current, "a");
        print_file_system(file_system, 0);
        print_file_system(current,0);
        destroy_node(file_system);
        return 0;
    }
    puzzleFile = fopen(argv[1], "r");
    if(puzzleFile == NULL) {
        fprintf(stderr, "can't open file %s\n", argv[1]);
        return 1;
    }
    while(fgets(line, LINE_MAX, puzzleFile)) {
    }
    fclose(puzzleFile);
    return 0;
}
