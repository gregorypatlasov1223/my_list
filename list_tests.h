#ifndef LIST_TESTS_H_
#define LIST_TESTS_H_

// #include "list_tests.h"

#include <assert.h>
#include <stdio.h>

#include "list.h"
#include "list_tests.h"

void test_first_insert();
void test_multiple_insertions_and_delete_middle();
void test_delete_head();
void test_delete_tail();
void test_delete_last_remaining_element();
void test_reuse_after_delete();
void test_reallocation();
void all_tests();

#endif // LIST_TESTS_H_
