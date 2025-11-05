// #include "list_tests.h"

#include <assert.h>
#include <stdio.h>

#include "list.h"
#include "list_tests.h"

//Вставка первого элемента в пустой список
void test_first_insert()
{
    doubly_linked_list list = {};
    list_constructor_with_specified_capacity(&list, 5);

    const char* folder_name = "log_TestFirstInsert";
    initialization_of_list_log(folder_name);

    list_dump(&list, folder_name);
    list_type_error result = list_insert_the_first_element(&list, 100);
    list_dump(&list, folder_name);

    close_list_log(folder_name);
    list_destructor(&list);
}

//Вставка нескольких элементов и удаление среднего
void test_multiple_insertions_and_delete_middle()
{
    doubly_linked_list list = {};
    list_constructor_with_specified_capacity(&list, 7);

    const char* folder_name = "log_TestMultipleInsertionsAndDeleteMiddle";
    initialization_of_list_log(folder_name);

    list_insert_the_first_element(&list, 10);
    list_dump(&list, folder_name);

    insert_after_element(&list, 1, 20);
    list_dump(&list, folder_name);

    insert_after_element(&list, 2, 30);
    list_dump(&list, folder_name);

    insert_after_element(&list, 3, 40);
    list_dump(&list, folder_name);

    list_delete_element(&list, 2);
    list_dump(&list, folder_name);

    insert_after_element(&list, 4, 50);
    list_dump(&list, folder_name);

    close_list_log(folder_name);
    list_destructor(&list);
}

// Удаление головы
void test_delete_head()
{
    doubly_linked_list list = {};
    list_constructor_with_specified_capacity(&list, 10);

    const char* folder_name = "log_TestDeleteHead";
    initialization_of_list_log(folder_name);

    list_insert_the_first_element(&list, 10);
    insert_after_element(&list, 1, 20);
    insert_after_element(&list, 2, 30);

    list_dump(&list, folder_name);
    list_type_error result = list_delete_element(&list, 1);
    list_dump(&list, folder_name);

    close_list_log(folder_name);
    list_destructor(&list);
}

// Удаление хвоста
void test_delete_tail()
{
    doubly_linked_list list = {};
    list_constructor_with_specified_capacity(&list, 10);

    const char* folder_name = "log_TestDeleteTail";
    initialization_of_list_log(folder_name);

    list_insert_the_first_element(&list, 10);
    insert_after_element(&list, 1, 20);
    insert_after_element(&list, 2, 30);

    list_dump(&list, folder_name);
    list_type_error result = list_delete_element(&list, 3);
    list_dump(&list, folder_name);

    close_list_log(folder_name);
    list_destructor(&list);
}

// Удаление последнего оставшегося элемента
void test_delete_last_remaining_element()
{
    doubly_linked_list list = {};
    list_constructor_with_specified_capacity(&list, 5);

    const char* folder_name = "log_TestDeleteLastRemainingElement";
    initialization_of_list_log(folder_name);

    list_insert_the_first_element(&list, 100);
    list_dump(&list, folder_name);

    list_type_error result = list_delete_element(&list, 1);
    list_dump(&list, folder_name);

    close_list_log(folder_name);
    list_destructor(&list);
}

// Тест реаллокации
void test_reallocation()
{
    doubly_linked_list list = {};
    list_constructor_with_specified_capacity(&list, 3);

    const char* folder_name = "log_TestReallocation";
    initialization_of_list_log(folder_name);

    // Заполняем список
    list_insert_the_first_element(&list, 10);
    insert_after_element(&list, 1, 20);
    list_dump(&list, folder_name);

    list_type_error result = insert_after_element(&list, 2, 30);
    list_dump(&list, folder_name);

    close_list_log(folder_name);
    list_destructor(&list);
}

// Вставка после удаления (повторное использование памяти)
void test_reuse_after_delete()
{
    doubly_linked_list list = {};
    list_constructor_with_specified_capacity(&list, 5);

    const char* folder_name = "log_TestReuseAfterDelete";
    initialization_of_list_log(folder_name);

    list_insert_the_first_element(&list, 10);
    insert_after_element(&list, 1, 20);
    list_dump(&list, folder_name);

    int first_element_index = 1;

    list_delete_element(&list, first_element_index);
    list_dump(&list, folder_name);

    // Вставляем новый элемент на место удаленного
    insert_after_element(&list, 0, 30);
    list_dump(&list, folder_name);

    close_list_log(folder_name);
    list_destructor(&list);
}

void all_tests()
{
    test_first_insert();
    test_multiple_insertions_and_delete_middle();
    test_delete_head();
    test_delete_tail();
    test_delete_last_remaining_element();
    test_reuse_after_delete();
    test_reallocation();
}
