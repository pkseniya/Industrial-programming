/**
\date October 2020
\copyright Ксения Петрушина. All rights reserved.
*/

#include "Shakespeare.h"

const char file_path[] = "/Users/kseniya/Desktop/log_file.txt";
const char error_file_path[] = "/Users/kseniya/Desktop/error_log_file.txt";

static FILE* file = NULL;

// Int
#define DEBUG
#define StackElem int
#define TYPE_FORMAT "d"
#include "Stack.h"

#undef DEBUG
#undef StackElem
#undef TYPE_FORMAT

// Double
#define StackElem double
#define TYPE_FORMAT "lf"
#include "Stack.h"

// ALL OK

int main() {
    OpenFile(file_path, &file, "wt");
    fprintf(file, "STACK 2020\n");
    PrintDescription("TEST 0\nALL OK");

    Stack(int) stk1 = {};
    StackConstruct(&stk1, 4);
    int value = 0;

    if (StackPop(&stk1, &value))
        printf("Can't pop\n");
    else
        printf("Value: %d\n", value);

    StackPush(&stk1, 10);
    StackPush(&stk1, 110);

    Stack(double) stk2 = {};
    Stack(int) stk3 = {};
    StackConstruct(&stk3, 4);
    StackConstruct(&stk2, 4);

    StackPush(&stk2, 3.14);
    StackPush(&stk1, 1);
    StackPush(&stk3, 9);
    StackPush(&stk2, 9.888);
    double value2 = 0;

    if (StackPop(&stk2, &value2))
        printf("Can't pop");
    else
        printf("Double value: %f\n", value2);

    StackPush(&stk3, 144);

    StackDestruct(&stk1);
    StackDestruct(&stk2);
    StackDestruct(&stk3);
    CloseFile(&file);
    return 0;
}

//int main(){
//    OpenFile(file_path, &file, "wt");
//    fprintf(file, "STACK 2020\n");
//
//    Stack(int) stk = {};
//    StackConstruct(&stk, 10);
//
//    for (int i = 0; i < 513; i++)
//        if (StackPush(&stk, i))
//            printf("Can't push %d-th element\n", i + 1);
//
//    StackDump(&stk, STACK_OK, 70, "main", "stk", "StackDump");
//    StackDestruct(&stk);
//    CloseFile(&file);
//    return 0;
//}

//int main(){
//    OpenFile(file_path, &file, "wt");
//    fprintf(file, "STACK 2020\n");
//
//    Stack(int) stk = {};
//    StackConstruct(&stk, 4);
//    int value = 0;
//    StackPush(&stk, 10);
//    StackPush(&stk, 10);
//    StackPush(&stk, 10);
//    for (int i = 0; i < 100; i++) {
//        StackPush(&stk, 10);
//        StackPop(&stk, &value);
//    }
//    StackDestruct(&stk);
//    CloseFile(&file);
//    return 0;
//}

// INVALID POINTER

//int main() {
//    OpenFile(error_file_path, &file, "at");
//
//    // NULL stack
//    PrintDescription("TEST 1.1\nINVALID POINTER");
//    StackPush(NULL, 10);
//
//    // NULL data
//    PrintDescription("TEST: 1.2\nINVALID POINTER");
//    Stack(int) stk = {};
//    StackConstruct(&stk, 4);
//    stk.data = NULL;
//    StackPush(&stk, 1);
//    CloseFile(&file);
//    return 0;
//}

// INCORRECT CAPACITY

//int main() {
//    OpenFile(error_file_path, &file, "at");
//    PrintDescription("TEST 2\nINCORRECT CAPACITY");
//    Stack(int) stk = {};
//    StackConstruct(&stk, 0);
//    StackPush(&stk, 5);
//    StackDestruct(&stk);
//    CloseFile(&file);
//    return 0;
//}

// INCORRECT SIZE

//int main() {
//    OpenFile(error_file_path, &file, "at");
//    PrintDescription("TEST 3\nINCORRECT SIZE");
//    Stack(int) stk = {};
//    StackConstruct(&stk, 4);
//    stk.size = 5;
//    StackPush(&stk, 5);
//    StackDestruct(&stk);
//    CloseFile(&file);
//    return 0;
//}

// STACK FIELDS CORRUPTED

//int main() {
//    OpenFile(error_file_path, &file, "at");
//    PrintDescription("TEST 4\nSTACK FIELDS CORRUPTED");
//    Stack(int) stk = {};
//    StackConstruct(&stk, 4);
//    stk.capacity = 10;
//    StackPush(&stk, 5);
//    StackDestruct(&stk);
//    CloseFile(&file);
//    return 0;
//}

// STACK DATA CORRUPTED

//int main() {
//    OpenFile(error_file_path, &file, "at");
//    PrintDescription("TEST 5\nSTACK DATA CORRUPTED");
//    Stack(int) stk = {};
//    StackConstruct(&stk, 4);
//    StackPush(&stk, 5);
//    *buffer_start(&stk) = 10;
//    int value = 0;
//    StackPop(&stk, &value);
//    StackDestruct(&stk);
//    CloseFile(&file);
//    return 0;
//}

// DATA CANARY CORRUPTED

//int main() {
//    OpenFile(error_file_path, &file, "at");
//    PrintDescription("TEST 6\nDATA CANARY CORRUPTED");
//    Stack(int) stk = {};
//    StackConstruct(&stk, 4);
//    stk.data[0] = 10;
//    int value = 0;
//    StackPop(&stk, &value);
//    StackDestruct(&stk);
//    CloseFile(&file);
//    return 0;
//}

// STACK CANARY CORRUPTED

//int main() {
//    OpenFile(error_file_path, &file, "at");
//    PrintDescription("TEST 7\nSTACK CANARY CORRUPTED");
//    Stack(int) stk = {};
//    StackConstruct(&stk, 4);
//    *((int *) &stk) = 10;
//    StackPush(&stk, 5);
//    StackDestruct(&stk);
//    CloseFile(&file);
//    return 0;
//}

//

// STACK POISON CORRUPTED

//int main() {
//    OpenFile(error_file_path, &file, "at");
//    PrintDescription("TEST 8\nSTACK POISON CORRUPTED");
//    Stack(int) stk = {};
//    StackConstruct(&stk, 4);
//    *(buffer_start(&stk) + 1) = 10;
//    StackPush(&stk, 5);
//    StackDestruct(&stk);
//    CloseFile(&file);
//    return 0;
//}
