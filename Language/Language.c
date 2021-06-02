/**
\date November 2020
\copyright Ксения Петрушина. All rights reserved.
*/

#include "Language.h"

int main() {
    double val = GetG("sin(3)+5*8$");
    printf("%lf\n", val);
    return 0;
}
