#include "utils.h"
#include "structures.h"


int read_int() {
    int value;
    char buffer[20]; // Buffer for input

    while (1) {
        printf("Enter an integer: ");

        // Read a line of input
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input.\n");
            exit(EXIT_FAILURE);
        }

        // Attempt to convert the input to an integer
        if (sscanf(buffer, "%d", &value) == 1) {
            // Successful conversion
            return value;
        } else {
            // Invalid input, prompt the user again
            printf("Invalid input. Please enter an integer.\n");
        }
    }
}

void read_filename(char* filename){
    int res;
    do{
        res = scanf("%s", filename);
    }while(res != 1);
}

int maximum(int a, int b){
    return (a>b?a:b);
}
