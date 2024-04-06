#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define X_CAPACITY 300
#define Y_CAPACITY 400
#define Z_CAPACITY 500
#define ARGV_SIZE 100
#define MAX_ORDERS 50 // placeholder number for max number of orders
#define MAX_PLANTS 30

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

int main(){
    int orderno = 0; // number of orders
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
    
    
    RR(6);
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
        printf("\n");
    }
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

}


    // int calculate_no_days(int *plant_produced, int *days_used, char orders[100][4][11], char Plant_X[30][4][11], char Plant_Y[30][4][11], char Plant_Z[30][4][11], int i )
    // {
    //     int quantity;
    //     char Starting_date[11];
    //     char product_No[11], due_Date[11], product_Name[11];
    //     strcpy(product_No, orders[i][0]);
    //     strcpy(due_Date, orders[i][1]);
    //     sscanf(orders[i][2], "%d", &quantity);
    //     strcpy(product_Name, orders[i][3]);
    //     strcpy(Starting_date, startdate);
    //     while(quantity != 0)
    //     {
    //         if(quantity <= 700 || quantity >= 501)
    //         {
    //             quantity  -= Y_CAPACITY;
    //             plant_produced[1] += 1;
    //             days_used[1] += 1;
    //             strcpy(Plant_Y[i][0], Starting_date);
    //             strcpy(Plant_Y[i][1], product_Name);
    //             strcpy(Plant_Y[i][2], product_No);
    //             strcpy(Plant_Y[i][2], quantity);
    //             strcpy(Plant_Y[i][3], due_Date);

    //             quantity -= X_CAPACITY;
    //             plant_produced[0] += 1;
    //             days_used[0] += 1;
    //             strcpy(Plant_X[i][0], Starting_date);
    //             strcpy(Plant_X[i][1], product_Name);
    //             strcpy(Plant_X[i][2], product_No);
    //             strcpy(Plant_X[i][2], quantity);
    //             strcpy(Plant_X[i][3], due_Date);


    //         }
    //         else if(quantity >= 500){
            
    //             quantity -= Z_CAPACITY;
    //             plant_produced[2] += 1;
    //             days_used[2] += 1;
    //             strcpy(Plant_Z[i][0], Starting_date);
    //             strcpy(Plant_Z[i][1], product_Name);
    //             strcpy(Plant_Z[i][2], product_No);
    //             strcpy(Plant_Z[i][2], quantity);
    //             strcpy(Plant_Z[i][3], due_Date);
    //         }
    //         else if(quantity <= 400 && quantity >= 300)
    //         {
    //             quantity  -= Y_CAPACITY;
    //             plant_produced[1] += 1;
    //             days_used[1] += 1;
    //             strcpy(Plant_Y[i][0], Starting_date);
    //             strcpy(Plant_Y[i][1], product_Name);
    //             strcpy(Plant_Y[i][2], product_No);
    //             strcpy(Plant_Y[i][2], quantity);
    //             strcpy(Plant_Y[i][3], due_Date);

    //         }
    //         else{
    //             quantity -= X_CAPACITY;
    //             plant_produced[0] += 1;
    //             days_used[0] += 1;
    //             strcpy(Plant_X[i][0], Starting_date);
    //             strcpy(Plant_X[i][1], product_Name);
    //             strcpy(Plant_X[i][2], product_No);
    //             strcpy(Plant_X[i][2], quantity);
    //             strcpy(Plant_X[i][3], due_Date);

    //         }

    //     }
    // }




    

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

void updateX(int value, int i, char* currdate ){
    strcpy(Plant_X[X_pointer][0], currdate); // setting current date as the date
    strcpy(Plant_X[X_pointer][1], orders[i][3]);
    strcpy(Plant_X[X_pointer][2], orders[i][0]);
    strcpy(Plant_X[X_pointer][4], orders[i][1]);
    if(value >= X_CAPACITY){
    strcpy(Plant_X[X_pointer][3], "300\0");
    }
    else{
        char v[4];
        sprintf(v,"%ld", value);
        strcpy(Plant_X[X_pointer][3], v);
    }
    X_pointer++;
}
void updateY(int value, int i, char* currdate ){
    strcpy(Plant_Y[Y_pointer][0], currdate); // setting current date as the date
    strcpy(Plant_Y[Y_pointer][1], orders[i][3]);
    strcpy(Plant_Y[Y_pointer][2], orders[i][0]);
    strcpy(Plant_Y[Y_pointer][4], orders[i][1]);
    if(value >= Y_CAPACITY){
    strcpy(Plant_Y[Y_pointer][3], "400\0");
    }
    else{
        char v[4];
        sprintf(v,"%ld", value);
        strcpy(Plant_Y[Y_pointer][3], v);
    }
    Y_pointer++;
}

void updateZ(int value, int i, char* currdate ){
    strcpy(Plant_Z[Z_pointer][0], currdate); // setting current date as the date
    strcpy(Plant_Z[Z_pointer][1], orders[i][3]);
    strcpy(Plant_Z[Z_pointer][2], orders[i][0]);
    strcpy(Plant_Z[Z_pointer][4], orders[i][1]);
    if(value >= Z_CAPACITY){
    strcpy(Plant_Z[Z_pointer][3], "500\0");
    }
    else{
        char v[4];
        sprintf(v,"%ld", value);
        strcpy(Plant_Z[Z_pointer][3], v);
    }
    Z_pointer++;
}

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
//         int todo = atoi(orders[j][2]);
//         if(todo > (X_CAPACITY + Y_CAPACITY + Z_CAPACITY) * round){
//             if(todo > 500 && todo <= 700){
//                 updateY(todo, j, currdate);
//                 todo -=400;
//                 updateX(todo,j, currdate);
//                 todo = 0;
//             }
//             if(todo>400){
//                 updateZ(todo,j,currdate);
//                 if(todo >= Z_CAPACITY){
//                     todo -= Z_CAPACITY;
//                     }
//                     else{
//                         todo = 0;
//                     }
//             }
//             if(todo>300 && todo<=400){
//                 updateY(todo,j,currdate);
//                 if(todo >= Y_CAPACITY){
//                     todo -= Y_CAPACITY;
//                     }
//                 else{
//                     todo = 0;
//                 }
//             }
//             if(todo<=300){
//                 updateX(todo,j,currdate);
//                 if(todo >= X_CAPACITY){
//                     todo -= X_CAPACITY;
//                     }
//                 else{
//                     todo = 0;
//                 }
//             }
//         }
//         else{
//             done++;
//         }
//     }
//     get_next_day(currdate);
//     if(done == orderno){
//         break;
//     }
//     round++;
    

//     }
// }


void RR(int orderno){ //round-robin giving 1 day to each product
    char* currdate = startdate;
    int round = 0; //round number
    int done; // number of orders completed
    while(1){ //loop until done
    done = 0; //checks how many orders completed each round
    int j; //round robin behaviour

    int plant_produced[3] = {0,0,0};
    int days_used[3] = {0,0,0};

    for(j = 0; j < orderno; j++){
        int todo = atoi(orders[j][2]) - ((X_CAPACITY + Y_CAPACITY + Z_CAPACITY) * round);
        if(todo > 0){
            if(todo!=0 && todo > 500 && todo <= 700){
                updateY(todo, j, currdate);
                todo -=400;
                updateX(todo,j, currdate);
                todo = 0;
            }
            if(todo!= 0 && todo>400){
                updateZ(todo,j,currdate);
                if(todo >= Z_CAPACITY){
                    todo -= Z_CAPACITY;
                    }
                    else{
                        todo = 0;
                    }
            }
            if(todo!=0 && todo>300 && todo<=400 || todo>400){
                updateY(todo,j,currdate);
                if(todo >= Y_CAPACITY){
                    todo -= Y_CAPACITY;
                    }
                else{
                    todo = 0;
                }
            }
            if(todo != 0 && todo<=300 || todo>300){
                updateX(todo,j,currdate);
                if(todo >= X_CAPACITY){
                    todo -= X_CAPACITY;
                    }
                else{
                    todo = 0;
                }
            }

        currdate = get_next_day(currdate); // change curr date only when there is something being produced
        }
        else{
            done++;
        }
    }
    if(done == orderno){
        break;
    }
    round++;
    }
}