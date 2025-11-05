#include "list.h"
#include "list_error_types.h"
#include <stdio.h>

int main()
{
    // 1. Инициализация лога
    initialization_of_list_log("list_dump");

    // 2. Создание списка
    doubly_linked_list my_list = {};
    list_constructor_with_specified_capacity(&my_list, 10);

    // 3. Добавление элементов (чтобы список не был пустым)
    insert_before_head(&my_list, 100);
    insert_after_tail(&my_list, 200);
    insert_after_tail(&my_list, 300);

    // 4. Создание дампа с картинкой
    list_dump(&my_list, "list_dump");

    // 5. Добавляем еще элементов и создаем еще дамп
    insert_after_tail(&my_list, 400);
    list_delete_element(&my_list, 2); // удаляем какой-то элемент

    list_dump(&my_list, "list_dump");

    // 6. Закрытие лога
    close_list_log("list_dump");

    // 7. Деструктор списка
    list_destructor(&my_list);

    printf("Дампы созданы! Проверьте файлы:\n");
    printf("- list_dump.htm - HTML файл c дампами\n");
    printf("- list_dump_dump/ - папка c SVG картинками\n");

    return 0;
}
