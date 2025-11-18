#include <stdio.h>

int main()
{
    int p = 1;
    for (int i = 1; i <= 5; i++)
    {
        p *= i;
    }
    printf("Factorial: %d\n", p);
    return 0;
}