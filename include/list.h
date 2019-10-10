#ifndef LIST_H_
#define LIST_H_

typedef struct List List;

List * new_list(void (*free_callback)(void *));
void free_list(List *list);
void insert_list(List *list, void *value, unsigned int key);
void append_list(List *list, void *value, unsigned int key);
void * get_list(List *list,  unsigned int key);
void remove_list(List *list, unsigned int key);

#endif