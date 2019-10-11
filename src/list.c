#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "list.h"

typedef struct Node Node;
typedef struct List List;

struct Node{
    void *value;
    unsigned int key;

    Node *next;
};

struct List{
    Node *header;

    void (*free_callback)(void *);
};

static Node * new_node(void *value, unsigned int key) {
    Node *new = (Node *)malloc(sizeof(Node));
    new->value = value;
    new->next = NULL;
    new->key = key;

    return new;
}

static void free_node(Node *node, void (*free_callback)(void *)) {
    if (node == NULL) {
        return;
    }
    
    free_callback(node->value); //Will this work? LOL
    free(node);
}

List * new_list(void (*free_callback)(void *)) {
    List *new = (List *)malloc(sizeof(List));
    new->header = NULL;
    new->free_callback = free_callback;

    return new;
}

void free_list(List *list) {
    if (list == NULL) {
        return;
    }

    Node * to_free = list->header;
    Node * aux = list->header;

    while (aux != NULL) {
        aux = aux->next;

        free_node(to_free, list->free_callback);
        to_free = aux;
    }
}

void insert_list(List *list, void *value, unsigned int key) {
    if (list == NULL) {
        return;
    }

    Node * new = new_node(value, key);

    if (list->header == NULL) {
        list->header = new;
    } else {
        Node * aux = list->header;

        new->next = aux;
        list->header = new;
    }
}

void append_list(List *list, void *value, unsigned int key) {
    if (list == NULL) {
        return;
    }
    Node * new = new_node(value, key);
    
    if (list->header == NULL) {
        list->header = new;
    } else {
        Node * aux = list->header;

        while (aux->next != NULL) {
            aux = aux->next;
        }

        aux->next = new;
    }
}

void * get_list(List *list,  unsigned int key) {
    if (list == NULL) {
        return NULL;
    }

    Node * aux = list->header;

    while (aux != NULL) {
        if (aux->key == key) {
            return aux->value;
        }

        aux = aux->next;
    }

    return NULL;
}

void remove_list(List *list, unsigned int key) {
    if (list == NULL) {
        return;
    }

    if (list->header->key == key) {
        Node * tmp = list->header->next; 
        free_node(list->header, list->free_callback);
        list->header = tmp;

        return;
    }

    Node * aux = list->header;
    Node * to_delete = aux->next;

    while (to_delete != NULL) {
        if (to_delete->key == key) {
            aux->next = to_delete->next;
            free_node(to_delete, list->free_callback);
            
            return;
        }
        aux = to_delete;
        to_delete = to_delete->next;
    }
}

void foreach_list(List *list, void (*callback)(unsigned)) {
    if (list == NULL) {
        return;
    }

    Node * aux = list->header;

    while (aux != NULL) {
        callback(aux->key);

        aux = aux->next;
    }

    return;
}