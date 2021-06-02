/**
\date November 2020
\copyright Ксения Петрушина. All rights reserved.
*/

#define AKINATOR
//#undef AKINATOR
//#define DIFFERENTIATOR

#include <locale.h>
#include "Akinator.h"

int main() {
    //setlocale (LC_ALL, "Russian");
    OpenFile("/Users/kseniya/Desktop/Tree.txt", &file, "rt");
    //OpenFile("/Users/kseniya/Desktop/TreeLog.txt", &log_file, "wt");
    OpenFile("/Users/kseniya/Desktop/database.txt", &database, "wt");
    Node* root = ReadTree(file);
    if (!root) {
        printf("ERR");
        DeleteTree(root);
        CloseFile(&file);
        //CloseFile(&log_file);
        CloseFile(&database);
        return 5;
    }
    CloseFile(&file);
    
    char* indent = "";
    while (true) {
        //PrintTree(root, indent, stdout);
        //PrintDescription(root, "умная");
        PlayAkinator(root);
        printf("Еще один раз?\n");
        char ans[10] = "\0";
        scanf("%s", ans);
        if (strcmp(ans, "да") != 0 && strcmp(ans, "Да") != 0)
            break;
    }
    PrintTree(root, indent, database);
    DeleteTree(root);
    //CloseFile(&log_file);
    CloseFile(&database);
    return 0;
}
