/**
\date September 2020
\copyright Ксения Петрушина. All rights reserved.
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>

#define eps 1e-7
#define MAX_ATTEMPTS 7

typedef enum {
    ZERO_ROOTS = 0,
    ONE_ROOT = 1,
    TWO_ROOTS = 2,
    INF_ROOTS = INT_MAX
} roots;

typedef enum {
    TEST_ERROR = 1,
    INPUT_ERROR = 2,
    UNEXPECTED_ERROR = 3
} exit_codes;

/**
Launches unit-tests
*/

void Test(void);

/**
Checks that the coefficients of the equation ax^2 + bx + c = 0 are entered correctly
 
@param[out] a a-coeffitient
@param[out] b b-coeffitient
@param[out] c c-coeffitient
 
@return Is the input correct
*/

bool Scan(double* a, double* b, double* c);

/**
Checks if the number is within epsilon-neighbourhood from zero

@param[in] a Number we are checking

@return Is the number close to zero
*/

bool IsZero(double a);

/**
Determines the type of equation ax^2 + bx + c = 0 and solves it
 
@param[in] a a-coefficient
@param[in] b b-coefficient
@param[in] c c-coefficient
@param[out] x1 Pointer to the 1st root
@param[out] x2 Pointer to the 2d root

@return Number of roots
*/

roots SolveEquation(double a, double b, double c,
               double* x1, double* x2);

/**
Solves a linear equation bx + c = 0

@param[in] b b-coefficient
@param[in] c c-coefficient
@param[out] x Pointer to the root

@return Number of roots of a linear equation
*/

roots Linear(double b, double c, double* x);

/**
Solves a quadratic equation ax^2 + bx + c = 0

@param[in] a a-coefficient
@param[in] b b-coefficient
@param[in] c c-coefficient
@param[out] x1 Pointer to the 1st root
@param[out] x2 Pointer to the 2d root

@return Number of roots of a quadratic equation
*/

roots Quadratic(double a, double b, double c,
                double* x1, double* x2);

/**
Prints the number of roots and their values

@param[in] n_roots Number of roots
@param[in] x1 Value of the 1st root
@param[in] x2 Value of the 2d root
*/

void PrintSolution(roots n_roots, double x1, double x2);


int main() {
    Test();
    double a = 0, b = 0, c = 0;
    double x1 = 0, x2 = 0;
    printf("Solving equation ax^2 + bx + c = 0\n");
    if (!Scan(&a, &b, &c)) {
        fprintf(stderr, "Input Error. "
                        "Termination of the program.\n");
        exit(INPUT_ERROR);
    }
    roots result = SolveEquation(a, b, c, &x1, &x2);
    PrintSolution(result, x1, x2);
    return 0;
}

void Test() {
    bool ok = true;
        
    // ZERO_TEST
    {
    if (!IsZero(1e-8) || !IsZero(-1e-8) || IsZero(1)) {
        fprintf(stderr, "IsZeroTest failed.\n");
        ok = false;
    }
    else
        fprintf(stderr, "IsZeroTest OK.\n");
    }
    
    // LINEAR_TEST
    {
    double x1 = 0, x2 = 0;
    if (SolveEquation(0, 0, 0, &x1, &x2) != INF_ROOTS ||
        SolveEquation(0, 0, 1, &x1, &x2) != ZERO_ROOTS ||
        SolveEquation(0, 1, 2, &x1, &x2) != ONE_ROOT)
        {
        fprintf(stderr, "LinearEquationTest failed.\n");
        ok = false;
        }
    else
        fprintf(stderr, "LinearEquationTest OK.\n");
    }
    
    // QUAD_TEST
    {
    double x1 = 0, x2 = 0;
    if (SolveEquation(1, 2, 3, &x1, &x2) != ZERO_ROOTS ||
        SolveEquation(1, 2, 1, &x1, &x2) != ONE_ROOT ||
        SolveEquation(1, 5, 4, &x1, &x2) != TWO_ROOTS) {
        fprintf(stderr, "QuadraticEquationTest failed.\n");
        ok = false;
    }
    else
        fprintf(stderr, "QuadraticEquationTest OK.\n");
    }
        
    if (!ok) {
        fprintf(stderr, "Termination of the program.\n");
        exit(TEST_ERROR);
    }
}
    
bool Scan(double* a, double* b, double* c) {
    assert(a);
    assert(b);
    assert(c);
    
    int num_attempts = 1;
    bool success = false;
    while ((num_attempts <= MAX_ATTEMPTS)) {
        printf("Enter 3 real numbers a, b, c:\n");
        fflush(stdin);
        success = (scanf("%lf %lf %lf", a, b, c) == 3) && (!isinf(*a))
                && (!isinf(*b)) && (!isinf(*c));
        if (success || num_attempts == MAX_ATTEMPTS)
            break;
        else
            printf("Incorrect input. Attempts remaining: %d\n",
               MAX_ATTEMPTS - num_attempts++);
    }
    return success;
}

bool IsZero(double a) {
    return fabs(a) < eps;
}

roots SolveEquation(double a, double b, double c,
               double* x1, double* x2) {
    assert (x1);
    assert (x2);
    assert (x1 != x2);
    
    if (IsZero(a))
        return Linear(b, c, x1);
    else
        return Quadratic(a, b, c, x1, x2);
}

roots Linear(double b, double c, double* x) {
    assert (x);
    
    if (IsZero(b))
        return (IsZero(c)) ? INF_ROOTS : ZERO_ROOTS;
    else {
        *x = -c / b;
        return ONE_ROOT;
    }
}

roots Quadratic(double a, double b, double c,
                double* x1, double* x2) {
    assert (x1);
    assert (x2);
    assert (x1 != x2);
    assert(!isinf(a));
    assert(!isinf(b));
    assert(!isinf(c));
    
    double d = b * b - 4 * a * c;
    
    if (IsZero(d)) {
        *x1 = -b / (2 * a);
        return ONE_ROOT;
    }
    
    else if (d < 0)
        return ZERO_ROOTS;
    
    else {
        double sqrt_d = sqrt(b * b - 4 * a * c);
        *x1 = (-b + sqrt_d) / (2 * a);
        *x2 = (-b - sqrt_d) / (2 * a);
        return TWO_ROOTS;
    }
}

void PrintSolution(roots n_roots, double x1, double x2) {
    switch (n_roots) {
            
        case ZERO_ROOTS:
            printf("No roots\n");
            break;
            
        case ONE_ROOT:
            printf("One root: x = %lf\n", x1);
            break;
            
        case TWO_ROOTS:
            printf("Two roots: x1 = %lf, x2 = %lf\n", x1, x2);
            break;
            
        case INF_ROOTS:
            printf("Infinite number of roots. x ∈ R.\n");
            break;
            
        default:
            fprintf(stderr, "Unexpected Error."
                            "Termination of the program.\n");
            exit(UNEXPECTED_ERROR);
            break;
    }
}
