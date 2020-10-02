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
Determines the type of equation by the coefficients and calls the corresponding function
 
@param[in] a a-coefficient
@param[in] b b-coefficient
@param[in] c c-coefficient
@param[out] x1 Pointer to the 1st root
@param[out] x2 Pointer to the 2d root

@return Number of roots
*/

roots NumRoots(double a, double b, double c,
               double* x1, double* x2);

/**
Solves a linear equation bx + c = 0

@param[in] b b-coefficient
@param[in] c c-coefficient
@param[out] x Pointer to the root

@return Number of roots of a linear equation

@note Returns enum type, in case of infinite number of roots returns INF.
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

void PrintNumRoots(roots n_roots, double x1, double x2);

int main()
    {
    Test();
    double a = 0, b = 0, c = 0;
    double x1 = 0, x2 = 0;
    if (!Scan(&a, &b, &c))
        {
        fprintf(stderr, "Input Error. \
Termination of the program.\n");
        exit(INPUT_ERROR);
        }
    roots n_roots = NumRoots(a, b, c, &x1, &x2);
    PrintNumRoots(n_roots, x1, x2);
    return 0;
    }

#define ZERO_TEST
#define LINEAR_TEST
#define QUAD_TEST

void Test()
    {
    bool ok = true;
    ZERO_TEST
        {
        if (!IsZero(1e-8) || !IsZero(-1e-8) || IsZero(1))
            {
            fprintf(stderr, "IsZeroTest failed.\n");
            ok = false;
            }
        else
            fprintf(stderr, "IsZeroTest OK.\n");
        }
    LINEAR_TEST
        {
        double x1 = 0, x2 = 0;
        if (NumRoots(0, 0, 0, &x1, &x2) != INF_ROOTS ||
            NumRoots(0, 0, 1, &x1, &x2) != ZERO_ROOTS ||
            NumRoots(0, 1, 2, &x1, &x2) != ONE_ROOT)
            {
            fprintf(stderr, "LinearEquationTest failed.\n");
            ok = false;
            }
        else
            fprintf(stderr, "LinearEquationTest OK.\n");
        }
    QUAD_TEST
        {
        double x1 = 0, x2 = 0;
        if (NumRoots(1, 2, 3, &x1, &x2) != ZERO_ROOTS ||
            NumRoots(1, 2, 1, &x1, &x2) != ONE_ROOT ||
            NumRoots(1, 5, 4, &x1, &x2) != TWO_ROOTS)
            {
            fprintf(stderr, "QuadraticEquationTest failed.\n");
            ok = false;
            }
        else
            fprintf(stderr, "QuadraticEquationTest OK.\n");
        }
    if (!ok)
        {
        fprintf(stderr, "Termination of the program.\n");
        exit(TEST_ERROR);
        }
    }
    
bool Scan(double* a, double* b, double* c)
    {
    printf("Solving equation ax^2 + bx + c = 0\n\
Enter 3 real numbers a, b, c:\n");
    return ((scanf("%lf %lf %lf", a, b, c) == 3) && (!isinf(*a))
            && (!isinf(*b)) && (!isinf(*c)));
    }

bool IsZero(double a)
    {
    return fabs(a) < eps;
    }

roots NumRoots(double a, double b, double c,
               double* x1, double* x2)
    {
    assert (x1 != NULL);
    assert (x2 != NULL);
    assert (x1 != x2);
    if (IsZero(a))
        {
        return Linear(b, c, x1);
        }
    return Quadratic(a, b, c, x1, x2);
    }

roots Linear(double b, double c, double* x)
    {
    if (IsZero(b))
        {
        return (IsZero(c)) ? INF_ROOTS : ZERO_ROOTS;
        }
    *x = -c / b;
    return ONE_ROOT;
    }

roots Quadratic(double a, double b, double c,
                double* x1, double* x2){
    double d = b * b - 4 * a * c;
    //D = 0
    if (IsZero(d))
        {
        *x1 = -b / (2 * a);
        return ONE_ROOT;
        }
    //D < 0
    else if (d < 0)
        return ZERO_ROOTS;
    //D > 0
    double sqrt_d = sqrt(b * b - 4 * a * c);
    *x1 = (-b + sqrt_d) / (2 * a);
    *x2 = (-b - sqrt_d) / (2 * a);
    return TWO_ROOTS;
    }

void PrintNumRoots(roots n_roots, double x1, double x2)
    {
    switch (n_roots)
        {
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
            fprintf(stderr, "Unexpected Error. \
Termination of the program.\n");
            exit(UNEXPECTED_ERROR);
            break;
        }
    }
