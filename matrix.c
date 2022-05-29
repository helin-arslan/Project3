#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define EQUATION_LINE_BUFFER_LENGTH 256

#define ASCII_LOWER_A 97
#define ASCII_LOWER_Z 122

#define FIRST_EQUATION_INDEX 0

#define MAX_NUMBER_OF_VARIABLES 10
#define MAX_EQUATION_PARTS (MAX_NUMBER_OF_VARIABLES + 1)

#define FILE_NAME "equations_2.txt"

void removeAllChars(char* str, char c);

int main(int argc, char* argv[]) {

    const char equationPartTerminSymbols[] = {'+', '-', '='};

    float M [MAX_NUMBER_OF_VARIABLES][MAX_NUMBER_OF_VARIABLES];
    float C [MAX_NUMBER_OF_VARIABLES];

    int firstEquationVariableCount;
    char* acceptedVariables = malloc(sizeof(char) * MAX_NUMBER_OF_VARIABLES);

    char equation[EQUATION_LINE_BUFFER_LENGTH];
    FILE* equationsFile = fopen(FILE_NAME, "r");

    int equationIndex = 0;
    while (fgets(equation, sizeof(equation), equationsFile)) {
        int currentEquestionPartIndex = 0;
        int currentEquationVariableCount = 0;

        char equationParts[MAX_EQUATION_PARTS][100];
        memset(equationParts, 0, sizeof(equationParts));

        int equationPartCharIndex = 0;
        for (int charIndex=0; charIndex<strlen(equation) ; charIndex++) {
            char currentChar = equation[charIndex];
            if (strchr(equationPartTerminSymbols, currentChar) != NULL && (charIndex != 0) && equation[charIndex-1] != '=') {
                currentEquestionPartIndex++;
                equationPartCharIndex=0;
            }

            equationParts[currentEquestionPartIndex][equationPartCharIndex] = currentChar;
            equationPartCharIndex++;

            if (currentChar >= ASCII_LOWER_A && currentChar <= ASCII_LOWER_Z ) {
                if (equationIndex == FIRST_EQUATION_INDEX) {
                    acceptedVariables[currentEquationVariableCount] = currentChar;
                }
                currentEquationVariableCount++;
            }
        }

        if (equationIndex == FIRST_EQUATION_INDEX) {
            firstEquationVariableCount = strlen(acceptedVariables);
        }

        if (currentEquationVariableCount != firstEquationVariableCount) {
            printf("Not possible to solve! Current equation has %d variables", currentEquationVariableCount);
            return EXIT_FAILURE;
        }

        if  (equationIndex == FIRST_EQUATION_INDEX) {
            printf("Variables: ");
            for (int acceptedVariableIndex=0 ; acceptedVariableIndex < firstEquationVariableCount ; acceptedVariableIndex++ ) {
                printf("%c ", acceptedVariables[acceptedVariableIndex]);
            }
            printf("\n\n");
        }

        bool isBeforeEqualSign = true;
        const int equationPartCount = firstEquationVariableCount + 1;
        for (int partIndex = 0 ; partIndex < equationPartCount ; partIndex++) {
            char* currentEquationPart = equationParts[partIndex];
            // Check if equation part is on right or left side of equal sign
            if (strchr(currentEquationPart, '=') != NULL) {
                isBeforeEqualSign = false;
                removeAllChars(currentEquationPart, '=');
            }
            // Check if equation part contains variable
            bool hasVariable = false;
            // We defined variableIndex out of for loop to use it while putting coeffiecent in to M matrix
            int variableIndex = 0;
            for ( ; variableIndex<firstEquationVariableCount ; variableIndex++) {
                if (strchr(currentEquationPart, acceptedVariables[variableIndex]) != NULL) {
                    // If yes variableIndex is index of coloumn
                    hasVariable = true;
                    break;
                }
            }

            if (hasVariable) {
                // Take coefficent from equationPart
                float coefficent = strtod(currentEquationPart, NULL);
                if (coefficent == 0) {
                    coefficent = 1;
                }
                // Make sure coefficent left side of equation
                if (isBeforeEqualSign == false) {
                    coefficent *= -1;
                }
                // Put into coefficent matrix
                M[equationIndex][variableIndex] = coefficent;
            } else {
                // Take constant from equationPart
                float constant = strtod(currentEquationPart, NULL);
                // Make sure constant right side of equation
                if (isBeforeEqualSign == true) {
                    constant *= -1;
                }
                // Put into constant matrix
                C[equationIndex] = constant;        
            }
        }
        equationIndex++;
    }
    fclose(equationsFile);

    // Check if Variable and Equaion Counts are equal
    if (firstEquationVariableCount != equationIndex) {
        printf("Not possible to solve! Variable and Equation count are not equal!");
        return EXIT_FAILURE;
    }

    // Print M and C matrices
    for (int rowIndex=0 ; rowIndex<equationIndex; rowIndex++) {
        for (int columnIndex=0; columnIndex < firstEquationVariableCount ; columnIndex++) {
            printf("%f ",M[rowIndex][columnIndex]);
        }
        printf("C: %f ", C[rowIndex]);
        printf("\n");
    }

    // Calculate Upper Triangle Matrix
    for (int column = 0; column < firstEquationVariableCount; column++) {
        for (int row = 0; row < firstEquationVariableCount; row++) {
            if (row > column) {
                float ratio = (M[row][column] / M[column][column]) * -1;
                for (int multiplyColumn = 0; multiplyColumn < firstEquationVariableCount; multiplyColumn++) {
                    M[row][multiplyColumn] = M[row][multiplyColumn] + (ratio * M[column][multiplyColumn]);
                }
                C[row] = C[row] + (ratio * C[column]);
            }   
        }   
    } 

    printf("\n");

    // Print M and C matrices and C matrices
    for (int rowIndex=0 ; rowIndex<equationIndex; rowIndex++) {
        for (int columnIndex=0; columnIndex < firstEquationVariableCount ; columnIndex++) {
            printf("%f ",M[rowIndex][columnIndex]);
        }
        printf("C: %f ", C[rowIndex]);
        printf("\n");
    }

    // Calculate solution
    float* solution = malloc(sizeof(float)* firstEquationVariableCount);
    for (int rowIndex = firstEquationVariableCount - 1 ; rowIndex >= 0; rowIndex--) {
        float constant = C[rowIndex];
        for (int columnIndex=0 ; columnIndex<firstEquationVariableCount ; columnIndex++) {
            if (columnIndex != rowIndex) {
                constant -= M[rowIndex][columnIndex] * solution[columnIndex];
            }
        }
        solution[rowIndex] = constant / M[rowIndex][rowIndex];
    }

    // Print solution
    printf("\n");
    for (int solutionIndex=0 ; solutionIndex< firstEquationVariableCount ; solutionIndex++) {
        printf("Variable %c = %f \n", acceptedVariables[solutionIndex], solution[solutionIndex]);
    }

    return EXIT_SUCCESS;
}

void removeAllChars(char* str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}
