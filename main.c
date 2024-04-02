#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define X_CAPACITY 300
#define Y_CAPACITY 400
#define Z_CAPACITY 500


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



} ⁠