#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

#ifdef Stack
    #undef Stack
    #undef StackConstruct
    #undef StackPush
    #undef StackPop
    #undef StackTop
#endif

#ifdef OVERLOAD
    #undef OVERLOAD
#endif

#ifndef StackElem
    #define StackElem                         int
    #define TYPE_FORMAT                       "d"
#endif

#define OVERLOAD(name)                        PARSE(name, StackElem)

#ifdef ON_DEBUG
    #undef ON_DEBUG
#endif

#ifdef OFF_DEBUG
    #undef OFF_DEBUG
#endif
    
#ifdef DEBUG
    #define ON_DEBUG(code)                    code
    #define OFF_DEBUG(code)
#else
    #define ON_DEBUG(code)
    #define OFF_DEBUG(code)                   code
#endif

#ifndef Stack_h
    #define Stack_h

#include "Crc32.h"

#define POISON                                0xBAD
#define MIN_CAPACITY                          4
#define MAX_CAPACITY                          512
// In fact capacity can reach 1024

#define NAME_JOIN(name1,name2)                name1 ## _ ## name2
#define PARSE(name1,name2)                    NAME_JOIN(name1,name2)

#define LINE __LINE__
#define FUNC __FUNCTION__
#define NAME(v) #v

#define ASSERT_OK(write) {                                                \
    stack_state state = StackCheck(stk);                                  \
                                                                          \
    if (state) {                                                          \
        StackDump(stk, state, line, func, name, __FUNCTION__);            \
        if (state != INVALID_POINTER)                                     \
            StackDestruct(stk);                                           \
        CloseFile(&file);                                                 \
        exit(state);                                                      \
    }                                                                     \
                                                                          \
    else if (write) {                                                     \
        StackDump(stk, state, line, func, name, __FUNCTION__);            \
    }                                                                     \
}

#define REHASH(pointer, crc) xcrc32((const size_t *) pointer, 1, crc);

#define UPDATEHASH(stk) {                                                 \
    stk->dataHash = HashStackData(stk);                                   \
    stk->stackHash = HashStack(stk);                                      \
}

/**
 Stack states
 */

typedef enum {
    STACK_OK,
    STACK_EMPTY,
    STACK_FULL,
    INVALID_POINTER,
    NO_ALLOCATED_MEMORY,
    INCORRECT_CAPACITY,
    INCORRECT_SIZE,
    STACK_FIELDS_CORRUPTED,
    STACK_DATA_CORRUPTED,
    DATA_CANARY_CORRUPTED,
    STACK_CANARY_CORRUPTED,
    STACK_POISON_CORRUPTED                 
} stack_state;

typedef size_t StackCanary;

typedef size_t StackHash;

/**
 Writes test description to the file
 
 @param[in]      str                    String with test description
*/

void PrintDescription(const char* str) {
    
    fprintf(file, "\n--------------------------------------------------\n");
    fprintf(file, "%s", str);
    fprintf(file, "\n--------------------------------------------------\n");
}

/**
 Writes error description to the file
 
 @param[in]      err                    String with error description
*/

void PrintError(const char* err) {
    
    size_t len = (50 + strlen(err)) / 2;
    fprintf(file, "\n--------------------------------------------------\n");
    fprintf(file, "                    FATAL ERROR                   ");
    fprintf(file, "\n%*s\n", (int) len, err);
    fprintf(file, "            TERMINATION OF THE PROGRAM            ");
    fprintf(file, "\n--------------------------------------------------\n");
}

/**
Computes the 32-bit CRC of {buf} which has length @var{len}.  The
starting value is @var{init}

@param[in]     buf               Buffer pointer
@param[in]     len               Length of buffer
@param[in]     init             Initial value of hash
*/

StackHash xcrc32 (const size_t *buf, size_t len, StackHash init) {
    
    StackHash crc = init;
    
    while (len--) {
        crc = (crc << 8) ^ crc32_table[((crc >> 24) ^ *buf) & 255];
        buf++;
    }
    return crc;
}

#endif //Stack_h

typedef struct {
    
    ON_DEBUG(StackCanary stackCanaryFront;)
    
    size_t size;
    size_t capacity;
    bool empty;
    bool memory_allocated;
    
    StackElem* data;
    
    ON_DEBUG(
             StackHash stackHash;
             StackHash dataHash;
             
             StackCanary stackCanaryBack;
    )
} OVERLOAD(Stack);

// Function declarations

/**
 Checks stack state
 
 @param[in]      stk               Pointer to the stack
 
 @return         Stack state
*/

stack_state StackCheck (const OVERLOAD(Stack)* stk);

/**
 Writes stack state and stack contents if stack is ok to the log-file
 
 @param[in]      stk               Pointer to the stack
 @param[in]      state           Stack state
 @param[in]      line             Line number in main function that called the function, which detected an error
 @param[in]      func             Name of the main function
 @param[in]      name             Name of the stack
 @param[in]      func_name  Name of stack inteaction function
*/

void StackDump (const OVERLOAD(Stack)* stk, stack_state state, int line, const char* func, const char* name, const char* func_name);

/**
 Resizes stack capacity if nessesary
 
 @param[in]      stk               Pointer to the stack
 @param[in]      line             Line number in main function that called the function, which detected an error
 @param[in]      func             Name of the main function
 @param[in]      name             Name of the stack
 
 @return         Stack state
*/

stack_state StackResize (OVERLOAD(Stack)* stk, int line, const char* func, const char* name);

/**
 Constructs stack of given capacity
 
 @param[in,out]  stk               Pointer to the stack
 @param[in]      capacity    Capacity of the stack
 @param[in]      line             Line number in main function that called the function, which detected an error
 @param[in]      func             Name of the main function
 @param[in]      name             Name of the stack
*/

stack_state StackConstruct (OVERLOAD(Stack)* stk, size_t capacity, int line, const char* func, const char* name);

/**
 Pushes given value to the stack
 
 @param[in,out]  stk               Pointer to the stack
 @param[in]      value           Value to push
 @param[in]      line             Line number in main function that called the function, which detected an error
 @param[in]      func             Name of the main function
 @param[in]      name             Name of the stack
 
 @return         Stack state
*/

stack_state StackPush (OVERLOAD(Stack)* stk, StackElem value, int line, const char* func, const char* name);

/**
 Writes the value at the top of the stack to a variable
 
 @param[in]      stk               Pointer to the stack
 @param[in]      value           Pointer to variable to write to
 @param[in]      line             Line number in main function that called the function, which detected an error
 @param[in]      func             Name of the main function
 @param[in]      name             Name of the stack
 
 @return d
*/

stack_state StackTop (OVERLOAD(Stack)* stk, StackElem* value, int line, const char* func, const char* name);

/**
 Writes the value at the top of the stack to a variable and delites it from the stack
 
 @param[in,out]  stk               Pointer to the stack
 @param[in]      value           Pointer to variable to write to
 @param[in]      line             Line number in main function that called the function, which detected an error
 @param[in]      func             Name of the main function
 @param[in]      name             Name of the stack
 
 @return d
*/

stack_state StackPop (OVERLOAD(Stack)* stk, StackElem* value, int line, const char* func, const char* name);

/**
 Destructs the stack
 
 @param[in,out]  stk               Pointer to the stack
 */

void StackDestruct (OVERLOAD(Stack)* stk);

/**
 Dumps stack elements
 
 @param[in]      stk               Pointer to the stack
*/

void PrintElems (const OVERLOAD(Stack)* stk);

/**
 Prints an stack items according to theirs type
 
 @param[in]      elem             Pointer to the stack elem
 @param[in]      is_poison  If we use poison
*/

void PrintStackElem (const StackElem* elem, bool is_poison);

ON_DEBUG(

        /**
         Calculates stack and data canaries
                  
         @param[in]      stk               Pointer to the stack
                  
         @return         Canary value
        */

        StackCanary Canary (const OVERLOAD(Stack)* stk);
         
        /**
         Calculates the hash sum using stack fields
         
         @param[in]      stk               Pointer to the stack
                  
         @return         Hash sum
        */

        StackHash HashStack (const OVERLOAD(Stack)* stk);
           
        /**
         Calculates the hash sum using stack data
          
         @param[in]      stk               Pointer to the stack
                   
         @return         Hash sum
        */
                 
        StackHash HashStackData (const OVERLOAD(Stack)* stk);
         
        /**
         Finds the beginning of a block of memory with stack contents
         
         @param[in]      stk               Pointer to the stack
         
         @return         Pointer to the bottom of the stack
        */
                 
        StackElem* buffer_start(const OVERLOAD(Stack)* stk);
                 
         /**
          Finds the back data canary
          
          @param[in]      stk               Pointer to the stack
          
          @return         Pointer to the back data canary
         */
         
        StackCanary* buffer_end(const OVERLOAD(Stack)* stk);
                 
        /**
         Checks stack poison
         
          @param[in]      stk               Pointer to the stack
         
          @return         Is the poison ok
         */
         
        bool poison_ok(const OVERLOAD(Stack)* stk);
)

// Function defenitions

stack_state StackCheck (const OVERLOAD(Stack)* stk) {
    
    ON_DEBUG(StackCanary canary = Canary (stk);)
    
    if (!stk)
        return INVALID_POINTER;

    else if (stk->capacity > MAX_CAPACITY || stk->capacity < MIN_CAPACITY)
        return INCORRECT_CAPACITY;
    
    else if (!stk->data)
        return INVALID_POINTER;

    else if (!stk->memory_allocated)
        return NO_ALLOCATED_MEMORY;
    
    else if (stk->size >= stk->capacity || (stk->empty && stk->size > 0)) {
        return INCORRECT_SIZE;
    }
    
    ON_DEBUG(
             else if (stk->stackCanaryFront != canary ||
                      stk->stackCanaryBack  != canary)
                return STACK_CANARY_CORRUPTED;
             
             else if (stk->stackHash != HashStack (stk))
                return STACK_FIELDS_CORRUPTED;
                
             else if (stk->dataHash != HashStackData (stk))
                return STACK_DATA_CORRUPTED;
             
             else if (*buffer_end(stk) != canary ||
                      *((StackCanary *) stk->data) != canary)
                return DATA_CANARY_CORRUPTED;

             else if (!poison_ok(stk))
                return STACK_POISON_CORRUPTED;
    )
    
    else
        return STACK_OK;
}

void StackDump (const OVERLOAD(Stack)* stk, stack_state state, int line, const char* func,
                const char* name, const char* func_name) {
    
    fprintf(file, "\nFunction %s on line %d in function %s\nstack name: %s\n\n",
            func, line, func_name, name);
    
    switch (state) {
        case STACK_OK:
            fprintf(file, "Stack (ok) [%p] {\n\n\tsize:     %lu",
                    stk, stk->size);
            fprintf(file, "\n\tcapacity: %lu\n\tempty: %s\n",
                    stk->capacity, (stk->empty) ? "true" : "false");
            
            ON_DEBUG(
                     fprintf(file, "\n\tstack hash: %lu\n\tdata hash: %lu\n",
                             stk->stackHash, stk->dataHash);
                     fprintf(file, "\n\tstack canary: %lu\n",
                             stk->stackCanaryFront);
                     )
            
            PrintElems(stk);
            fprintf(file, "\n}");
            break;
            
        case INVALID_POINTER:
            PrintError("INVALID POINTER");
            break;
            
        case NO_ALLOCATED_MEMORY:
            PrintError("NO ALLOCATED MEMORY");
            break;
        
        case INCORRECT_CAPACITY:
            PrintError("INCORRECT CAPACITY");
            break;
            
        case INCORRECT_SIZE:
            PrintError("INCORRECT SIZE");
            break;
        
        case STACK_FIELDS_CORRUPTED:
            PrintError("STACK FIELDS CORRUPTED");
            break;
                
        case STACK_DATA_CORRUPTED:
            PrintError("STACK DATA CORRUPTED");
            break;
            
        case DATA_CANARY_CORRUPTED:
            PrintError("DATA CANARY CORRUPTED");
            break;
            
        case STACK_CANARY_CORRUPTED:
            PrintError("STACK CANARY CORRUPTED");
            break;
            
        case STACK_POISON_CORRUPTED:
            PrintError("STACK POISON CORRUPTED");
            break;
            
        default:
            PrintError("UNEXPECTED ERROR");
            break;
    }
    
    fprintf(file, "\n");
    return;
}

stack_state StackResize (OVERLOAD(Stack)* stk, int line, const char* func, const char* name) {
    ASSERT_OK(false)
    
    size_t capacity = stk->capacity;
        
    size_t upper_bound = capacity / 2;
    size_t lower_bound = capacity / 4;
    
    if (stk->empty)
        return STACK_OK;
    
    else if ((capacity >= MAX_CAPACITY) && (stk->size == stk->capacity - 1))
        return STACK_FULL;
    
    else if (capacity >= MAX_CAPACITY)
        return STACK_OK;
    
    else if ((lower_bound < stk->size) && (stk->size < upper_bound))
        return STACK_OK;
    
    else if (stk->size >= upper_bound) {
        stk->capacity = (capacity * 2 > MAX_CAPACITY) ? MAX_CAPACITY : capacity * 2;
        
        OFF_DEBUG(
                  stk->data = (StackElem *) realloc(stk->data, stk->capacity * sizeof(StackElem));
                  if (!stk->data)
                      return INVALID_POINTER;
                  
                  for (size_t i = stk->size + 1; i < capacity * 2; i++)
                      stk->data[i] = 0;
                  )
        
        ON_DEBUG (
                 stk->data = (StackElem *) realloc(stk->data, stk->capacity * sizeof(StackElem) + sizeof(StackCanary) * 2);
                  
                  if (!stk->data)
                    return INVALID_POINTER;
                  
                 StackElem* pointer = buffer_start(stk);
                  
                 for (size_t i = stk->size + 1; i < capacity * 2; i++)
                     pointer[i] = POISON;
                 
                 *buffer_end(stk) = Canary(stk);
        )
    }
    
    else if (stk->size <= lower_bound && stk->capacity >= MIN_CAPACITY * 2) {
        
        OFF_DEBUG(
                  stk->capacity = stk->capacity / 2;
                  stk->data = (StackElem *) realloc(stk->data, stk->capacity * sizeof(StackElem));
                  
                  if (!stk->data)
                      return INVALID_POINTER;
        )
        
        ON_DEBUG(
                 *buffer_end(stk) = 0;
                 stk->capacity = stk->capacity / 2;
                 StackElem* pointer = buffer_start(stk);
                 
                 for (size_t i = capacity; i > stk->capacity; i--)
                    pointer[i] = 0;
                 
                 stk->data = (StackElem *) realloc(stk->data, stk->capacity * sizeof(StackElem) + sizeof(StackCanary)* 2);
                 
                 if (!stk->data)
                     return INVALID_POINTER;
                 
                 *buffer_end(stk) = Canary(stk);
        )
    }
    
    ON_DEBUG(UPDATEHASH(stk))
    
    ASSERT_OK(true)
    return STACK_OK;
}

stack_state StackConstruct (OVERLOAD(Stack)* stk, size_t capacity, int line, const char* func, const char* name) {
    assert(stk);
    
    if (capacity > MAX_CAPACITY || capacity < MIN_CAPACITY)
        return INCORRECT_CAPACITY;
    
    stk->size = 0;
    stk->capacity =  capacity;
    stk->empty = true;
    stk->memory_allocated = false;
    
    OFF_DEBUG(
              stk->data = (StackElem *) calloc(stk->capacity, sizeof(StackElem));
              
              if (!stk->data)
                  return INVALID_POINTER;
              else
                  stk->memory_allocated = true;
              )

    ON_DEBUG(
             stk->data = (StackElem *) malloc(stk->capacity * sizeof(stk->data[0]) + 2 * sizeof(StackCanary));
             
             if (!stk->data)
                 return INVALID_POINTER;
             else
                 stk->memory_allocated = true;
             
             StackCanary canary = Canary(stk);
             stk->stackCanaryFront = canary;
             stk->stackCanaryBack = canary;
             
             StackElem* pointer = buffer_start(stk);
             for (size_t i = 0; i < stk->capacity; i++) {
                pointer[i] = POISON;
             }
             
             //Front buffer canary
             *((StackCanary *) stk->data) = canary;
             
             //Back  buffer canary
             *buffer_end(stk) = canary;
             
             stk->stackHash = 0LU;
             stk->dataHash = 0LU;

             UPDATEHASH(stk);
    )

    ASSERT_OK(true)
    return STACK_OK;
}

stack_state StackPush (OVERLOAD(Stack)* stk, StackElem value, int line, const char* func, const char* name) {
    ASSERT_OK(false)
    
    stack_state state = StackResize(stk, line, func, name);
    
    if (!state) {
        if (stk->empty)
            stk->empty = false;
        else
            stk->size++;
        
        OFF_DEBUG(stk->data[stk->size] = value;)
        
        ON_DEBUG (
                  buffer_start(stk)[stk->size] = value;
                  UPDATEHASH(stk);
                  )
    }
    
    ASSERT_OK(true)
    return state;
}

stack_state StackTop(OVERLOAD(Stack)* stk, StackElem* value, int line, const char* func, const char* name) {
    ASSERT_OK(true)
    
    if (!stk->empty) {
        
        OFF_DEBUG(*value = stk->data[stk->size];)

        ON_DEBUG (*value = buffer_start(stk)[stk->size];)
        
        return STACK_OK;
    }
    else
        return STACK_EMPTY;
}

stack_state StackPop (OVERLOAD(Stack)* stk, StackElem* value, int line, const char* func, const char* name) {

    stack_state error = StackTop(stk, value, line, func, name);
    
    if (!error) {
        
        OFF_DEBUG(stk->data[stk->size] = 0;)
        
        ON_DEBUG (buffer_start(stk)[stk->size] = POISON;)
        
        if (stk->size == 0)
            stk->empty = true;
        else
            stk->size--;
    }
    
    ON_DEBUG (UPDATEHASH(stk);)
    
    StackResize(stk, line, func, name);
    ASSERT_OK(true)
    return error;
}

void StackDestruct (OVERLOAD(Stack)* stk) {
    
    OFF_DEBUG(size_t size = stk->size;)
    
    ON_DEBUG (
              if (stk->capacity > MAX_CAPACITY * 2)
                  stk->capacity = MAX_CAPACITY * 2;
              size_t size = stk->capacity + 2 * sizeof(StackCanary)/sizeof(StackElem);
              )
    
    if (!stk->empty && stk->memory_allocated)
        for (size_t i = 0; i < size; i++)
            stk->data[i] = 0;
    
    stk->size = 0;
    stk->capacity = 0;
    stk->memory_allocated = false;
    
    ON_DEBUG (
             stk->stackCanaryFront = 0;
             stk->stackCanaryBack  = 0;
             stk->stackHash        = 0;
             stk->dataHash         = 0;
    )
    
    if (stk->memory_allocated)
        free(stk->data);
}

ON_DEBUG(
         StackCanary Canary (const OVERLOAD(Stack)* stk) {
            return 0x2DEC0DEBADC0DE | (StackCanary) stk;
         }

         StackHash HashStack (const OVERLOAD(Stack)* stk_pointer) {
            
            StackHash crc = xcrc32(0, 0, 0);
    
            crc = REHASH(stk_pointer,                     crc);
            crc = REHASH(&(stk_pointer->capacity),        crc);
            crc = REHASH(&(stk_pointer->size),            crc);
            crc = REHASH(&(stk_pointer->empty),           crc);
            crc = REHASH(&(stk_pointer->memory_allocated),crc);
    
            return crc;
         }

         StackHash HashStackData (const OVERLOAD(Stack)* stk) {
            
            StackHash crc = xcrc32(0, 0, 0);
            
            if (!stk->empty) {
                for (size_t i = 0; i < stk->size; i++)
                    crc = REHASH(buffer_start(stk) + i, crc);
            }
            else
                crc = REHASH(buffer_start(stk), crc);
    
            return crc;
        }
         
        StackElem* buffer_start(const OVERLOAD(Stack)* stk) {
            return (StackElem *) (((StackCanary *) stk->data) + 1);
        }
         
        StackCanary* buffer_end(const OVERLOAD(Stack)* stk) {
            return (StackCanary *) (buffer_start(stk) + stk->capacity);
        }
         
        bool poison_ok(const OVERLOAD(Stack)* stk) {
             
            StackElem* pointer = buffer_start(stk);
            bool result = true;
             
            for (size_t i = stk->size + 1; i < stk->capacity; i++)
                result = (pointer[i] == POISON) && result;

            return result;
        }
)

void PrintElems(const OVERLOAD(Stack)* stk) {
    
    fprintf(file, "\n\t{\n");
    
    OFF_DEBUG(
              for (size_t i = 0; i < stk->capacity; i++) {
                  fprintf(file, "\n\t\t[%lu] ", i);
                  PrintStackElem(stk->data + i, false);
              }
    )
    
    ON_DEBUG (
              bool is_poison = false;
              StackElem* pointer = buffer_start(stk);
             
              for (size_t i = 0; i < stk->capacity; i++) {
                  if (stk->empty || i > stk->size)
                      is_poison = true;
        
                  fprintf(file, "\n\t\t[%lu] ", i);
                  PrintStackElem(pointer + i, is_poison);
             }
    )
    
    fprintf(file, "\n\t}\n");
    
    return;
}

void PrintStackElem (const StackElem* elem, bool is_poison) {

    fprintf(file, "%" TYPE_FORMAT, *elem);
      
    if (is_poison)
        fprintf(file, " (POISON)");
    
    fprintf(file, "\n");
    return;
}

#define Stack(type)                         PARSE          (Stack, type)

#define StackConstruct(stk, capacity)       StackConstruct (stk, capacity, LINE, FUNC, NAME(stk))
#define StackPush(stk, value)               StackPush      (stk, value, LINE, FUNC, NAME(stk))
#define StackPop(stk, value)                StackPop       (stk, value, LINE, FUNC, NAME(stk))
#define StackTop(stk, value)                StackTop       (stk, value, LINE, FUNC, NAME(stk))
