#ifndef _LIST_INSERTION_SORT
#define _LIST_INSERTION_SORT

#include <stdio.h>
#include <stdlib.h>
#include "database.h"
#include "functions_decision_tree_id3.h"
#include "decision_tree_id3.h"

struct node_list {
    struct node_list *next;
    struct node *node;
};

struct node_list *add(struct node_list **list, struct node *new_node);

struct node *pop(struct node_list **list);

int is_empty(struct node_list **list);

void print_list(struct node_list **list);

#endif
