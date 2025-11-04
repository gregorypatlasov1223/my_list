#ifndef LIST_H_
#define LIST_H_

#include <time.h>
#include <stdio.h>
#include <sys/types.h>

#include "list_error_types.h"

typedef double type_of_data;

struct element_in_list
{
    double data;
    int    next;
    int    prev;
};

struct doubly_linked_list
{
    element_in_list* array;
    ssize_t free;
    ssize_t size;
    ssize_t capacity;
};

const int FICTIVE_ELEMENT_INDEX          = 0;
const int CAPACITY_INCREASE_COEFFICIENT  = 2;
const int MAX_LENGTH_OF_FILENAME         = 256;
const int MAX_LENGTH_OF_SYSTEM_COMMAND   = 512;
const type_of_data POISON                = 525252;

// 1. вспомогательные функции
int element_is_free(doubly_linked_list* list, int index);
const char* get_status_of_element(doubly_linked_list* list, int index);
// 2. основные операции со списком
list_type_error list_realloc(doubly_linked_list* list, ssize_t new_capacity);
list_type_error list_constructor_with_specified_capacity(doubly_linked_list* ptr_list_struct, ssize_t capacity);
list_type_error list_destructor(doubly_linked_list* ptr_list_struct);
list_type_error insert_after_element(doubly_linked_list* list, int target_index, int value);
list_type_error insert_before_head(doubly_linked_list* list, int value);
list_type_error insert_after_tail(doubly_linked_list* list, int value);
list_type_error list_delete_element(doubly_linked_list* list, int index);
ssize_t get_index_of_head(doubly_linked_list* list);
ssize_t get_index_of_tail(doubly_linked_list* list);

// 3. функции дампа
void write_dump_header(FILE* htm_file, time_t now);
void write_information_about_list(FILE* htm_file, doubly_linked_list* list);
void write_elements_in_table(FILE* htm_file, doubly_linked_list* list);
list_type_error create_graph_visualization(doubly_linked_list* list, FILE* htm_file, const char* folder_name, time_t now);
list_type_error list_dump_to_htm(doubly_linked_list* list, FILE* htm_file, const char* folder_name);
list_type_error list_dump(doubly_linked_list* list, const char* filename);

// 4. dot-функции
void create_dot_nodes(doubly_linked_list* list, FILE* dot_file);
void create_invisible_element_connections(doubly_linked_list* list, FILE* dot_file);
void create_normal_element_connections(doubly_linked_list* list, FILE* dot_file);
void create_free_element_connections(doubly_linked_list* list, FILE* dot_file);
list_type_error create_dot_file(doubly_linked_list* list, const char* filename);

// 5. логирование
list_type_error initialization_of_list_log(const char* filename);
list_type_error close_list_log(const char* filename);

// 6. верификации
const char* verify_result_translator(verify_result result);
verify_result detect_cycle(doubly_linked_list* list);
verify_result verify_list(doubly_linked_list* list);

#endif // LIST_H_
