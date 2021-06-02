#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef enum {
    SIN,
    COS,
    TAN,
    LOG,
    NONE
} function;


void SyntaxError() {
    printf("syntax err");
}

static char* s = NULL;
static size_t p = 0;

void Require(char symb) {
    if (s[p] == symb)
        p++;
    else
        printf("Need %c symbol", symb);
}

function is_func() {
    if (s[p] == 's' && s[p+1] == 'i' && s[p+2] == 'n') {
        p += 3;
        return SIN;
    }
    else if (s[p] == 'c' && s[p+1] == 'o' && s[p+2] == 's') {
        p += 3;
        return COS;
    }
    else if (s[p] == 't' && s[p+1] == 'a' && s[p+2] == 'n') {
        p += 3;
        return TAN;
    }
    else if (s[p] == 'l' && s[p+1] == 'o' && s[p+2] == 'g'){
        p += 3;
        return LOG;
    }
    else
        return NONE;
}

double GetG(char* str);
double GetE(void);
double GetP(void);
double GetT(void);
double GetN(void);

double GetE() {
    printf("enter E\n");
    double val = GetT();
    while (s[p] == '+' || s[p] == '-') {
        char op = s[p];
        p++;
        double val2 = GetT();
        if (op == '+')
            val += val2;
        else
            val -= val2;
    }
    return val;
}

double GetP() {
    printf("enter P\n");
    function func = is_func();
    if (s[p] == '(') {
        p++;
        double val = GetE();
        Require(')');
        switch (func) {
            case SIN:
                val = sin(val);
                break;
            case COS:
                val = cos(val);
                break;
            case TAN:
                val = tan(val);
                break;
            case LOG:
                val = log(val);
                break;
            default:
                break;
        }
        return val;
    }
    else
        return GetN();
}

double GetT() {
    printf("enter T\n");
    double val = GetP();
    while (s[p] == '*' || s[p] == '/') {
        char op = s[p];
        p++;
        double val2 = GetP();
        if (op == '*')
            val *= val2;
        else
            val /= val2;
    }
    return val;
}

double GetG(char* str) {
    printf("enter G\n");
    s = str;
    p = 0;
    double val = GetE();
    if (s[p] == '$')
        p++;
    else
        SyntaxError();
    return val;
}

double GetN() {
    
    printf("enter N\n");
    printf("p: %zu\n", p);
    size_t val = 0;
    bool changed = false;
    while ('0' <= s[p] && s[p] <= '9') {
        printf("%c\n", s[p]);
        val = val * 10 + s[p] - '0';
        changed = true;
        p++;
    }
    if (!changed)
        SyntaxError();
    return (double) val;
}
