#include <stdio.h>
#include <stdlib.h>

#include "list.h"

typedef struct Node Node;
typedef struct List List;

struct Node{
    int value;
    Node *next;
};

struct List{
    Node *header;
};

List * new_list() {
    List *new = (List *)malloc(sizeof(List));
    new->header = NULL;

    return new;
}

void free_list(List *list) {
    Node * to_free = list->header;
    Node * aux = list->header;

    while (aux != NULL) {
        aux = aux->next;

        free(to_free);
        to_free = aux;
    }
}

static Node * new_node(int value) {
    Node *new = (Node *)malloc(sizeof(Node));
    new->value = value;
    new->value = NULL;

    return new;
}

static void free_node(Node *node) {
    free(node);
}

void insert_list(List *list, int value) {
    Node * new = new_node(value);

    if (list->header == NULL) {
        list->header = new;
    } else {
        Node * aux = list->header;

        new->next = aux;
        list->header = new;
    }
}

void append_list(List *list, int value) {
    Node * new = new_node(value);
    
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