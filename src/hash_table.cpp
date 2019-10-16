#include <stdio.h>
#include <stdlib.h>

#include "hash_table.hpp"
#include "list.hpp"

#define DEFAULT_TABLE_SIZE 50

typedef struct HashTable HashTable;

struct HashTable {
    List **entries;

    int size;

    void (*free_callback)(void *);
};

HashTable * new_hash_table(void (*free_callback)(void *)) {
    HashTable *new_ht = (HashTable*)malloc(sizeof(HashTable));

    new_ht->entries = (List **)malloc(DEFAULT_TABLE_SIZE * sizeof(List *));
    new_ht->size = DEFAULT_TABLE_SIZE;
    new_ht->free_callback = free_callback;

    for (int i = 0; i < DEFAULT_TABLE_SIZE; i++) {
        new_ht->entries[i] = NULL;
    }

    return new_ht;
}

void free_hash_table(HashTable *ht) {
    if (ht == NULL) {
        return;
    }

    for (int i = 0; i < ht->size; i++) {
        if (ht->entries[i] != NULL) {
            free_list(ht->entries[i]);
        }
    }

    free(ht);
}

static unsigned int hash(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

void insert_hash_table(HashTable * ht, unsigned int key, void *value) {
    unsigned int index = hash(key) % ht->size;

    if (ht->entries[index] == NULL) {
        ht->entries[index] = new_list(ht->free_callback);
    }

    append_list(ht->entries[index], value, key); //Appending or inserting???
}

void * get_hash_table(HashTable * ht, unsigned int key) {
    unsigned int index = hash(key) % ht->size;

    return get_list(ht->entries[index], key); 
}

void remove_hash_table(HashTable * ht, unsigned int key) {
    unsigned int index = hash(key) % ht->size;

    remove_list(ht->entries[index], key);
}

//TODO: Move this to variable arguments??
static void copy_to_list_hash_table(void * node, void * list, void * padding) {
    List * l = (List*)list;
    Node * n = (Node*) node;

    insert_list(l, get_value_list(n),get_key_list(n));
}

List * get_values_hash_table(HashTable * ht, List * list) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->entries[i] != NULL) {
            foreach_list(ht->entries[i], copy_to_list_hash_table, list, NULL);
        }
    }

    return list;
}