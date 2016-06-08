#ifndef _DECISION_TREE_ID3
#define _DECISION_TREE_ID3

/*
Author: Luis Angelo Loss de Castro
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <string.h>

struct atribute {
    double value;
};

struct class {
    char *name;
    char status; // '0' = not used, '1' = used
};

struct database {
    struct atribute **atributes; // table with all the tuples
    struct class *classes; // only one tuple with the name of classes and your status
    int size_database;
    int size_classes;
};

// this is a tree with a circular list where, each node has a subtree of nodes
struct node {
    int *array; // elements selected from the database
    int class; // node class
    int class_status; // class state to be used with the class
    int result; // 0..n = answer
    struct node *next; // to be used in the list
    struct node *children; // to be used by the tree
};

double entropy(struct database *data, int *array, int size);

struct database *create_database(int size_database, int size_classes);

struct database *mount_database(const char *file_name, int size_database, int size_classes);

struct node *create_node(int class, int class_status, int result);

void add_list(struct node *list_root, struct node *new);

struct node *create_list(struct database *table, int *array, int size);

void add_children(struct node *node, struct node *new);

void free_list(struct node *root);

void free_tree(struct node *root);

void free_database(struct database *base);

struct node *create_tree(struct database *table, int *array);

#endif
