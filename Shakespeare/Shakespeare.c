#include "Shakespeare.h"

int main() {
    
    // Open file, place text to buffer
    const char input_file_path[] = "/Users/kseniya/Desktop/input.txt";
    char* buffer = NULL;
    size_t size = 0;
    FILE* input_file = NULL;
    Check(OpenFile(input_file_path, &input_file, "r"));
    Check(ReadTextAndGetSize(input_file, &buffer, &size));
    Check(CloseFile(&input_file));
    
    // Separate text to lines
    size_t number_of_lines = 0;
    string* text = NULL;
    Check(SeparateTextToLines(buffer, size, &text, &number_of_lines));
    
    // Sort text and write to file
    const char output_file_path[] = "/Users/kseniya/Desktop/output.txt";
    FILE* output_file = NULL;
    Check(OpenFile(output_file_path, &output_file, "w"));
    Check(SortAndWriteLines(&output_file, &text, number_of_lines, false));
    Check(SortAndWriteLines(&output_file, &text, number_of_lines, true));
    Check(WriteBuffer(&output_file, buffer, size));
    Check(CloseFile(&output_file));
    
    // Free memory
    free(text);
    free(buffer);
    fprintf(stderr, "All OK\n");
    return ALL_OK;
}

exit_code OpenFile(const char file_path[], FILE** file_pointer, const char* mode) {
    assert(file_path);
    assert(file_pointer);
    
    if (access(file_path, F_OK) == -1)
        return FILE_NOT_EXIST_ERROR;

    else if (*mode == 'r' && access(file_path, R_OK) == -1)
        return FILE_READING_ERROR;
    
    else if (*mode == 'w' && access(file_path, W_OK) == -1)
        return FILE_WRITTING_ERROR;
    
    else if (!(*file_pointer = fopen(file_path, mode)))
        return FILE_OPENING_ERROR;
    
    else
        return ALL_OK;
}

exit_code CloseFile(FILE** file_pointer) {
    assert(file_pointer);
    assert(*file_pointer);
    
    if (fclose(*file_pointer) == EOF)
        return FILE_CLOSING_ERROR;
    
    else
        return ALL_OK;
}

exit_code ReadTextAndGetSize(FILE* file, char** buffer_pointer, size_t* size_pointer) {
    assert(file);
    assert(buffer_pointer);
    
    long tmp_size = SizeOfFile(file);
    
    if (tmp_size < 0) {
        return FILE_READING_ERROR;
    }
    
    *size_pointer = (size_t) tmp_size;
    *buffer_pointer = (char*) calloc(*size_pointer + 1, sizeof((*buffer_pointer)[0]));
    
    if (!(*buffer_pointer))
        return MEMORY_ALLOCATION_ERROR;
        
    fread(*buffer_pointer, sizeof(char), *size_pointer, file);
    return ALL_OK;
}

long SizeOfFile(FILE* file) {
    assert(file);
    
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size;
}

exit_code SeparateTextToLines(char* buffer, size_t size, string** text_pointer, size_t* num_lines_pointer) {
    assert(buffer);
    assert(text_pointer);
    assert(num_lines_pointer);
    
    *num_lines_pointer = CountLines(buffer, size);
    *text_pointer = (string *) calloc(*num_lines_pointer + 1, sizeof(string));
    
    if (!(*text_pointer))
        return MEMORY_ALLOCATION_ERROR;
    
    const char* sep = "\n";
    char* new_string = strtok(buffer, sep);
    int index = 0;
    
    while (new_string) {
        (*text_pointer)[index].str = new_string;
        (*text_pointer)[index++].len = strlen(new_string);
        new_string = strtok(NULL, sep);
    }
    
    return ALL_OK;
}

size_t CountLines(char* buffer, size_t size) {
    assert(buffer);
    
    size_t count = 0, buf_index = 0;
    
    /*  new_line equals the result of logical expression:
        (start of buffer) OR (previous character is '\n')
     
        The count of non-empty lines increments if
        (current character is not '\n') AND (new_line)
        After the increment new_line == false
     
        Thus, the result is number of non-empty lines
     */
    bool new_line = true;
    
    
    while (buf_index < size) {
        
        if (buffer[buf_index] == '\n')
            new_line = true;
        
        else {
            if (new_line)
                count++;
            new_line = false;
        }
        
        buf_index++;
    }
    
    return count;
}

exit_code SortAndWriteLines(FILE** file_pointer, string** text_pointer, size_t num_lines, bool reversed) {
    
    SortLines(text_pointer, num_lines, reversed);
    return WriteLines(file_pointer, *text_pointer, num_lines, reversed);
}

void SortLines(string** text_pointer, size_t num_lines, bool reversed) {
    assert(text_pointer);
    assert(*text_pointer);
    
    qsort(*text_pointer, num_lines, sizeof(string),
          (int(*) (const void*, const void*)) ((reversed) ? rotarapmoC : Comparator));
    return;
}

int Comparator(const string* str1, const string* str2) {
    
    return cmp(str1, str2, false);
}

int rotarapmoC(const string* str1, const string* str2) {
    
    return cmp(str1, str2, true);
}

int cmp(const string* str1, const string* str2, bool reversed) {
    assert(str1);
    assert(str2);
    
    char step = (reversed) ? -1 : 1;
    size_t i = (reversed) ? str1 -> len : 0;
    size_t j = (reversed) ? str2 -> len : 0;
    size_t bound1 = (reversed) ? -1 : str1 -> len;
    size_t bound2 = (reversed) ? -1 : str2 -> len;
    char symbol1 = get_lower(str1, i);
    char symbol2 = get_lower(str2, j);
    
    while (i != bound1 && j != bound2) {
        
        if (isalpha(symbol1) && isalpha(symbol2)) {
            
            if (symbol1 < symbol2)
                return -1;
            
            else if (symbol1 > symbol2)
                return 1;
            i += step;
            j += step;
        }
        
        else {
            if (!isalpha(symbol1))
                i += step;
            
            if (!isalpha(symbol2))
                j += step;
        }
        
        symbol1 = get_lower(str1, i);
        symbol2 = get_lower(str2, j);
    }
    
    if (i == bound1 && j == bound2)
        return 0;
    
    else if (i == bound1)
        return -1;
    
    else
        return 1;
}

char get_lower(const string* str, size_t i) {
    assert(str);
    
    return tolower((*str).str[i]);
}

exit_code WriteLines(FILE** file_pointer, string* text, size_t num_lines, bool reversed) {
    assert(file_pointer);
    assert(*file_pointer);
    assert(text);
    
    if (reversed) {
        if (fprintf(*file_pointer, "\nRHYMING DICTIONARY OF LINES:\n\n") < 0)
            return FILE_WRITTING_ERROR;
    }
    
    else {
        if (fprintf(*file_pointer, "DICTIONARY OF LINES IN ALPHABETICAL ORDER:\n\n") < 0)
            return  FILE_WRITTING_ERROR;
    }
    
    for (size_t i = 0; i < num_lines; i++) {
        if (fprintf(*file_pointer, "%s\n", text[i].str) < 0)
            return FILE_WRITTING_ERROR;
    }
    
    return ALL_OK;
}

exit_code WriteBuffer(FILE** file_pointer, char* buffer, size_t buffer_size) {
    assert(file_pointer);
    assert(*file_pointer);
    assert(buffer);
    
    if (fprintf(*file_pointer, "\nINITIAL TEXT:\n\n") < 0)
        return FILE_WRITTING_ERROR;
    
    size_t i = 0;
    
    while (i < buffer_size) {
        char symbol = buffer[i++];
        
        /*
         The use of strtok function has lead to replacement
                of all '\n' characters to '\0' characters in memory.
         
         In order to restore initial text
                these characters should be changed back.
         */
        if (fprintf(*file_pointer, "%c", (symbol == '\0') ? '\n' :  symbol) < 0)
            return FILE_WRITTING_ERROR;
    }
    
    return ALL_OK;
}

void Check(exit_code code) {
    
    switch (code) {
            
        case ALL_OK:
            break;
            
        case FILE_OPENING_ERROR:
            fprintf(stderr, "File Opening Error. "
                    "Termination of the program.\n");
            exit(code);
            break;
            
        case FILE_READING_ERROR:
            fprintf(stderr, "File Reading Error. "
                    "Termination of the program.\n");
            exit(code);
            break;
            
        case MEMORY_ALLOCATION_ERROR:
            fprintf(stderr, "Memory Allocation Error. "
                    "Termination of the program.\n");
            exit(code);
            break;
            
        case FILE_WRITTING_ERROR:
            fprintf(stderr, "File Writting Error. "
                    "Termination of the program.\n");
            exit(code);
            break;
            
        case FILE_CLOSING_ERROR:
            fprintf(stderr, "File Closing Error. "
                    "Termination of the program.\n");
            exit(code);
            break;
            
        default:
            fprintf(stderr, "Unexpected Error. "
                    "Termination of the program.\n");
            exit(UNEXPECTED_ERROR);
            break;
    }
}
