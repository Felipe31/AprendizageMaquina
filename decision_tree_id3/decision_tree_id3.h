#ifndef _DECISION_TREE_ID3
#define _DECISION_TREE_ID3

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include "database.h"
#include "functions_decision_tree_id3.h"
#include "list_insertion_sort.h"

// this is a tree with a circular list where, each node has a subtree of nodes
struct node {
    struct database *data; // database
    struct my_data *node_data;
    int node_atribute; // 0, 1, 2, ... number of the atribute(column)
    double node_value; // value of the atribute(line) -1 = is not a leaf
    double node_class; // value of the class(answer) -1 = is not a leaf
    double node_entropy;
    double node_information_gain;
    struct node *next; // to be used in the list
    struct node *children; // to be used by the tree
    struct node *parent;
};

struct node *create_node(struct database *data, struct my_data *node_data, int node_atribute, double node_value, double node_class, double node_entropy, double node_information_gain);

struct node *create_root(struct database *data);

struct node *create_children(struct node *parent, struct node_list **list);

void free_list(struct node *list);

void add_sorted_list(struct node_list **list, struct node *node);

struct node *create_tree(struct database *data);

void print_tree(struct node *root);

void free_tree(struct node *root);

struct node *search_list(struct node *list, double value);

double search(struct node *root, double *array);

#endif
