#include "list_insertion_sort.h"

int main(void) {

    struct node *n1 = (struct node*)calloc(1, sizeof(struct node));
    assert(n1);
    n1->node_information_gain = 2;
    struct node *n2 = (struct node*)calloc(1, sizeof(struct node));
    assert(n2);
    n2->node_information_gain = 6;
    struct node *n3 = (struct node*)calloc(1, sizeof(struct node));
    assert(n3);
    n3->node_information_gain = 1.5;
    struct node *n4 = (struct node*)calloc(1, sizeof(struct node));
    assert(n4);
    n4->node_information_gain = 1.2;

    struct node_list *list = NULL;

    assert(add(&list, n1));
    assert(add(&list, n2));
    assert(add(&list, n3));
    assert(add(&list, n4));

    print_list(&list);

    free(pop(&list));
    free(pop(&list));
    free(pop(&list));
    free(pop(&list));

    print_list(&list);

    return 0;
}
