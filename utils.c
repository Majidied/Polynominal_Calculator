#include "Poly_project.h"

int findlen(int num)
{
    int num_digits = 1;
    if (num < 0)
        num_digits++;
    while (num /= 10 >= 10)
    {
        num_digits++;
    }
    return num_digits;
}