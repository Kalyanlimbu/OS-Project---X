#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define X_CAPACITY 300
#define Y_CAPACITY 400
#define Z_CAPACITY 500
#define ARGV_SIZE 100
#define MAX_ORDERS 50 // placeholder number for max number of orders

char * argv[ARGV_SIZE];
char startDate[MAX_ORDERS];
char endDate[MAX_ORDERS];

int main(){
    char startdate[11]; //period start date
    char enddate[11]; //period end date
    char orders[100][4][11]; //index i,j,k where i is the entries, j is the 4 attributes of order, k is the max length of the char array.

    //initialize hardcoded value
    startdate = "2024-06-01";
    enddate = "2024-06-10";
    orders[0] = {"P000", "2024-06-02","1200","Shoes"};
    orders[1] = {"P001", "2024-06-03","800","Socks"};
    orders[2] = {"P002", "2024-06-06","1000","Pants"};
    orders[3] = {"P003", "2024-06-06","500","Shirt"};
    orders[4] = {"P004", "2024-06-08","400","Jacket"};
    orders[5] = {"P005", "2024-06-07","2000","Hats"};



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

}
