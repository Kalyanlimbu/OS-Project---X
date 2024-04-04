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
char startDate[MAX_ORDERS];
char endDate[MAX_ORDERS];


int main(){
    char startdate[11]; // period start date
    char enddate[11]; // period end date
    strcpy(startdate, "2024-06-01");
    strcpy(enddate, "2024-06-10");

    char orders[100][4][11]; // Assuming 100 orders max, 4 attributes each, 11 characters max per attribute

    // Initialize orders using strcpy for each string
    // Order 0
    strcpy(orders[0][0], "P000");
    strcpy(orders[0][1], "2024-06-02");
    strcpy(orders[0][2], "1200");
    strcpy(orders[0][3], "Shoes");

    // Order 1
    strcpy(orders[1][0], "P001");
    strcpy(orders[1][1], "2024-06-03");
    strcpy(orders[1][2], "800");
    strcpy(orders[1][3], "Socks");

    // Order 2
    strcpy(orders[2][0], "P002");
    strcpy(orders[2][1], "2024-06-06");
    strcpy(orders[2][2], "1000");
    strcpy(orders[2][3], "Pants");

    // Order 3
    strcpy(orders[3][0], "P003");
    strcpy(orders[3][1], "2024-06-06");
    strcpy(orders[3][2], "500");
    strcpy(orders[3][3], "Shirt");

    // Order 4
    strcpy(orders[4][0], "P004");
    strcpy(orders[4][1], "2024-06-08");
    strcpy(orders[4][2], "400");
    strcpy(orders[4][3], "Jacket");

    // Order 5
    strcpy(orders[5][0], "P005");
    strcpy(orders[5][1], "2024-06-07");
    strcpy(orders[5][2], "2000");
    strcpy(orders[5][3], "Hats");

    // Remember, for more orders, continue with similar pattern...
    
    // Example of accessing an order's attributes:
    printf("Order ID: %s, Date: %s, Quantity: %s, Product: %s\n",
           orders[0][0], orders[0][1], orders[0][2], orders[0][3]);

    return 0;
}


void FCFS(char InputFile[40], char OutputFile[40]){
    int day_count=0;
    char startDate[MAX_ORDERS];
    char endDate[MAX_ORDERS];
    char dueDate[MAX_ORDERS];
    char productRequired[MAX_ORDERS];
    char quantity[MAX_ORDERS];
    FILE *input_file;
    FILE *output_file;
    input_file=fopen(InputFile,"Read");
    output_file = fopen(OutputFile,"Write");

    if (input_file==NULL) { 
        printf("File not available\n"); 
        exit(0);     
}
