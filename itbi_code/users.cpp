#include <iostream>

void print_arr(int *arr, int size)
{
    printf("[");
    for (int i = 0; i < size - 1; i++)
    {
        printf("%d,", arr[i]);
    }
    printf("%d]\n", arr[size - 1]);
}

void print_nested_arr(int **arr, int rows, int cols)
{
    printf("[\n");
    for (int i = 0; i < rows - 1; i++)
    {
        printf("  ");
        print_arr(arr[i], cols);
    }
    printf("  ");
    print_arr(arr[rows - 1], cols);
    printf("]\n");
}

int main()
{
    int size = 10;
    int *arr = (int *)malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++)
    {
        arr[i] = i;
    }

    print_arr(arr, size);

    printf("\n");

    int **nestedArr = (int **)malloc(sizeof(int *) * size);
    for (int i = 0; i < size; i++)
    {
        nestedArr[i] = (int *)malloc(sizeof(int) * size);
        for (int j = 0; j < size; j++)
        {
            nestedArr[i][j] = j;
        }
    }

    print_nested_arr(nestedArr, size, size);
}