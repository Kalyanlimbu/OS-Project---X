#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define X_CAPACITY 300
#define Y_CAPACITY 400
#define Z_CAPACITY 500
#define ARGV_SIZE 100
#define MAX_ORDERS 50 // placeholder number for max number of orders

char argv[ARGV_SIZE][100];
char orders[MAX_ORDERS][4][11]; // Assuming 100 orders max, 4 attributes each, 11 characters max per attribute
char Plant_X[30][4][11];
char Plant_Y[30][4][11];
char Plant_Z[30][4][11];
int orderno = 0;

int substring(const char *str, const char *substr) {
    int startsWith = 0;
    int i = 0;
    for (i = 0; substr[i] != '\0'; i++) {
        if (str[i] != substr[i]) {
            startsWith = 0;
            break;
        }
        startsWith = 1;
    }
    return startsWith;
}

int main() {
    char startdate[11]; // period start date
    char enddate[11]; // period end date

    printf("\t\t\t\t\t\tWELCOME TO PLS!\n");
    char command1[] = "addPEIOD";
    char command2[] = "addORDER";
    char command3[] = "addBATCH";
    char command4[] = "runPLS";
    char command5[] = "printREPORT";
    char command6[] = "exitPLS";

    while (1) {
        printf("\nPlease Enter:\n> ");
        char input[150];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            continue;
        }

        // Remove the newline character from the input
        input[strcspn(input, "\n")] = '\0';

        // Check the input command
        if (substring(input, command1)) {
            // Update the period
            char *token = strtok(input, " ");
            token = strtok(NULL, " "); // Skip the command
            if (token != NULL) {
                strcpy(startdate, token);
                token = strtok(NULL, " ");
                if (token != NULL) {
                    strcpy(enddate, token);
                    printf("The StartDate is: %s\n", startdate);
                    printf("The EndDate is: %s\n", enddate);
                } else {
                    fprintf(stderr, "Invalid command format for addPERIOD\n");
                }
            } else {
                fprintf(stderr, "Invalid command format for addPERIOD\n");
            }
        } else if (substring(input, command2)) {
            // Update orders
            if (orderno >= MAX_ORDERS) {
                fprintf(stderr, "Maximum number of orders reached\n");
                continue;
            }

            char *token = strtok(input, " ");
            token = strtok(NULL, " "); // Skip the command
            int field = 0;
            while (token != NULL && field < 4) {
                strcpy(orders[orderno][field], token);
                token = strtok(NULL, " ");
                field++;
            }

            if (field != 4) {
                fprintf(stderr, "Invalid command format for addORDER\n");
                continue;
            }
            printf ("The orders[current][0]: %s\n", orders[orderno][0]);
            printf ("The orders[current][0]: %s\n", orders[orderno][1]);
            printf ("The orders[current][0]: %s\n", orders[orderno][2]);
            printf ("The orders[current][0]: %s\n", orders[orderno][3]);

            orderno++;
        } else if (substring(input, command3)) {
            // Read the batch file
            char *token = strtok(input, " ");
            token = strtok(NULL, " "); // Skip the command
            if (token != NULL) {
                printf("Reading batch data from file: %s\n", token);
                // Implement file reading logic here
            } else {
                fprintf(stderr, "Invalid command format for addBATCH\n");
            }
        } else if (substring(input, command4)) {
            // Running the algorithm
            char *token = strtok(input, " ");
            token = strtok(NULL, " "); // Skip the command
            if (token != NULL) {
                printf("Running the production planning algorithm: %s\n", token);
                // Implement the algorithm logic here
            } else {
                fprintf(stderr, "Invalid command format for runPLS\n");
            }
        } else if (substring(input, command5)) {
            // Print report
            char *token = strtok(input, ">");
            token = strtok(NULL, ">"); // Skip the command
            if (token != NULL) {
                printf("Printing report to file: %s\n", token);
                // Implement report printing logic here
            } else {
                fprintf(stderr, "Invalid command format for printREPORT\n");
            }
        } else if (substring(input, command6)) {
            // Exit
            printf("\n\t\t\t\tPLS SYSTEM TERMINATED\n");
            break;
        } else {
            fprintf(stderr, "Invalid command\n");
        }
    }

    return 0;
}
