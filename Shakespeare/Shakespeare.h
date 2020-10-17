/**
\date October 2020
\copyright Ксения Петрушина. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

typedef enum {
    ALL_OK = 0,
    FILE_NOT_EXIST_ERROR = 1,
    FILE_OPENING_ERROR = 2,
    FILE_READING_ERROR = 3,
    MEMORY_ALLOCATION_ERROR = 4,
    FILE_WRITTING_ERROR = 5,
    FILE_CLOSING_ERROR = 6,
    UNEXPECTED_ERROR = 7
} exit_code;


typedef struct string {
    char* str;
    size_t len;
} string;


/**
 Opens file using file path
 
 @param[in]  file_path         Array containing file path
 @param[out] file_pointer  Pointer to the file
 @param[in]  mode                    Mode of opening
 
 @return Exit code
 */

exit_code OpenFile(const char file_path[], FILE** file_pointer, const char* mode);

/**
 Closes the file
 
 @param[in] file_pointer  Pointer to the file
 
 @return Exit code
 */

exit_code CloseFile(FILE** file_pointer);

/**
 Reads text from file and counts its size in characters
 
 @param[in,out] file                        File to read from
 @param[out]    buffer_pointer  Pointer to buffer of text
 @param[out]    size_pointer      Pointer to size of text in characters
 
 @return Exit code
 */

exit_code ReadTextAndGetSize(FILE* file, char** buffer_pointer, size_t* size_pointer);

/**
 Counts number of characters in the text
 
 @param[in] file  File
 
 @return Number of characters
 
 @note Returns -1L in case of error
 */

long SizeOfFile(FILE* file);

/**
 Separates text from buffer to non-empty lines
 
 @param[in]  buffer                          Pointer to the first character of the text
 @param[in]  size                               Number of characters in the text
 @param[out] text_pointer             Pointer to array of lines
 @param[out] num_lines_pointer  Pointer to number of lines in text
 
 @return Exit code
 */

exit_code SeparateTextToLines(char* buffer, size_t size, string** text_pointer, size_t* num_lines_pointer);

/**
 Counts line in text
 
 @param[in] buffer  Pointer to the first character of the text
 @param[in] size      Number of chsracters in the text
 
 @return Number of non-empty lines in the text
 */

size_t CountLines(char* buffer, size_t size);

/**
 Sorts lines lexicographically from the beggining or from the end and writes to the file
 
 @param[in]     file_pointer  Pointer to the file to write to
 @param[in,out] text_pointer  Pointer to the array of lines
 @param[in]     num_lines         Number of lines in the text
 @param[in]     reversed           Whether to sort from the end
 
 @return Exit code
 */

exit_code SortAndWriteLines(FILE** file_pointer, string** text_pointer, size_t num_lines, bool reversed);

/**
 Sorts lines lexicographically from the beggining or from the end
 
 @param[in,out] text_pointer  Pointer to the array of lines
 @param[in]     num_lines         Number of lines in the text
 @param[in]     reversed           Whether to sort from the end
 */

void SortLines(string** text_pointer, size_t num_lines, bool reversed);

/**
 Writes sorted lines to the file

 @param[in]     file_pointer  Pointer to the file to write to
 @param[in,out] text                   Array of lines
 @param[in]     num_lines        Number of lines in the text
 @param[in]     reversed          Whether to sort from the end

 @return Exit code
*/

exit_code WriteLines(FILE** file_pointer, string* text, size_t num_lines, bool reversed);

/**
 Writes initial text from buffer to the file

 @param[in]     file_pointer  Pointer to the file to write to
 @param[in]     buffer               Pointer to the first character of the text
 @param[in]     buffer_size    Size of buffer in characters

 @return Exit code
*/

exit_code WriteBuffer(FILE** file_pointer, char* buffer, size_t buffer_size);

/**
 Compares strings lexicographically

 @param[in] str1  Pointer to the first string
 @param[in] str2  Pointer to the second string
 
 @return Result of comparison
*/

int Comparator(const string* str1, const string* str2);

/**
 Compares strings lexicographically but from the end

 @param[in] str1  Pointer to the first string
 @param[in] str2  Pointer to the second string
 
 @return Result of comparison
*/

int rotarapmoC(const string* str1, const string* str2);

/**
 Compares strings lexicographically from the beginning or from the end

 @param[in] str1           Pointer to the first string
 @param[in] str2           Pointer to the second string
 @param[in] reversed  Whether to compare from the end
 
 @return Result of comparison
*/

int cmp(const string* str1, const string* str2, bool reversed);

/**
 Gets i-th char from the string and transforms it to lower case
 
 @param[in] str  The string to get char to
 @param[in] i      Char position
 
 @return Char in the position i in lower case
 */

char get_lower(const string* str, size_t i);

/**
 Halts the program if necessary
 
 @param[in] code Exit code of the function
 */

void Check(exit_code code);
