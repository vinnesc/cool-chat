#ifndef LIST_H_
#define LIST_H_

typedef struct List List;
typedef struct Node Node;

List * new_list(void (*free_callback)(void *));
void free_list(List *list);
void insert_list(List *list, void *value, unsigned int key);
void append_list(List *list, void *value, unsigned int key);
void * get_list(List *list,  unsigned int key);
void remove_list(List *list, unsigned int key);
void foreach_list(List *list, void (*callback)(void *, void *, void*), void * first_parameter, void * second_parameter);
unsigned int get_key_list(Node * node);
void * get_value_list(Node * node);

#endif