#ifndef LIST_ERROR_TYPE_H_
#define LIST_ERROR_TYPE_H_

enum list_type_error
{
    SHIT_VALUE                    = -1,
    SHIT_INDEX                    =  0,
    LIST_NO_ERROR                 =  1,
    LIST_ALLOCATION_FAILED        =  2,
    LIST_WRONG_SPECIFIED_CAPACITY =  3,
    LIST_OPENING_FILE_ERROR       =  4,
    LIST_NULL_POINTER             =  5,
    LIST_NO_FREE_SPACE            =  6,
};

enum  verify_result
{
    VERIFY_SUCCESS                  =  1,
    VERIFY_FAKE_ELEMENT_NEXT_ERROR  =  2,
    VERIFY_FAKE_ELEMENT_PREV_ERROR  =  3,
    VERIFY_TAIL_NEXT_ERROR          =  4,
    VERIFY_HEAD_PREV_ERROR          =  5,
    VERIFY_CAPACITY_EXCEEDED        =  6,
    VERIFY_INVALID_INDEX            =  7,
    VERIFY_BIDIRECTIONAL_LINK_ERROR =  8,
    VERIFY_COUNT_MISMATCH           =  9,
    VERIFY_EMPTY_LIST_LINKS_ERROR   =  10,
    VERIFY_CYCLE_DETECTED           =  11,
};

#endif // LIST_ERROR_TYPE_H_
