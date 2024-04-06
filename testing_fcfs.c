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
// char startDate[MAX_ORDERS];
// char endDate[MAX_ORDERS];
char startdate[11]; // period start date
char enddate[11]; // period end date

char orders[100][4][11]; 
char Plant_X[30][5][11];// Assuming 100 orders max, 4 attributes each, 11 characters max per attribute
char Plant_Y[30][5][11];
char Plant_Z[30][5][11];

int X_pointer = 0;
int Y_pointer = 0;
int Z_pointer = 0;


char* get_next_day(char* current_date) {
    // Extract year, month, and day from the input string
    int year, month, day;
    sscanf(current_date, "%d-%d-%d", &year, &month, &day);

    // Calculate the next day
    if (day < 28) {
        day++;
    } else {
        // Handle month end
        if (day == 28) {
            if (month == 2 && (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)) {
                // Leap year
                day++;
            } else {
                day = 1;
                month++;
            }
        } else if (day == 29) {
            if (month == 2) {
                day = 1;
                month++;
            } else {
                day++;
            }
        } else if (day == 30) {
            if (month == 4 || month == 6 || month == 9 || month == 11) {
                day = 1;
                month++;
            } else {
                day++;
            }
        } else { // day == 31
            if (month == 12) {
                day = 1;
                month = 1;
                year++;
            } else {
                day = 1;
                month++;
            }
        }
    }

    // Convert the next day back to string format
    static char next_day[11]; // "YYYY-MM-DD\0"
    sprintf(next_day, "%04d-%02d-%02d", year, month, day);

    return next_day;
}


void updateX(int value, int i, char* currdate) {
    if (X_pointer >= 31) return; // Prevents overflow
    strcpy(Plant_X[X_pointer][0], currdate);
    strcpy(Plant_X[X_pointer][1], orders[i][3]);
    strcpy(Plant_X[X_pointer][2], orders[i][0]);
    strcpy(Plant_X[X_pointer][4], orders[i][1]);
    char v[12]; // Increased size for safety
    sprintf(v, "%d", value < X_CAPACITY ? value : X_CAPACITY); // Using %d for int
    strcpy(Plant_X[X_pointer][3], v);
    X_pointer++;
}

void updateY(int value, int i, char* currdate) {
    if (Y_pointer >= 31) return; // Prevent overflow by checking against MAX_PLANTS
    strcpy(Plant_Y[Y_pointer][0], currdate);
    strcpy(Plant_Y[Y_pointer][1], orders[i][3]);
    strcpy(Plant_Y[Y_pointer][2], orders[i][0]);
    strcpy(Plant_Y[Y_pointer][4], orders[i][1]);
    char v[12]; // Buffer size increased for safety
    sprintf(v, "%d", value < Y_CAPACITY ? value : Y_CAPACITY); // Using %d for int, safely format
    strcpy(Plant_Y[Y_pointer][3], v);
    Y_pointer++;
}

void updateZ(int value, int i, char* currdate) {
    if (Z_pointer >= 31) return; // Check to prevent writing outside array bounds
    strcpy(Plant_Z[Z_pointer][0], currdate);
    strcpy(Plant_Z[Z_pointer][1], orders[i][3]);
    strcpy(Plant_Z[Z_pointer][2], orders[i][0]);
    strcpy(Plant_Z[Z_pointer][4], orders[i][1]);
    char v[12]; // Ensuring buffer is adequately sized for any integer value as string
    sprintf(v, "%d", value < Z_CAPACITY ? value : Z_CAPACITY); // Correctly using %d for int values
    strcpy(Plant_Z[Z_pointer][3], v);
    Z_pointer++;
}

// void updateX(int value, int i, char* currdate ){
//     strcpy(Plant_X[X_pointer][0], currdate); // setting current date as the date
//     strcpy(Plant_X[X_pointer][1], orders[i][3]);
//     strcpy(Plant_X[X_pointer][2], orders[i][0]);
//     strcpy(Plant_X[X_pointer][4], orders[i][1]);
//     if(value >= X_CAPACITY){
//     strcpy(Plant_X[X_pointer][3], "300\0");
//     }
//     else{
//         char v[4];
//         sprintf(v,"%ld", value);
//         strcpy(Plant_X[X_pointer][3], v);
//     }
//     X_pointer++;
// }
// void updateY(int value, int i, char* currdate ){
//     strcpy(Plant_Y[Y_pointer][0], currdate); // setting current date as the date
//     strcpy(Plant_Y[Y_pointer][1], orders[i][3]);
//     strcpy(Plant_Y[Y_pointer][2], orders[i][0]);
//     strcpy(Plant_Y[Y_pointer][4], orders[i][1]);
//     if(value >= Y_CAPACITY){
//     strcpy(Plant_Y[Y_pointer][3], "400\0");
//     }
//     else{
//         char v[4];
//         sprintf(v,"%ld", value);
//         strcpy(Plant_Y[Y_pointer][3], v);
//     }
//     Y_pointer++;
// }

// void updateZ(int value, int i, char* currdate ){
//     strcpy(Plant_Z[Z_pointer][0], currdate); // setting current date as the date
//     strcpy(Plant_Z[Z_pointer][1], orders[i][3]);
//     strcpy(Plant_Z[Z_pointer][2], orders[i][0]);
//     strcpy(Plant_Z[Z_pointer][4], orders[i][1]);
//     if(value >= Z_CAPACITY){
//     strcpy(Plant_Z[Z_pointer][3], "500\0");
//     }
//     else{
//         char v[4];
//         sprintf(v,"%ld", value);
//         strcpy(Plant_Z[Z_pointer][3], v);
//     }
//     Z_pointer++;
// }

void calculate_no_days(int *plant_produced_fcfs, int *days_used_fcfs, char orders[100][4][11],int i )
{
    int quantity;
    char* currentdate = startdate;
    sscanf(orders[i][2], "%d", &quantity);
    while(quantity != 0)
    {
        if(quantity <= 700 && quantity >= 501)
        {
            quantity  -= Y_CAPACITY;
            plant_produced_fcfs[1] += 1;
            days_used_fcfs[1] += 1;
            updateY(quantity,i, currentdate);

            quantity -= X_CAPACITY;
            plant_produced_fcfs[0] += 1;
            days_used_fcfs[0] += 1;
            updateX(quantity,i, currentdate);

        }
        if(quantity >= 500 || quantity > 400){
        
            quantity -= Z_CAPACITY;
            plant_produced_fcfs[2] += 1;
            days_used_fcfs[2] += 1;
            updateZ(quantity,i, currentdate);

        }
        if(quantity <= 400 && quantity >= 300)
        {
            quantity  -= Y_CAPACITY;
            plant_produced_fcfs[1] += 1;
            days_used_fcfs[1] += 1;
            updateY(quantity,i, currentdate);


        }
        if(quantity < 300)
        {
            quantity -= X_CAPACITY;
            plant_produced_fcfs[0] += 1;
            days_used_fcfs[0] += 1;
            updateX(quantity,i, currentdate);


        }
        currentdate = get_next_day(currentdate);

    }
    
}


int main(){
    //int orderno = 0; // number of orders
    strcpy(startdate, "2024-06-01");
    strcpy(enddate, "2024-06-10");


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
    int i = 0;
    int plant_produced_fcfs[3] = {0,0,0};
    int days_used_fcfs[3] = {0,0,0};
    for(i = 0;i<6;i++)
    {
        calculate_no_days(plant_produced_fcfs, days_used_fcfs, orders, i );
    }
    
    for (int i = 0; i < 6; i++) {
        printf("Plant_X[%d]:\n", i);
        for (int j = 0; j < 5; j++) {
            printf("    ");
            for (int k = 0; k < 11; k++) {
                printf("%c", Plant_X[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    for (int i = 0; i < 6; i++) {
        printf("Plant_Y[%d]:\n", i);
        for (int j = 0; j < 5; j++) {
            printf("    ");
            for (int k = 0; k < 11; k++) {
                printf("%c", Plant_Y[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    for (int i = 0; i < 6; i++) {
        printf("Plant_Z[%d]:\n", i);
        for (int j = 0; j < 5; j++) {
            printf("    ");
            for (int k = 0; k < 11; k++) {
                printf("%c", Plant_Z[i][j][k]);
            }
            printf("\n");
        }
    }
        printf("\n");
    return 0;
}


    

//uses best-fit
// void FCFS2(int orderno){
//     //Where 0 = X (300), 1 = Y (400), 2 = Z (500)
//     int plant_produced[3] = {0,0,0};
//     int days_used[3] = {0,0,0};
//     int i = 0;
//     for(i = 0; i < orderno; i++){
//         int daysrequired = calculate_no_days(plant_produced, days_used, orders, Plant_X, Plant_Y, Plant_Z, i); //calculating the number of days required for order index i
//     }
// }



// void RR(int orderno){ //round-robin giving 1 day to each product
//     char* currdate = startdate;
//     int round = 0; //round number
//     int done; // number of orders completed
//     while(1){ //loop until done
//     done = 0; //checks how many orders completed each round
//     int j; //round robin behaviour

//     int plant_produced[3] = {0,0,0};
//     int days_used[3] = {0,0,0};

//     for(j = 0; j < orderno; j++){
//         int todo = atoi(orders[j][2]) - ((X_CAPACITY + Y_CAPACITY + Z_CAPACITY) * round);
//         if(todo > 0){
//             if(todo!=0 && todo > 500 && todo <= 700){
//                 updateY(todo, j, currdate);
//                 todo -=400;
//                 updateX(todo,j, currdate);
//                 todo = 0;
//             }
//             if(todo!= 0 && todo>400){
//                 updateZ(todo,j,currdate);
//                 if(todo >= Z_CAPACITY){
//                     todo -= Z_CAPACITY;
//                     }
//                     else{
//                         todo = 0;
//                     }
//             }
//             if(todo!=0 && todo>300 && todo<=400 || todo>400){
//                 updateY(todo,j,currdate);
//                 if(todo >= Y_CAPACITY){
//                     todo -= Y_CAPACITY;
//                     }
//                 else{
//                     todo = 0;
//                 }
//             }
//             if(todo != 0 && todo<=300 || todo>300){
//                 updateX(todo,j,currdate);
//                 if(todo >= X_CAPACITY){
//                     todo -= X_CAPACITY;
//                     }
//                 else{
//                     todo = 0;
//                 }
//             }

//         currdate = get_next_day(currdate); // change curr date only when there is something being produced
//         }
//         else{
//             done++;
//         }
//     }
//     if(done == orderno){
//         break;
//     }
//     round++;
//     }
// }