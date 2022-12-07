#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

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
        printf("  ");
    switch(node->type) {
        case DIR_NODE:
            sprintf(info, "(dir) [%ld]", node->size);
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

struct node *new_node(char *name, long size) {
    struct node *node = malloc(sizeof(struct node));
    node->name = malloc(strlen(name)+1);
    memset(node->name, 0, strlen(name)+1);
    strcpy(node->name, name);
    node->type = size == -1 ? DIR_NODE : FILE_NODE;
    node->size = size;
    node->parent = NULL;
    node->siblings = NULL;
    node->children = NULL;
    return node;
}
struct node *add_node(struct node *parent, char *name, long size) {
    struct node *node = new_node(name, size);
    node->parent = parent;
    if(node->parent->children == NULL) {
        node->parent->children = node;
    } else {
        struct node *last_child = node->parent->children;
        while(last_child->siblings != NULL) {
            if(!strcmp(last_child->name, name)) {
                free(node);
                return last_child;
            }
            last_child = last_child->siblings;
        }
        last_child->siblings = node;
    }
    return node;
}

struct node *change_dir(struct node *file_system, struct node *current, char *name) {
    if(current == NULL)
        return NULL;
    if(current->type != DIR_NODE) return NULL;
    if(!strcmp(name, "/")) return file_system;
    if(!strcmp(name, "..")) return current->parent;
    struct node *node = current->children;
    while(node != NULL) {
        if(!strcmp(node->name, name)) {
            return node;
        }
        node = node->siblings;
    }
    return NULL;
}

long compute_size(struct node *node) {
    long size = 0;
    if(node->size != -1)
        return node->size;
    struct node *child = node->children;
    while(child) {
        size += compute_size(child);
        child = child->siblings;
    }
    node->size = size;
    return node->size;
}

long sum_dir_at_most(struct node *node, long limit) {
    long sum = 0;
    while(node != NULL) {
        if(node->type == DIR_NODE) {
            if(node->size <= limit) {
                sum += node->size;
            }
        }
        sum += sum_dir_at_most(node->children, limit);
        node = node->siblings;
    }
    return sum;
}

void find_smallest_at_least(struct node *node, long limit, long *min) {
    while(node != NULL) {
        if(node->type == DIR_NODE) {
            if((node->size >= limit) && (node->size < *min)) {
                    *min = node->size;
            }
        }
        find_smallest_at_least(node->children, limit, min);
        node = node->siblings;
    }
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

    puzzleFile = fopen(argv[1], "r");
    if(puzzleFile == NULL) {
        fprintf(stderr, "can't open file %s\n", argv[1]);
        return 1;
    }
    struct node *file_system = new_node("/", -1);
    struct node *current = file_system;
    while(fgets(line, LINE_MAX, puzzleFile)) {
        line[strcspn(line, "\n")] = 0;
        const char *sep = " ";
        char *first = strtok(line, sep);
        if(!strcmp(first, "$")) {
            char *command = strtok(NULL, sep);
            if (!strcmp(command, "cd")) {
                char *directory = strtok(NULL, sep);
                current = change_dir(file_system, current, directory);
            }
            else {
                if(strcmp(command, "ls")) {
                    fprintf(stderr, "error: unknown command %s\n", command);
                    return 1;
                }
            }
        }else {
                if(!strcmp(first, "dir")) {
                    char *directory = strtok(NULL, sep);
                    add_node(current, directory, -1);
                }else{
                    char *end;
                    long size = strtol(first, &end, 10);
                    char *file_name = strtok(NULL, sep);
                    add_node(current, file_name, size);
                }
            }
        }
    fclose(puzzleFile);
    compute_size(file_system->children);
    long sum = sum_dir_at_most(file_system->children, 100000);
    printf("%ld\n", sum);
    long outermost_size = file_system->children->size;
    printf("size of outermost dir: %ld\n", outermost_size);
    long unused_space = 70000000L - outermost_size; 
    printf("unused space:          %ld\n", unused_space);
    long delete_at_least = 30000000L - unused_space;
    printf("delete at least:       %ld\n", delete_at_least);
    long min = LONG_MAX;
    find_smallest_at_least(file_system->children, delete_at_least, &min);
    printf("smallest at least:     %ld\n", min);
    destroy_node(file_system);
    return 0;
}
