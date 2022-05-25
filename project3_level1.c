#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define UNKNOWN "n"

void eliminationGauss();

int main() {
    int choice;
    do {
        printf("\n\n ### WELCOME TO LINEAR EQUATION SOLVING SYSTEM ### \n");
        printf("\n 1. Use the elimination method to solve a system of linear equations.");
        printf("\n 2. Exit");
        printf("\n Enter your choice:");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            eliminationGauss();
            break;
        case 2:
            exit(0);
            printf("\nBye!");
        default:
            printf("\n Wrong choice. Enter between 1 to 3.");
        }

    } while (choice != 0);

    return 0;
}

void eliminationGauss() {
    int k, n;
    float A[20][20], c, x[10], sum = 0.0;
    printf("\nEnter the order of matrix: ");   
    scanf("%d", &n);
    printf("\nEnter the elements of augmented matrix row-wise:\n\n");   
    for (int row = 1; row <= n; row++)
    {
        for (int column = 1; column <= (n + 1); column++)
        {
            printf("A[%d][%d] : ", row, column);
            scanf("%f", &A[row][column]);
        }
    } 

    for (int column = 1; column <= n; column++) 
    {
        for (int row = 1; row <= n; row++)
        {
            if (row > column)
            {
                c = A[row][column] / A[column][column];
                for (k = 1; k <= n + 1; k++)
                {
                    A[row][k] = A[row][k] - c * A[column][k];
                }
            }
        }
    }
    x[n] = A[n][n + 1] / A[n][n];
    printf("\nThe Upper Triangular matrix is: \n\n");
    for (int row = 1; row <= n; row++)
    {
        for (int column = 1; column <= (n + 1); column++)
        {
            printf(" %f ", A[row][column]);
        }
        printf("\n");
    }
   
    for (int row = n - 1; row >= 1; row--)
    {
        sum = 0;
        for (int column = row + 1; column <= n; column++)
        {
            sum = sum + A[row][column] * x[column];
        }
        x[row] = (A[row][n + 1] - sum) / A[row][row];
    }
    printf("\nThe solution is: \n");
    for (int row = 1; row <= n; row++)
    {
        printf("\nx%d=%f\t", row, x[row]); //x1, x2, x3 are the required solutions
    }
    return;
}
