#include <stdlib.h>
#include <stdio.h>
#include "list.h"

void free_string(void * string) {
    if (string == NULL) {
        return;
    }

    free((char*)string);
}

void print_key(unsigned int key) {
    printf("Key: %c\n", (char)key);
}

int test_append_multiple() {
    char * string_1 = (char*)malloc(32 * sizeof(char));
    string_1[0] = 'A';
    char * string_2 = (char*)malloc(32 * sizeof(char));
    string_2[0] = 'B';
    char * string_3 = (char*)malloc(32 * sizeof(char));
    string_3[0] = 'C';
    char * string_4 = (char*)malloc(32 * sizeof(char));
    string_4[0] = 'D';
    char * string_5 = (char*)malloc(32 * sizeof(char));
    string_5[0] = 'E';

    List * l = new_list(free_string);

    append_list(l, string_1, (int)'A');
    append_list(l, string_2, (int)'B');
    append_list(l, string_3, (int)'C');
    append_list(l, string_4, (int)'D');
    append_list(l, string_5, (int)'E');

    int result = get_list(l, (int)'A') && get_list(l, (int)'B') && get_list(l, (int)'C') && get_list(l, (int)'D') && get_list(l, (int)'E');

    free_list(l);

    return result;
}

int test_insert_multiple() {
    char * string_1 = (char*)malloc(32 * sizeof(char));
    string_1[0] = 'A';
    char * string_2 = (char*)malloc(32 * sizeof(char));
    string_2[0] = 'B';
    char * string_3 = (char*)malloc(32 * sizeof(char));
    string_3[0] = 'C';
    char * string_4 = (char*)malloc(32 * sizeof(char));
    string_4[0] = 'D';
    char * string_5 = (char*)malloc(32 * sizeof(char));
    string_5[0] = 'E';

    List * l = new_list(free_string);

    insert_list(l, string_1, (int)'A');
    insert_list(l, string_2, (int)'B');
    insert_list(l, string_3, (int)'C');
    insert_list(l, string_4, (int)'D');
    insert_list(l, string_5, (int)'E');

    int result = get_list(l, (int)'A') && get_list(l, (int)'B') && get_list(l, (int)'C') && get_list(l, (int)'D') && get_list(l, (int)'E');

    free_list(l);

    return result;
}

void test_foreach_multiple() {
    char * string_1 = (char*)malloc(32 * sizeof(char));
    string_1[0] = 'A';
    char * string_2 = (char*)malloc(32 * sizeof(char));
    string_2[0] = 'B';
    char * string_3 = (char*)malloc(32 * sizeof(char));
    string_3[0] = 'C';
    char * string_4 = (char*)malloc(32 * sizeof(char));
    string_4[0] = 'D';
    char * string_5 = (char*)malloc(32 * sizeof(char));
    string_5[0] = 'E';

    List * l = new_list(free_string);

    append_list(l, string_1, (int)'A');
    append_list(l, string_2, (int)'B');
    append_list(l, string_3, (int)'C');
    append_list(l, string_4, (int)'D');
    append_list(l, string_5, (int)'E');

    foreach_list(l, print_key);

    free_list(l);
}



int main() {
    printf(test_append_multiple() ? "TEST append_multiple: PASS\n" : "TEST append_multiple: FAIL\n");
    printf(test_insert_multiple() ? "TEST insert_multiple: PASS\n" : "TEST insert_multiple: FAIL\n");
    printf("TEST foreach_list: EXPECTING A, B, C, D, E\n");
    test_foreach_multiple();
}