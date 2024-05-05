#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void matrix_multi(int **matrix1, int **matrix2, int **result, int i, int j, int k) {
    for (int x = 0; x < i; x++) {
        for (int y = 0; y < k; y++) {
            result[x][y] = 0;
            for (int z = 0; z < j; z++) {
                result[x][y] += matrix1[x][z] * matrix2[z][y];
            }
        }
    }
}

int main() {
    int i, j, k;
    
    printf("Enter the value of i: ");
    scanf("%d", &i);
    
    printf("Enter the value of j: ");
    scanf("%d", &j);
    
    printf("Enter the value of k: ");
    scanf("%d", &k);
    
    if (i <= 0 || j <= 0 || k <= 0) {
        printf("Error: i, j, and k must be positive integers.\n");
        return 1;
    }
    
    int **matrix1 = (int **)malloc(i * sizeof(int *));
    int **matrix2 = (int **)malloc(j * sizeof(int *));
    int **result = (int **)malloc(i * sizeof(int *));
    
    for (int x = 0; x < i; x++) {
        matrix1[x] = (int *)malloc(j * sizeof(int));
        result[x] = (int *)malloc(k * sizeof(int));
        for (int y = 0; y < j; y++) {
            matrix1[x][y] = rand() % 51;
        }
    }
    
    for (int x = 0; x < j; x++) {
        matrix2[x] = (int *)malloc(k * sizeof(int));
        for (int y = 0; y < k; y++) {
            matrix2[x][y] = rand() % 51;
        }
    }
    
    matrix_multi(matrix1, matrix2, result, i, j, k);
    
    printf("Matrix 1:\n");
    for (int x = 0; x < i; x++) {
        for (int y = 0; y < j; y++) {
            printf("%d ", matrix1[x][y]);
        }
        printf("\n");
    }
    
    printf("Matrix 2:\n");
    for (int x = 0; x < j; x++) {
        for (int y = 0; y < k; y++) {
            printf("%d ", matrix2[x][y]);
        }
        printf("\n");
    }
    
    printf("Result:\n");
    for (int x = 0; x < i; x++) {
        for (int y = 0; y < k; y++) {
            printf("%d ", result[x][y]);
        }
        printf("\n");
    }
    
    for (int x = 0; x < i; x++) {
        free(matrix1[x]);
        free(result[x]);
    }
    
    for (int x = 0; x < j; x++) {
        free(matrix2[x]);
    }
    
    free(matrix1);
    free(matrix2);
    free(result);
    
    return 0;
}