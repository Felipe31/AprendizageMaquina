
#include "list_insertion_sort.h"

struct node_list *add(struct node_list **list, struct node *new_node) {
    assert(list);
    assert(new_node);

    struct node_list *new = (struct node_list *)calloc(1, sizeof(struct node_list));
    assert(new);
    new->node = new_node;
    new->next = NULL;

    if(*list == NULL) {
        *list = new;
        return new;
    } else {
        struct node_list *aux = *list;

        if(aux->node->node_information_gain < new->node->node_information_gain) {
            new->next = *list;
            *list = new;
            return new;
        }

        struct node_list *aux2 = aux->next;
        if(aux2 == NULL) {
            aux->next = new;
            return new;
        }

        do {
            if(aux2->node->node_information_gain < new->node->node_information_gain) {
                new->next = aux2;
                aux->next = new;
                return new;
            }
            aux = aux->next;
            aux2 = aux2->next;

            if(aux2 == NULL) {
                aux->next = new;
                return new;
            }
        } while(1);
    }
}

struct node *pop(struct node_list **list) {
    assert(list);
    if(*list == NULL) return NULL;
    else {
        struct node_list *aux = *list;
        *list = aux->next;
        struct node *i = aux->node;
        free(aux);
        return i;
    }
}

int is_empty(struct node_list **list) {
    if(list == NULL) return -1;
    if(*list == NULL) return 0;
    else return 1;
}

void print_list(struct node_list **list) {
    assert(list);
    if(*list == NULL) return;

    struct node_list *aux = *list;
    do {
        printf("key: %f\n", aux->node->node_information_gain);
        aux = aux->next;
    } while(aux != NULL);
}
