#include <stdio.h>
#include <stdlib.h>

#include "hash_table.h"
#include "list.h"

#define DEFAULT_TABLE_SIZE 50

typedef struct HashTable HashTable;

struct HashTable {
    List **entries;

    int size;

    void (*free_callback)(void *);
};

HashTable * new_hash_table(void (*free_callback)(void *)) {
    HashTable *new = (HashTable*)malloc(sizeof(HashTable));

    new->entries = (List **)malloc(DEFAULT_TABLE_SIZE * sizeof(List *));
    new->size = DEFAULT_TABLE_SIZE;
    new->free_callback = free_callback;

    for (int i = 0; i < DEFAULT_TABLE_SIZE; i++) {
        new->entries[i] = NULL;
    }

    return new;
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