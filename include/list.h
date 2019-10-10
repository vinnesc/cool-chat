typedef struct List List;

List * new_list();
void free_list(List *list);
void insert_list(List *list, int value);
void append_list(List *list, int value);