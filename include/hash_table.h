#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

typedef struct HashTable HashTable;

HashTable * new_hash_table(void (*free_callback)(void *));
void free_hash_table(HashTable *ht);
void insert_hash_table(HashTable * ht, unsigned int key, void *value);
void remove_hash_table(HashTable * ht, unsigned int key);
void * get_hash_table(HashTable * ht, unsigned int key);

#endif