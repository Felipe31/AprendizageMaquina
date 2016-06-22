#include "database.h"
#include "functions_decision_tree_id3.h"
#include "decision_tree_id3.h"
#include "list_insertion_sort.h"

// RECEIVE:
//      a database, a struct my_data node_data, a node_atribute, a node_value,
//      a node_entropy and a node_information_gain
// RETURN:
//      if ok then
//          a node of the type struct node *
//      else
//          NULL
// PARAMETERS:
//      data = database
//      node_data = array with her tuples of the database
//      node_atribute = an atribute
//      node_value = a value of the atribute
//      node_class = the class of the node indicating he is a leaf
//      node_entropy = the entropy of the node
//      node_information_gain = the information gain of the node
struct node *create_node(struct database *data, struct my_data *node_data, int node_atribute, double node_value, double node_class, double node_entropy, double node_information_gain) {
    assert(data);
    assert(data->atributes);
    assert(node_data);
    assert(node_data->my_data_array);
    if(node_data->size_my_data_array > data->size_database) return NULL;
    if(node_data->size_my_data_array < 0) return NULL;

    struct node *node = (struct node *)calloc(1, sizeof(struct node));
    assert(node);

    node->data = data;
    node->node_data = node_data;
    node->node_atribute = node_atribute;
    node->node_value = node_value;
    node->node_class = node_class;
    node->node_entropy = node_entropy;
    node->node_information_gain = node_information_gain;
    node->next = NULL;
    node->children = NULL;

    return node;
}

// RECEIVE:
//      a database
// RETURN:
//      if ok then
//          a node of the type struct node * representing the root of the tree
//          automatically set the atribute how 'used' on the database
//      else
//          NULL
// PARAMETERS:
//      data = database
// PRECAUTIONS:
//      XXX RETURN DYNAMIC ALLOCATION
struct node *create_root(struct database *data) {
    assert(data);
    assert(data->atributes);

    struct my_data *my_data = (struct my_data *)calloc(1, sizeof(struct my_data));
    assert(my_data);

    my_data->my_data_array = (int *)calloc(data->size_database, sizeof(int));
    assert(my_data->my_data_array);
    my_data->size_my_data_array = data->size_database;

    int i;
    for(i=0; i < data->size_database; i++) {
        my_data->my_data_array[i] = i;
    }

    double _entropy = entropy(data, my_data);

    struct node *root = create_node(data, my_data, -1, -1, -1, _entropy, -1);
    assert(root);

    double gain_children[data->quantity_atributes];
    double bigger=0;
    int indice=0;
    for(i=0; i < data->quantity_atributes - 1; i++) {
        gain_children[i] = information_gain(_entropy, data, my_data, i);
        if(gain_children[i] > bigger) {
            bigger = gain_children[i];
            indice = i;
        }
    }

    root->node_information_gain = bigger;
    root->node_atribute = indice;

    data->ids[indice].status = '1';

    return root;
}

// RECEIVE:
//      a node that is the parent and a list of nodes still undefined
// RETURN:
//      if ok then
//          a node of the type struct node * representing the start of a list
//          with the children
//      else
//          NULL
// PARAMETERS:
//      parent = node parent
//      list = a list of undefined nodes
// PRECAUTIONS:
//      XXX RETURN DYNAMIC ALLOCATION
//      uses free_list, add_sorted_list and add from list_insertion_sort
struct node *create_children(struct node *parent, struct node_list **list) {
    assert(parent);
    assert(parent->data);
    assert(parent->data->atributes);
    assert(parent->node_data);
    assert(list);

    struct values_found *values_found = atribute_values(parent->data, parent->node_data, parent->node_atribute);
    assert(values_found);

    struct position_values *position_value = (struct position_values *)calloc(1, sizeof(struct position_values));
    assert(position_value);

    position_value->values_found = values_found;
    position_value->atribute = parent->node_atribute;

    position_values(parent->data, parent->node_data, position_value);
    assert(position_value->positions);

    if(values_found->size_values_found > 1) {
        struct my_data *array = (struct my_data*)calloc(1, sizeof(struct my_data));
        assert(array);
        array->my_data_array = position_value->positions[0].my_data_array;
        array->size_my_data_array = position_value->positions[0].size_my_data_array;

        parent->children = create_node(parent->data, array, parent->node_atribute, values_found->values[0], -1, entropy(parent->data, array), -1);
        assert(parent->children);
        parent->children->parent = parent;
        add_sorted_list(list, parent->children);

        struct node *_list = parent->children;
        int i;
        for(i=1; i < position_value->size_positions; i++) {
            struct my_data *aux_array = (struct my_data*)calloc(1, sizeof(struct my_data));
            assert(aux_array);
            aux_array->my_data_array = position_value->positions[i].my_data_array;
            aux_array->size_my_data_array = position_value->positions[i].size_my_data_array;

            _list->next = create_node(parent->data, aux_array, parent->node_atribute, values_found->values[i], -1, entropy(parent->data, aux_array), -1);
            assert(_list->next);
            _list->next->parent = parent;
            add_sorted_list(list, _list->next);
            _list = _list->next;
        }
    }

    free(values_found->values);
    free(values_found);
    free(position_value->positions);
    free(position_value);

    return parent->children;
}

// RECEIVE:
//      a node that is a head of a list
// RETURN: nothing
// PARAMETERS:
//      list = node head of a list
void free_list(struct node *list) {
    if(list == NULL) return;

    struct node *aux = list;

    while(aux) {
        aux = aux->next;
        free(list->node_data->my_data_array);
        free(list->node_data);
        free(list);
        list = aux;
    }
}

// RECEIVE:
//      a node that is a head of a list of the type struct node_list
//      and a node of the tree
// RETURN: nothing, but calculate the information gain of the node and set him
//         and add him to the list of the type struct node_list
// PARAMETERS:
//      list = node head of a list of the type struct node_list
//      node = node of the tree
void add_sorted_list(struct node_list **list, struct node *node) {
    assert(list);
    assert(node);
    assert(node->data);
    assert(node->node_data);

    if(node->node_entropy == 0.0) {
        node->node_class = node->data->atributes[node->node_data->my_data_array[0]][node->data->quantity_atributes-1].value;
        return;
    }

    double gain_children[node->data->quantity_atributes];
    double bigger=-1;
    int indice=0, i;
    for(i=0; i < node->data->quantity_atributes -1; i++) {
        if(node->data->ids[i].status == '1') continue;
        gain_children[i] = information_gain(node->node_entropy, node->data, node->node_data, i);
        if(gain_children[i] > bigger) {
            bigger = gain_children[i];
            indice = i;
        }
    }

    node->node_information_gain = bigger;
    node->node_atribute = indice;

    node->data->ids[indice].status = '1';

    add(list, node);
}

// RECEIVE:
//      the database
// RETURN:
//      a node representing the root of the decision tree created
// PARAMETERS:
//      data = database
// PRECAUTIONS:
//      RETURNS DYNAMIC ALLOCATION
//      uses create_root, create_children and pop(list_insertion_sort)
struct node *create_tree(struct database *data) {
    assert(data);
    assert(data->atributes);

    struct node *root = create_root(data);
    assert(root);
    assert(root->node_data);
    assert(root->data);
    assert(root->node_data->my_data_array);

    struct node_list *_list = NULL;
    create_children(root, &_list);
    struct node *node = pop(&_list);
    assert(node);

    while(node) {
        create_children(node, &_list);
        node = pop(&_list);
        if(node == NULL) break;
    }

    return root;
}

// RECEIVE:
//      the root of the tree
// RETURN: nothing
// PARAMETERS:
//      root = root of the decision tree
void print_tree(struct node *root) {
    if(root == NULL) return;

    int i;
    //if(root->node_class != -1.0) {
        printf("a:%d, c:%f: ", root->node_atribute, root->node_class);
        for(i=0; i < root->node_data->size_my_data_array; i++) {
            printf("%d, ", root->node_data->my_data_array[i]);
        }
        printf("\n");
    //}

    print_tree(root->children);
    print_tree(root->next);
}

// RECEIVE:
//      the root of a decision tree
// RETURN: nothing
// PARAMETERS:
//      root = root of a decision tree
void free_tree(struct node *root) {
    if(root == NULL) return;

    free_tree(root->next);
    free_tree(root->children);

    free(root->node_data->my_data_array);
    free(root->node_data);
    free(root);
}

// RECEIVE:
//      a list and a value to search for
// RETURN:
//      a node that has the value
// PARAMETERS:
//      list = a list of the type struct node
//      value = a value to search in the list
struct node *search_list(struct node *list, double value) {
    if(list == NULL) return NULL;

    struct node *aux = list;
    while(aux) {
        if(aux->node_value == value) {
            break;
        }
        aux = aux->next;
    }
    return aux;
}

// RECEIVE:
//      a root of a tree and an tuple(array) to search for
// RETURN:
//      the value of the class on the decision tree
// PARAMETERS:
//      root = the root of the tree
//      array = an array representing a tuple with the values
double search(struct node *root, double *array) {
    assert(array);

    if(root->node_class == -1) {
        struct node *aux = search_list(root->children, array[root->node_atribute]);
        if(aux == NULL) {
            return root->node_value;
        } else {
            return search(aux, array);
        }
    }
    return root->node_class;
}
