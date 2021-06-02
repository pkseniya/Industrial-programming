
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Shakespeare.h"

static FILE*  file = NULL;
static FILE*  log_file = NULL;
static FILE*  database = NULL;

#undef IS_LIST
#undef PREFIX
#undef INFIX

#ifdef AKINATOR
#define PREFIX(code) code
#define INFIX(code)
#define IS_LIST(type) type == ANSWER
#endif

#ifdef DIFFERENTIATOR
#define PREFIX(code)
#define INFIX(code) code
#define IS_LIST(type) type != OPERATION
#endif

#define IS_STR(str1,str2) strcmp(str1, str2) == 0

typedef enum {
    NOT_LIST,
    LIST,
    QUESTION,
    ANSWER,
    OPERATION,
    VARIABLE,
    CONST
} type_t;

typedef enum {
    LOW,
    MEDIUM,
    HIGH,
    VERY_HIGH,
    EXTREME,
    ERR
} priority_t;

typedef struct Node {
    struct Node* left;
    struct Node* right;
    type_t       type;
    char*        data;
    struct Node* parent;
    
    INFIX(priority_t priority;)
} Node;

typedef enum {
    RIGHT,
    WRONG,
    EMPTY_NODE
} succ_code;

void PrintTree(Node const* root, char* indent, FILE* output);

Node* CreateNode(char* data, Node* parent, type_t type) {
    Node* tmp = (Node*) malloc(sizeof(Node));

    if (tmp) {
        tmp->left = tmp->right = NULL;
        tmp->data = data;
        tmp->type = type;
        tmp->parent = parent;
        return tmp;
    }
    else {
        printf("MMMAAA");
        return NULL;
    }
}

succ_code Guess(char* answer) {
    char response[10] = "\0";
    if (IS_STR(answer, " "))
        return EMPTY_NODE;
    printf("Это %s?\n", answer);
    scanf("%s", response);
    if (IS_STR(response, "Да")|| IS_STR(response, "да"))
        return RIGHT;
    else
        return WRONG;
}

Node* UpdateAkinator(Node* node, succ_code code) {
    char answer[60] = "";
    printf("Что это?\n");
    
    scanf("\n%[^\n]", answer);
    char* new_ans = (char *) calloc(strlen(answer), sizeof(char));
    strcpy(new_ans, answer);
    if (code == WRONG) {
        char question[50] = "";
        printf("Чем %s отличается от %s?\n", new_ans, node->data);
        scanf("\n%[^\n]", question);
        char* new_que = (char *) calloc(strlen(question), sizeof(char));
        strcpy(new_que, question);
        node->right = CreateNode(node->data, node, ANSWER);
        node->left  = CreateNode(new_ans, node, ANSWER);
        node->type  = QUESTION;
        node->data  = new_que;
    }
    else if (code == EMPTY_NODE)
        node->data = new_ans;
    return node;
}

Node* Ask(Node* node) {
    if (Guess(node->data))
        return node->right;
    else
        return node->left;
}

Node* PlayAkinator(Node* node) {
    printf("Игра началась\n");
    while (true) {
        if (node->type == ANSWER) {
            succ_code res = Guess(node->data);
            if (res == RIGHT)
                printf("Ура. Я угадал\n");
            else {
                node = UpdateAkinator(node, res);
                printf("Эх, в следующий раз буду лучше\n");
            }
            while (node->parent)
                node = node->parent;
            break;
        }
        else if (node->type == QUESTION)
            node = Ask(node);
    }
    return node;
}

priority_t GetPriority(char* operation) {
    if (IS_STR(operation, "sin") || IS_STR(operation, "cos") || IS_STR(operation, "log"))
        return VERY_HIGH;
    else if (IS_STR(operation, "^"))
        return HIGH;
    else if (IS_STR(operation, "*") || IS_STR(operation, "/"))
        return MEDIUM;
    else if (IS_STR(operation, "+") || IS_STR(operation, "-"))
        return LOW;
    else
        return ERR;
}

Node* SetType(Node* node) {
    PREFIX(
           if (node->type == LIST)
               node->type = ANSWER;
           else
               node->type = QUESTION;
           )
    INFIX(
          if (node->type == LIST) {
              if (isalpha((node->data)[0])) {
                  node->type = VARIABLE;
              }
              else
                  node->type = CONST;
              node->priority = EXTREME;
          }
          else {
              node->type = OPERATION;
              priority_t priority = GetPriority(node->data);
              if (priority == ERR)
                  return NULL;
              else
                  node->priority = priority;
          }
          )
    return node;
}
    

Node* FillNode(Node* node, char* data) {
    char* tmp_data = (char*) calloc(strlen(data), sizeof(char));
    if (tmp_data)
        strcpy(tmp_data, data);
    node->data = tmp_data;
    return node;
}

Node* ReadTree(FILE* file) {
    Node* root = NULL;
    size_t size = 0;
    char* buffer = NULL;
    ReadTextAndGetSize(file, &buffer, &size);
    
    char* buffer_copy = (char *) calloc(size, sizeof(char));
    strcpy(buffer_copy, buffer);

    size_t num_nodes = 0;
    string* data = NULL;
    SeparateTextToLines(buffer, size, &data, &num_nodes, "()\n\t");
    
    char* plug = 0;
    size_t counter = 0, i = 0;
    
    while (i < size) {
        if (buffer_copy[i] == '(') {
            if (!root)
                root = CreateNode(plug, NULL, NOT_LIST);

            else if (!root->left) {
                root->left = CreateNode(plug, root, NOT_LIST);
                root = root->left;
            }
            else if (!root->right) {
                root->right = CreateNode(plug, root, NOT_LIST);
                root = root->right;
            }
            else
                return NULL; //WRONG_INPUT_FORMAT;
            i++;
        }
        else if (buffer_copy[i] == ')') {
            if (!root)
                return NULL; //WRONG_INPUT_FORMAT;

            else {
                if (!root->left)
                    root->type = LIST;
                
                root = SetType(root);
                
                if (root->parent)
                    root = root->parent;
            }
            i++;
        }
        
        else if (buffer_copy[i] == ' ' || buffer_copy[i] == '\n' || buffer_copy[i] == '\t')
            i++;
        else {
            if (strstr(buffer_copy + i, data[counter].str)) {
                root = FillNode(root, data[counter].str);
                i += data[counter].len;
                counter++;
            }
            else
                printf("error\n");
        }
    }
    free(data);
    free(buffer);
    free(buffer_copy);
    return root;
}

void PrintTree(Node const* root, char* indent, FILE* output) {
    if (root) {
        char* tmp = (char*) calloc(strlen(indent) + 1, sizeof(char));
        strcpy(tmp, indent);
        strcat(tmp, "\t");
        if (IS_LIST(root->type))
            fprintf(output, "%s(%s)\n", indent, root->data);
    
        else {
            fprintf(output, "%s(\n%s%s\n", indent, indent, root->data);
            if (root->left)
                PrintTree(root->left, tmp, output);
            if (root->right)
                PrintTree(root->right, tmp, output);
            fprintf(output, "%s)\n", indent);
        }
        free(tmp);
    }
    else
        printf("no root\n");
    return;
}

Node* FindNode(Node* node, char* data) {
    Node* res = NULL;
    
    if (IS_STR(node->data, data))
        return node;
    
    if (node->left) {
        res = FindNode(node->left, data);
        if (!res && node->right)
            res = FindNode(node->right, data);
        return res;
    }
    else {
        if (IS_STR(node->data, data))
            return node;
        else
            return NULL;
    }
}

void PrintDescription(Node* root, char* obj) {
    Node* node = FindNode(root, obj);
    if (!node)
        printf("%s нет в моей базе данных", obj);
    else if (node->type == QUESTION)
        printf("%s не является объектом", obj);
    else if (!node->parent)
        printf("%s единственный объект в базе данных. У него нет описания", obj);
    else {
        Node* parent = node->parent;
        char* sep = ": ";
        printf("%s", obj);
        while (parent) {
            printf("%s", sep);
            if (parent->right == node)
                printf("не ");
            printf("%s", parent->data);
            sep = ", ";
            node = node->parent;
            parent = node->parent;
        }
    }
    printf(".\n");
}

void DeleteTree(Node* root) {
    
    type_t not_list = NOT_LIST;
    PREFIX(not_list = QUESTION; )
    INFIX (not_list = OPERATION;)
    
    if (!root)
        return;
    else if (root->type != not_list) {
        free(root->data);
        free(root);
    }
    else {
        if (root->left)
            DeleteTree(root->left);
        if (root->right)
            DeleteTree(root->right);
        free(root->data);
        free(root);
    }
}

