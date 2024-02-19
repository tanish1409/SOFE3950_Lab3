#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define NUM_THREADS 11
#define SIZE 9

typedef struct {
    int* value;
} ThreadParam;

int sudoku[SIZE][SIZE] = {
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

bool validateRow(int row) {
    bool seen[SIZE] = {false};
    for (int i = 0; i < SIZE; i++) {
        int num = sudoku[row][i];
        if (num < 1 || num > SIZE || seen[num - 1]) {
            return false;
        }
        seen[num - 1] = true;
    }
    return true;
}

bool validateColumn(int col) {
    bool seen[SIZE] = {false};
    for (int i = 0; i < SIZE; i++) {
        int num = sudoku[i][col];
        if (num < 1 || num > SIZE || seen[num - 1]) {
            return false;
        }
        seen[num - 1] = true;
    }
    return true;
}

bool validateSubgrid(int startRow, int startCol) {
    bool seen[SIZE] = {false};
    for (int i = startRow; i < startRow + 3; i++) {
        for (int j = startCol; j < startCol + 3; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > SIZE || seen[num - 1]) {
                return false;
            }
            seen[num - 1] = true;
        }
    }
    return true;
}

void* checkRow(void* param) {
    ThreadParam* threadParam = (ThreadParam*)param;
    int row = *(threadParam->value);
    bool isValid = validateRow(row);
    printf("Row %d is %s\n", row + 1, isValid ? "valid" : "invalid");
    if (threadParam->value) {
        free(threadParam->value);
    }
    if (threadParam) {
        free(threadParam);
    }
    pthread_exit(NULL);
}

void* checkColumn(void* param) {
    ThreadParam* threadParam = (ThreadParam*)param;
    int col = *(threadParam->value);
    bool isValid = validateColumn(col);
    printf("Column %d is %s\n", col + 1, isValid ? "valid" : "invalid");
    if (threadParam->value) {
        free(threadParam->value);
    }
    if (threadParam) {
        free(threadParam);
    }
    pthread_exit(NULL);
}

void* checkSubGrid(void* param) {
    ThreadParam* threadParam = (ThreadParam*)param;
    int subgrid = *(threadParam->value);
    int startRow = (subgrid / 3) * 3;
    int startCol = (subgrid % 3) * 3;
    bool isValid = validateSubgrid(startRow, startCol);
    printf("Subgrid %d is %s\n", subgrid + 1, isValid ? "valid" : "invalid");
    if (threadParam->value) {
        free(threadParam->value);
    }
    if (threadParam) {
        free(threadParam);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int threadIndex = 0;

    // Row checks
    for (int i = 0; i < SIZE; i++) {
        ThreadParam* param = (ThreadParam*)malloc(sizeof(ThreadParam));
        param->value = (int*)malloc(sizeof(int));
        *(param->value) = i;
        pthread_create(&threads[threadIndex++], NULL, checkRow, (void*)param);
    }

    // Column checks
    for (int i = 0; i < SIZE; i++) {
        ThreadParam* param = (ThreadParam*)malloc(sizeof(ThreadParam));
        param->value = (int*)malloc(sizeof(int));
        *(param->value) = i;
        pthread_create(&threads[threadIndex++], NULL, checkColumn, (void*)param);
    }

    // Subgrid checks
    for (int i = 0; i < SIZE; i++) {
        ThreadParam* param = (ThreadParam*)malloc(sizeof(ThreadParam));
        param->value = (int*)malloc(sizeof(int));
        *(param->value) = i;
        pthread_create(&threads[threadIndex++], NULL, checkSubGrid, (void*)param);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
    }
