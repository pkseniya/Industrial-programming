/**
\date October 2020
\copyright Ксения Петрушина. All rights reserved.
*/

#include <stdio.h>
#include "Shakespeare.h"

const char file_path[] = "/Users/kseniya/Desktop/log_file.txt";
static FILE* file = NULL;

//#define DEBUG

// Int
#undef DEBUG
#define StackElem int
#include "Stack.h"
#undef StackElem

// Double
#define DEBUG
#define StackElem double
#include "Stack.h"

// ALL OK

int main() {
    OpenFile(file_path, &file, "wt");
    fprintf(file, "STACK 2020\n");
    PrintDescription("TEST 0\nALL OK");
    
    STACK (int) stk1 = {};
    CONSTRUCT (int, &stk1, 4);
    int value = 0;
    
    if (POP (int, &stk1, &value))
        printf("Can't pop\n");
    else
        printf("Value: %d\n", value);
    
    PUSH (int, &stk1, 10);
    PUSH (int, &stk1, 110);
    PUSH (int, &stk1, 20);
    PUSH (int, &stk1, 202);
    STACK (double) stk2 = {};
    CONSTRUCT (double, &stk2, 4);
    PUSH (double, &stk2, 3.14);
    PUSH (int, &stk1,  1);
    POP (int, &stk1, &value);
    printf("value: %d\n", value);
    POP (int, &stk1, &value);
    printf("value: %d\n", value);
    POP (int, &stk1, &value);
    printf("value: %d\n", value);
    PUSH (double, &stk2, 9.888);
    double value2 = 0;
    POP (double, &stk2, &value2);
    printf("Double value: %f\n", value2);
    DESTRUCT (int, &stk1);
    DESTRUCT (double, &stk2);
    CloseFile(&file);
    return 0;
}

// INVALID POINTER

int main1() {
    OpenFile(file_path, &file, "at");
    PrintDescription("TEST 1.1\nINVALID POINTER");
    PrintDescription("TEST: 1.2\nINVALID POINTER");
    
    // NULL stack
    PUSH (int, NULL, 10);
    
    // NULL data
    STACK (int) stk = {};
    PUSH (int, &stk, 1);
    CloseFile(&file);
    return 0;
}

// NO ALLOCATED MEMORY

int main2() {
    OpenFile(file_path, &file, "at");
    PrintDescription("TEST 2\nNO ALLOCATED MEMORY");
    STACK (int) stk = {};
    CONSTRUCT (int, &stk, 6);
    DESTRUCT  (int, &stk);
    PUSH (int, &stk, 10);
    CloseFile(&file);
    return 0;
}

// INCORRECT SIZE

int main3() {
    OpenFile(file_path, &file, "at");
    PrintDescription("TEST 3\nINCORRECT SIZE");
    STACK (int) stk = {};
    CONSTRUCT (int, &stk, 4);
    stk.size = 5;
    PUSH (int, &stk, 5);
    DESTRUCT (int, &stk);
    CloseFile(&file);
    return 0;
}

// STACK FIELDS CORRUPTED

int main4() {
    OpenFile(file_path, &file, "at");
    PrintDescription("TEST 4\nSTACK FIELDS CORRUPTED");
    STACK (int) stk = {};
    CONSTRUCT (int, &stk, 4);
    stk.capacity = 10;
    PUSH (int, &stk, 5);
    DESTRUCT (int, &stk);
    CloseFile(&file);
    return 0;
}

// STACK DATA CORRUPTED

int main5() {
    OpenFile(file_path, &file, "at");
    PrintDescription("TEST 5\nSTACK DATA CORRUPTED");
    STACK (int) stk = {};
    CONSTRUCT (int, &stk, 4);
    PUSH (int, &stk, 5);
    //*buffer_start(&stk) = 10;
    int value = 0;
    POP (int, &stk, &value);
    DESTRUCT (int, &stk);
    CloseFile(&file);
    return 0;
}

// DATA CANARY CORRUPTED

int main6() {
    OpenFile(file_path, &file, "at");
    PrintDescription("TEST 6\nDATA CANARY CORRUPTED");
    STACK (int) stk = {};
    CONSTRUCT (int, &stk, 4);
    stk.data[0] = 10;
    int value = 0;
    POP (int, &stk, &value);
    DESTRUCT (int, &stk);
    CloseFile(&file);
    return 0;
}

// STACK CANARY CORRUPTED

int main7() {
    OpenFile(file_path, &file, "at");
    PrintDescription("TEST 7\nSTACK CANARY CORRUPTED");
    STACK (int) stk = {};
    CONSTRUCT (int, &stk, 4);
    *((int *) &stk) = 10;
    PUSH (int, &stk, 5);
    DESTRUCT (int, &stk);
    CloseFile(&file);
    return 0;
}

//

// STACK POISON CORRUPTED

int main8() {
    OpenFile(file_path, &file, "at");
    PrintDescription("TEST 8\nSTACK POISON CORRUPTED");
    STACK (int) stk = {};
    CONSTRUCT (int, &stk, 4);
    //*(buffer_start(&stk) + 1) = 10;
    PUSH (int, &stk, 5);
    DESTRUCT (int, &stk);
    CloseFile(&file);
    return 0;
}

int maind() {
    printf("a");
    return STACK_FULL;
}
