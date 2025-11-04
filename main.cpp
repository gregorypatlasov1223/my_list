#include <stdio.h>

#include "list.h"

int main()
{
    doubly_linked_list list = {};
    list_constructor_with_specified_capacity(&list, 7);

    insert_after_element(&list, 0, 10);
    insert_after_element(&list, 1, 20);
    insert_after_element(&list, 2, 30);
    insert_after_element(&list, 3, 40);

    // list_dump(&list, "before_delete.txt");

    list_delete_element(&list, 2);

    // list_dump(&list, "after_delete.txt");

    insert_after_element(&list, 4, 50);

    // list_dump(&list, "after_new_insert.txt");

    list_destructor(&list);

    return 0;

}

