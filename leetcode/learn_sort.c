/**
 * for learning how to write sort code
 */

#include <stdio.h>

int bubble_sort(int *input, size_t input_size)
{
    size_t i, j;
    int tmp;
    printf("the input size = %u\n", input_size);

    for(i = 0; i < input_size - 1; i++)
    {
        printf("Range:%d\n", input_size - i - 1);

        for (j = 0; j < input_size - i - 1; j++)
        {
            printf("  Inner Range:%d\n", input_size - i - 1);

        if(input[j] > input[j + 1])
        {
            tmp = input[j];
            input[j] = input[j + 1];
            input[j + 1] = tmp;
        }
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    int i;
    int a[] = {23, 12, 78, 44, 2, 4};
    bubble_sort(a, sizeof(a) / sizeof(int));
    for (i = 0; i < sizeof(a) / sizeof(int); i++)
    {
        printf("%d  ", a[i]);
    }
    printf("\n");

    return 0;
}
