#include <ctime>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

#include "list.h"
#include "list_error_types.h"

int element_is_free(doubly_linked_list* list, int index)
{
    if (list == NULL)
        return LIST_NULL_POINTER;

    if (index < 0 || index >= list -> capacity)
        return 0;

    ssize_t current_free_index = list -> free; // list -> free - первый свободный
    while (current_free_index != SHIT_INDEX && current_free_index >= 0 && current_free_index < list -> capacity)
    {
        if (current_free_index == index)
            return 1; // надо 1 надо поработать

        current_free_index = list -> array[current_free_index].next;
    }

    return 0;
}


list_type_error list_realloc(doubly_linked_list* list, ssize_t new_capacity)
{
    if (list == NULL)
        return LIST_NULL_POINTER;

    if (new_capacity < 0 || new_capacity <= list -> capacity)
        return LIST_WRONG_SPECIFIED_CAPACITY;

    ssize_t old_size = list -> size;

    element_in_list* new_array = (element_in_list*)realloc(list -> array, size_t(new_capacity) * sizeof(element_in_list));
    if (new_array == NULL)
        return LIST_ALLOCATION_FAILED;

    list -> array = new_array;

    ssize_t old_capacity = list -> capacity;
    for (ssize_t i = old_capacity; i < new_capacity; i++) // тут индексы совпадают с номерами ячеек?
    {
        list -> array[i].data = POISON;
        list -> array[i].prev = -1;
        if (i == new_capacity - 1)
            list -> array[i].next = FICTIVE_ELEMENT_INDEX;
        else
            list -> array[i].next = (int)(i + 1);

    }

    if (list -> free == FICTIVE_ELEMENT_INDEX)
        list -> free = old_capacity; // old_capacity подразумевает с фиктивным элементом?
    else
    {
        ssize_t last_free = list -> free;
        while (list -> array[last_free].next != FICTIVE_ELEMENT_INDEX)
            last_free = list -> array[last_free].next;

        if (last_free >= 0 && last_free < list -> capacity)
            list -> array[last_free].next = (int)old_capacity;  // не понимаю как связываем последний свободный с первым новым
    }

    list -> capacity = new_capacity;
    list -> size = old_size;

    return LIST_NO_ERROR;
}


list_type_error list_constructor_with_specified_capacity(doubly_linked_list* ptr_list_struct, ssize_t capacity)
{
    assert(ptr_list_struct != NULL);

    if (capacity < 0)
        return LIST_WRONG_SPECIFIED_CAPACITY;

    ptr_list_struct -> array = (element_in_list*)calloc((size_t)capacity, sizeof(element_in_list));
    if (ptr_list_struct -> array == NULL)
        return LIST_ALLOCATION_FAILED;

    ptr_list_struct -> capacity = capacity;

    ptr_list_struct -> array[FICTIVE_ELEMENT_INDEX].data = POISON;
    ptr_list_struct -> array[FICTIVE_ELEMENT_INDEX].next = FICTIVE_ELEMENT_INDEX;
    ptr_list_struct -> array[FICTIVE_ELEMENT_INDEX].prev = FICTIVE_ELEMENT_INDEX;

    for (ssize_t i = 1; i < capacity; i++)
    {
        ptr_list_struct -> array[i].data = POISON;
        ptr_list_struct -> array[i].prev = -1;

        if (i == capacity - 1)
            ptr_list_struct -> array[i].next = FICTIVE_ELEMENT_INDEX;
        else
            ptr_list_struct -> array[i].next = (int)(i + 1);
    }

    ptr_list_struct -> free = 1;
    ptr_list_struct -> size = 0;

    return LIST_NO_ERROR;
}


list_type_error list_destructor(doubly_linked_list* ptr_list_struct)
{
    assert(ptr_list_struct != NULL);

    if (ptr_list_struct -> array != NULL)
            free(ptr_list_struct -> array);

    ptr_list_struct -> array    = NULL;
    ptr_list_struct -> capacity = SHIT_VALUE;
    ptr_list_struct -> free     = SHIT_VALUE;
    ptr_list_struct -> size     = SHIT_VALUE;


    return LIST_NO_ERROR;
}


list_type_error insert_after_element(doubly_linked_list* list, int target_index, int value)
{
    if (list == NULL)
        return LIST_NULL_POINTER;

    if (target_index < 0 || target_index >= list -> capacity)
        return SHIT_INDEX;

    if (list -> free == 0)
    {
        list_type_error result_of_realloc = list_realloc(list, (list -> capacity) * CAPACITY_INCREASE_COEFFICIENT);
        if (result_of_realloc != LIST_NO_ERROR)
            return result_of_realloc;
    }

    ssize_t new_index = list -> free;
    list -> free = list -> array[new_index].next;

    list -> array[new_index].data = value;

    list -> array[new_index].prev = target_index;
    list -> array[new_index].next = list -> array[target_index].next;

    list -> array[target_index].next = int(new_index);
    list -> array[list -> array[new_index].next].prev = int(new_index);

    list -> size++;

    return LIST_NO_ERROR;
}


list_type_error insert_before_head(doubly_linked_list* list, int value)
{
    if (list == NULL)
        return LIST_NULL_POINTER;

    list_type_error result  = insert_after_element(list, FICTIVE_ELEMENT_INDEX, value);
    if (result != LIST_NO_ERROR)
        return result;

    return LIST_NO_ERROR;
}


list_type_error insert_after_tail(doubly_linked_list* list, int value)
{
    if (list == NULL)
        return LIST_NULL_POINTER;

    ssize_t tail_index = list -> array[FICTIVE_ELEMENT_INDEX].prev;

    list_type_error result  = insert_after_element(list, int(tail_index), value);
    if (result != LIST_NO_ERROR)
        return result;

    return LIST_NO_ERROR;
}


list_type_error list_insert_the_first_element(doubly_linked_list* list, int value)
{
    return insert_after_element(list, FICTIVE_ELEMENT_INDEX, value);
}


list_type_error list_delete_element(doubly_linked_list* list, int index)
{
    if (list == NULL)
        return LIST_NULL_POINTER;

    if (index < 1 || index >= list -> capacity)
        return SHIT_INDEX;

    element_in_list* element = &list -> array[index];

    list -> array[element -> prev].next = element -> next;
    list -> array[element -> next].prev = element -> prev;

    element -> data = POISON;
    element -> prev = -1;
    element -> next = int(list -> free);
    list -> free = index;

    list -> size--;

    return LIST_NO_ERROR;
}


ssize_t get_index_of_head(doubly_linked_list* list)
{
    return list -> array[FICTIVE_ELEMENT_INDEX].next;
}


ssize_t get_index_of_tail(doubly_linked_list* list)
{
    return list -> array[FICTIVE_ELEMENT_INDEX].prev;
}


void write_dump_header(FILE* htm_file, time_t now)
{
    fprintf(htm_file, "<div style='border:2px solid #ccc; margin:10px; padding:15px; background:#f9f9f9;'>\n"); //создает красивый контейнер для одного дампа
    fprintf(htm_file, "<h2 style='color:#333;'>List Dump at %s</h2>\n", ctime(&now)); //ctime преобразует время в формате time_t в читаемую строку
}


verify_result detect_cycle(doubly_linked_list* list)
{
    if (list -> size == 0 || list == NULL)
        return VERIFY_SUCCESS;

    ssize_t current_by_next = FICTIVE_ELEMENT_INDEX; // идем с фиктивного элемента
    ssize_t current_by_prev = FICTIVE_ELEMENT_INDEX;
    ssize_t steps = 0;

     while (steps <= list -> size)
    {
        current_by_next = list -> array[current_by_next].next;
        current_by_prev = list -> array[current_by_prev].prev;

        if (current_by_next < 0 || current_by_next >= list -> capacity ||
            current_by_prev < 0 || current_by_prev >= list -> capacity)
            return VERIFY_INVALID_INDEX;

        steps++;

        // слишком рано встретили фиктивный элемент, значит, ошибка
        if ((current_by_next == FICTIVE_ELEMENT_INDEX || current_by_prev == FICTIVE_ELEMENT_INDEX) && (steps <= list -> size))
            return VERIFY_CYCLE_DETECTED;

        // если встретили фиктивный элемент на size + 1 шаге то все ок
        if (current_by_next == FICTIVE_ELEMENT_INDEX && current_by_prev == FICTIVE_ELEMENT_INDEX && steps == list -> size + 1)
            break;

        // // надо проверить что вышли именно по брейку
        // if (current_by_next != FICTIVE_ELEMENT_INDEX || current_by_prev != FICTIVE_ELEMENT_INDEX)
        //     return VERIFY_CYCLE_DETECTED;
    }

    return VERIFY_SUCCESS;
}


verify_result verify_list(doubly_linked_list* list)
{
    if (list == NULL)
        return VERIFY_INVALID_INDEX;

    verify_result cycle_check = detect_cycle(list);
    if (cycle_check != VERIFY_SUCCESS)
        return cycle_check;

    if (list -> size > 0)
    {
        ssize_t head_index = list -> array[FICTIVE_ELEMENT_INDEX].next;
        ssize_t tail_index = list -> array[FICTIVE_ELEMENT_INDEX].prev;

        if (list -> array[tail_index].next != FICTIVE_ELEMENT_INDEX)
            return VERIFY_TAIL_NEXT_ERROR;
        if (list -> array[head_index].prev != FICTIVE_ELEMENT_INDEX)
            return VERIFY_HEAD_PREV_ERROR;

        ssize_t current = head_index; //начинаем обходить с головы
        ssize_t count = 0;

        while (count < list -> size)
        {
            ssize_t next = list -> array[current].next;
            if (next < 0 || next >= list -> capacity)
                return VERIFY_INVALID_INDEX;

            if (list -> array[next].prev != current) // проверяем двустороннюю связь
                return VERIFY_BIDIRECTIONAL_LINK_ERROR;

            current = next;
            count++;
        }

        if (current != FICTIVE_ELEMENT_INDEX)
            return VERIFY_CYCLE_DETECTED;
    }

    else
    {
        if (list -> array[FICTIVE_ELEMENT_INDEX].next != FICTIVE_ELEMENT_INDEX ||
            list -> array[FICTIVE_ELEMENT_INDEX].prev != FICTIVE_ELEMENT_INDEX)
            return VERIFY_EMPTY_LIST_LINKS_ERROR;
    }

    return VERIFY_SUCCESS;
}


const char* verify_result_translator(verify_result result)
{
    switch (result)
    {
        case VERIFY_SUCCESS:                  return "Success";
        case VERIFY_FAKE_ELEMENT_NEXT_ERROR:  return "Fake element next pointer error";
        case VERIFY_FAKE_ELEMENT_PREV_ERROR:  return "Fake element prev pointer error";
        case VERIFY_TAIL_NEXT_ERROR:          return "Tail next pointer error";
        case VERIFY_HEAD_PREV_ERROR:          return "Head prev pointer error";
        case VERIFY_CAPACITY_EXCEEDED:        return "Capacity exceeded during traversal";
        case VERIFY_INVALID_INDEX:            return "Invalid node index";
        case VERIFY_BIDIRECTIONAL_LINK_ERROR: return "Bidirectional link broken";
        case VERIFY_COUNT_MISMATCH:           return "Element count mismatch";
        case VERIFY_EMPTY_LIST_LINKS_ERROR:   return "Empty list links error";
        case VERIFY_CYCLE_DETECTED:           return "Detected cycle in list";
        default:                              return "Unknown error";
    }
}


void write_information_about_list(FILE* htm_file, doubly_linked_list* list)
{
    fprintf(htm_file, "<div style='margin-bottom:15px;'>\n"); //margin -- внешний отступ, padding -- внутренний
    fprintf(htm_file, "<p><b>Capacity:</b> %ld</p>\n", list -> capacity);
    fprintf(htm_file, "<p><b>Size:</b> %ld</p>\n", list -> size);
    fprintf(htm_file, "<p><b>Free head:</b> %ld</p>\n", list -> free);
    fprintf(htm_file, "<p><b>Head index:</b> %ld</p>\n", list -> array[FICTIVE_ELEMENT_INDEX].next);
    fprintf(htm_file, "<p><b>Tail index:</b> %ld</p>\n", list -> array[FICTIVE_ELEMENT_INDEX].prev);

    verify_result result = verify_list(list);
    const char* verify_result_in_string = verify_result_translator(result);
    const char* verify_colour = (result == VERIFY_SUCCESS) ? "green" : "red";
    fprintf(htm_file, "<p><b>Verify result:</b> <span style='color:%s; font_weight: bold;'>%s</span></p>\n",
            verify_colour, verify_result_in_string);

    fprintf(htm_file, "</div>\n");
}


const char* get_status_of_element(doubly_linked_list* list, int index)
{
    if (index == 0)
        return "FICTIVE";

    else if (index == list -> array[FICTIVE_ELEMENT_INDEX].next && index == list -> array[FICTIVE_ELEMENT_INDEX ].prev)
        return "HEAD / TAIL";

    else if (index == list -> array[FICTIVE_ELEMENT_INDEX].next)
        return "HEAD";

    else if (index == list -> array[FICTIVE_ELEMENT_INDEX].prev)
        return "TAIL";

    else if (!element_is_free(list, index))
        return "USED";

    else
        return "FREE";

}


void write_elements_in_table(FILE* htm_file, doubly_linked_list* list)
{
    fprintf(htm_file, "<table border='1' style='border-collapse:collapse; width:100%%; margin-top:15px;'>\n"); //collapse -- убирает двойные линии в ячйеках
    fprintf(htm_file, "<tr><th>Index</th><th>Data</th><th>Next</th><th>Prev</th><th>Status</th></tr>\n");

    for (ssize_t i = 0; i < list -> capacity; i++)
    {
        element_in_list* element = &list -> array[i];
        const char* status = get_status_of_element(list, int(i));

        fprintf(htm_file, "<tr><td>%ld</td><td>%d</td><td>%ld</td><td>%ld</td><td>%s</td></tr>\n",
                i, element -> data, element -> next, element -> prev, status);
    }

    fprintf(htm_file, "</table>\n");
}


void create_dot_nodes(doubly_linked_list* list, FILE* dot_file)
{
    for (ssize_t i = 0; i < list -> capacity; i++)
    {
        element_in_list* element = &list -> array[i];
        const char* colour = "lightgreen";
        const char* label  = "FREE";                     // где будет отображаться label
        int is_free = element_is_free(list, int(i));


        if (i == FICTIVE_ELEMENT_INDEX)
        {
            colour = "lightcoral";
            label  = "FICTIVE";
        }

        else if (i == list -> array[FICTIVE_ELEMENT_INDEX].prev && i == list -> array[FICTIVE_ELEMENT_INDEX].next)
        {
            colour = "orange";
            label  = "HEAD / TAIL";
        }

        else if (i == list -> array[FICTIVE_ELEMENT_INDEX].next)
        {
            colour = "lightblue";
            label  = "HEAD";
        }

        else if (i == list -> array[FICTIVE_ELEMENT_INDEX].prev)
        {
            colour = "lightyellow";
            label  = "TAIL";
        }

        else if (!is_free)
        {
            colour = "white";
            label  = "USED";
        }

        fprintf(dot_file, "    element%ld [label=\"{%s|{idx: %ld|data: %d|next: %ld|prev: %ld}}\", style=filled, fillcolor=%s, color=black];\n", //Внешние фигурные скобки создают основную таблицу символ | между элементами разделяет строки таблицы; Внутренние фигурные скобки создают вложенные таблицы/ячейки Символ | внутри внутренних скобок разделяет столбцы внутри строки
                i, label, i, element -> data, element -> next, element -> prev, colour);

        }

        fprintf(dot_file, "\n");
}


void create_invisible_element_connections(doubly_linked_list* list, FILE* dot_file)
{
    for (ssize_t i = 0; i < list -> capacity -1; i++) // почему -1
        fprintf(dot_file, "element%ld -> element%ld [weight=100000, style=invis, color=white];\n", i, i + 1);
}


void create_normal_element_connections(doubly_linked_list* list, FILE* dot_file)
{
    for (ssize_t i = 0; i < list -> capacity; i++)
    {
        element_in_list* element = &list -> array[i];

        if (element_is_free(list, int(i)))
            continue;

        ssize_t next  = element -> next;
        ssize_t prev  = element -> prev;

        if (next >= 0 && next < list -> capacity)
        {
            if (list -> array[next].prev == i)
                 fprintf(dot_file, "    element%ld -> element%ld [color=black, constraint=false, arrowhead=normal, arrowtail=normal, dir=both];\n", i, next);
            else
            {
                fprintf(dot_file, "    element%ld -> element%ld [color=blue, label=\"next\", constraint=false];\n", i, next);

                fprintf(dot_file, "    error_prev_%ld [shape=ellipse, style=filled, fillcolor=orange, label=\"Prev Error: element%ld->next=%ld\\nbut element%ld->prev=%ld\"];\n",
                        i, i, next, next, list -> array[next].prev);
                fprintf(dot_file, "    error_prev_%ld -> element%ld [color=red, style=dashed];\n", i, next);
            }
        }

        if (prev >= 0 && prev < list -> capacity && prev != -1)
        {
            if (list -> array[prev].next != i)
            {
                 fprintf(dot_file, "    element%ld -> element%ld [color=red, label=\"prev\", constraint=false];\n", i, prev);

                fprintf(dot_file, "    error_next_%ld [shape=ellipse, style=filled, fillcolor=yellow, label=\"Next Error: element%ld->prev=%ld\\nbut element%ld->next=%ld\"];\n",
                        i, i, prev, prev, list -> array[prev].next);
                fprintf(dot_file, "    error_next_%ld -> element%ld [color=blue, style=dashed];\n", i, prev);
            }

        }

    }

}


void create_free_element_connections(doubly_linked_list* list, FILE* dot_file)
{
    fprintf(dot_file, "\n");

    ssize_t free_index = list -> free;
    while (free_index != 0 && free_index < list -> capacity)
    {
        element_in_list* element = &list -> array[free_index];
        if (element -> next != 0)
            fprintf(dot_file, "    element%ld -> element%ld [color=green, label=\"free\", constraint=false];\n", free_index, element -> next);
        free_index = element -> next;
    }
}


list_type_error create_dot_file(doubly_linked_list* list, const char* filename)
{
    assert(filename != NULL);
    if (list == NULL)
        return LIST_NULL_POINTER;

    FILE* dot_file = fopen(filename, "w");
    if (dot_file == NULL)
        return LIST_OPENING_FILE_ERROR;

    fprintf(dot_file, "digraph DoublyLinkedList {\n");
    fprintf(dot_file, "    rankdir=LR;\n");
    fprintf(dot_file, "    node [shape=Mrecord, color = black];\n\n");

    create_dot_nodes(list, dot_file);
    create_invisible_element_connections(list, dot_file);
    create_normal_element_connections(list, dot_file);
    create_free_element_connections(list, dot_file);

    fprintf(dot_file, "free_ptr [shape=plaintext, label=\"free\"];\n");

    if (list -> free != 0)
        fprintf(dot_file, "free_ptr -> element%ld [color=green];\n", list -> free);

    fprintf(dot_file, "}\n");
    fclose(dot_file);

    return LIST_NO_ERROR;
}


list_type_error create_graph_visualization(doubly_linked_list* list, FILE* htm_file, const char* folder_name, time_t now)
{
    int number_of_pictures = 0;

    char temp_dot[MAX_LENGTH_OF_FILENAME] = {};
    char temp_svg[MAX_LENGTH_OF_FILENAME] = {};

    snprintf(temp_dot, sizeof(temp_dot), "%s/temp_%d%ld.dot",folder_name, number_of_pictures, now);
    snprintf(temp_svg, sizeof(temp_svg), "%s/temp_%d%ld.svg",folder_name, number_of_pictures, now);
    number_of_pictures++;

    list_type_error dot_result = create_dot_file(list, temp_dot);
    if (dot_result != LIST_NO_ERROR)
        return dot_result;

    char command[MAX_LENGTH_OF_SYSTEM_COMMAND * COEF] = {};
    snprintf(command, sizeof(command), "dot -Tsvg %s -o %s", temp_dot, temp_svg); // преобразует DOT-файл в SVG-изображение
    int result = system(command);

    if (result == 0)
    {
        fprintf(htm_file, "<div style='text-align:center;'>\n");
        fprintf(htm_file, "<img src='%s' style='max-width:100%%; border:1px solid #ddd;'>\n", temp_svg);
        fprintf(htm_file, "</div>\n");
    }

    else
        fprintf(htm_file, "<p style='color:red;'>Error generating SVG graph</p>\n");

    remove(temp_dot);

    return LIST_NO_ERROR;
}


list_type_error list_dump_to_htm(doubly_linked_list* list, FILE* htm_file, const char* folder_name)
{
    time_t now = time(NULL); // time_t позволяет точно фиксировать момент создания дампа / time(NULL) возвращает текущее время

    write_dump_header(htm_file, now);

    write_information_about_list(htm_file, list);

    create_graph_visualization(list, htm_file, folder_name, now);

    write_elements_in_table(htm_file, list);

    fprintf(htm_file, "</div>\n\n");

    return LIST_NO_ERROR;
}


list_type_error list_dump(doubly_linked_list* list, const char* filename)
{
    assert(list     != NULL);
    assert(filename != NULL);

    char folder_name[MAX_LENGTH_OF_FILENAME] = {};
    snprintf(folder_name, sizeof(folder_name), "%s_dump", filename);

    char command[MAX_LENGTH_OF_SYSTEM_COMMAND] = {};
    //Флаг -p гарантирует, что папка создастся даже если родительские директории не существуют
    snprintf(command, sizeof(command), "mkdir -p %s", folder_name); // mkdir - это команда для создания директорий в OC
    system(command);

    char htm_filename[MAX_LENGTH_OF_FILENAME] = {};
    snprintf(htm_filename, sizeof(htm_filename), "%s.htm", filename);

    FILE* htm_file = fopen(htm_filename, "a"); // "a" означает режим добавления
    if (htm_file == NULL)
        return LIST_OPENING_FILE_ERROR;

    list_type_error result = list_dump_to_htm(list, htm_file, folder_name);

    fclose(htm_file);

    return result;
}


list_type_error initialization_of_list_log(const char* filename)
{
    char htm_filename[MAX_LENGTH_OF_FILENAME] = {};
    snprintf(htm_filename, sizeof(htm_filename), "%s.htm", filename);

    FILE* htm_file = fopen(htm_filename, "w");
    if (htm_file == NULL)
        return LIST_OPENING_FILE_ERROR;

    fprintf(htm_file, "<!DOCTYPE htm>\n"
                      "<htm>\n"
                      "<head>\n"
                      "<title>List Dump Log</title>\n" //то, что отображается во вкладке браузера
                      "<style>\n"
                      "body { font-family: Arial, sans-serif; margin: 20px; }\n"
                      "table { border-collapse: collapse; width: 100%%; }\n" //объединяет границы ячеек
                      "th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }\n" //td - ячейка, th - заголовок таблицы
                      "th { background-color: #f2f2f2; }\n"
                      "</style>\n"
                      "</head>\n"
                      "<body>\n"
                      "<h1>List Dump Log</h1>\n");
    fclose(htm_file);

    return LIST_NO_ERROR;
}


list_type_error close_list_log(const char* filename)
{
    char htm_filename[MAX_LENGTH_OF_FILENAME] = {};
    snprintf(htm_filename, sizeof(htm_filename), "%s.htm", filename);

    FILE* htm_file = fopen(htm_filename, "a");
    if (htm_file == NULL)
        return LIST_OPENING_FILE_ERROR;

    fprintf(htm_file, "</body>\n");
    fprintf(htm_file, "</htm>\n");
    fclose(htm_file);

    return LIST_NO_ERROR;
}















