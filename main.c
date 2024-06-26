#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define X_CAPACITY 300
#define Y_CAPACITY 400
#define Z_CAPACITY 500
#define ARGV_SIZE 100
#define MAX_ORDERS 50 // placeholder number for max number of orders
#define MAX_PLANTS 30
#define PLANT_ATTRIBUTES 5

char argv[ARGV_SIZE][100];
// char startDate[MAX_ORDERS];
// char endDate[MAX_ORDERS];
char startdate[11]; // period start date
char enddate[11]; // period end date


int scheduler_to_main[2];
int main_to_scheduler[2];

char orders[1000][4][11]; 
char Plant_X[1000][5][11];// Assuming 100 orders max, 4 attributes each, 11 characters max per attribute
char Plant_Y[1000][5][11];
char Plant_Z[1000][5][11];
char rejected_Products[1000][4][11];// Array to keep track of recjected product and right now we have only  
//assumed max 50 products can be rejected but could increase if desired
int orderno2 = 0;

int rejected = 0; // To keep track of rejected products
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

void updateX(int value, int i, char* currdate ){
    char buf[80];
    write(scheduler_to_main[1],"X",2);
    
    write(scheduler_to_main[1], currdate,11);
    read(main_to_scheduler[0],buf,5);
    write(scheduler_to_main[1], orders[i][3],11);
    read(main_to_scheduler[0],buf,5);
    write(scheduler_to_main[1], orders[i][0],11);
    read(main_to_scheduler[0],buf,5);
    write(scheduler_to_main[1], orders[i][1],11);
    read(main_to_scheduler[0],buf,5);

    if(value >= X_CAPACITY){
        write(scheduler_to_main[1], "300\0",4);
        read(main_to_scheduler[0],buf,5);
    }
    else{
        char v[4];
        sprintf(v,"%ld", value);
        strcpy(Plant_X[X_pointer][3], v);
        write(scheduler_to_main[1], v,11);
        read(main_to_scheduler[0],buf,5);
    }
    X_pointer++;
}

void updateY(int value, int i, char* currdate ){

    char buf[80];
    write(scheduler_to_main[1],"Y",2);
    
    write(scheduler_to_main[1], currdate,11);
    read(main_to_scheduler[0],buf,5);
    write(scheduler_to_main[1], orders[i][3],11);
    read(main_to_scheduler[0],buf,5);
    write(scheduler_to_main[1], orders[i][0],11);
    read(main_to_scheduler[0],buf,5);
    write(scheduler_to_main[1], orders[i][1],11);
    read(main_to_scheduler[0],buf,5);

    if(value >= Y_CAPACITY){
        write(scheduler_to_main[1], "400\0",4);
        read(main_to_scheduler[0],buf,5);
    }
    else{
        char v[4];
        sprintf(v,"%ld", value);
        strcpy(Plant_Y[Y_pointer][3], v);
        write(scheduler_to_main[1], v,11);
        read(main_to_scheduler[0],buf,5);
    }

    Y_pointer++;
}


void updateZ(int value, int i, char* currdate ){
    char buf[80];

    write(scheduler_to_main[1],"Z",2);
    
    write(scheduler_to_main[1], currdate,11);
    read(main_to_scheduler[0],buf,5);
    write(scheduler_to_main[1], orders[i][3],11);
    read(main_to_scheduler[0],buf,5);
    write(scheduler_to_main[1], orders[i][0],11);
    read(main_to_scheduler[0],buf,5);
    write(scheduler_to_main[1], orders[i][1],11);
    read(main_to_scheduler[0],buf,5);

    if(value >= Z_CAPACITY){
        write(scheduler_to_main[1], "500\0",4);
        read(main_to_scheduler[0],buf,5);
    }
    else{
        char v[4];
        sprintf(v,"%ld", value);
        strcpy(Plant_Z[Z_pointer][3], v);
        write(scheduler_to_main[1], v,11);
        read(main_to_scheduler[0],buf,5);
    }
    Z_pointer++;

}

int convertDateToNumber(char* date) {
    // Assuming the date string has a length of 10
    if (strlen(date) == 10 && date[4] == '-' && date[7] == '-') 
    {
        int number = 0;
        int i;
        for (i = 0; i < 10; i++) {
            if (i != 4 && i != 7) {
                number = number * 10 + (date[i] - '0');
            }
        }
        return number;
    } 
    else 
    {
        return -1;
    }
}

void sortOrders(char orders[100][4][11], int orderno) {
    char temp[4][11];
    int i;
    for (i = 0; i < orderno -1; ++i) {
        int j;
        for (j = i + 1; j < orderno; ++j) {
            // Convert the third elements from char arrays to integers
            int valueI = atoi(orders[i][2]);
            int valueJ = atoi(orders[j][2]);
            
            // Compare and swap if out of order
            if (valueI > valueJ) {
                // Swap orders[i] and orders[j]
                int k;
                for (k = 0; k < 4; ++k) {
                    strcpy(temp[k], orders[i][k]);
                    strcpy(orders[i][k], orders[j][k]);
                    strcpy(orders[j][k], temp[k]);
                }
            }
        }
    }
}

   
void store_Rejected_Products(int i)
{
    strcpy(rejected_Products[rejected][0], orders[i][0]);
    strcpy(rejected_Products[rejected][1], orders[i][3]);
    strcpy(rejected_Products[rejected][2], orders[i][1]);
    strcpy(rejected_Products[rejected][3], orders[i][2]);
    rejected++;
}

void print_Rejected_Products(){
    printf("rejected_Products\n");
    int i;
    for ( i = 0; i < 2; i++) 
    {
        int j;
        for (j = 0; j < 4; j++) 
        {
            printf("    ");
            int k;
            for (k = 0; k < 11; k++) 
            {
                printf("%c", rejected_Products[i][j][k]);
            }
            printf("\n");
        }
        printf("\n");
    }
    return;
}

int isLeapYear(int year) {
    if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
        return 1;
    }
    return 0;
}

int getDaysInMonth(int year, int month) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            if (isLeapYear(year)) {
                return 29;
            } else {
                return 28;
            }
        default:
            return 0;
    }
}

void IncrementDate(char* dateStr) {
    int year, month, day;
    sscanf(dateStr, "%d-%d-%d", &year, &month, &day);

    int daysInMonth = getDaysInMonth(year, month);
    day++;
    if (day > daysInMonth) {
        day = 1;
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }

    sprintf(dateStr, "%04d-%02d-%02d", year, month, day);
}

void printPlantDetails(char* plantName, char* startDate, char* endDate, char Plant[30][5][11], int len, int mx){
    
    int produced_amount;
    if(strcmp(plantName,"Plant_X")==0){
        produced_amount = X_CAPACITY;
    }
    else if(strcmp(plantName,"Plant_Y")==0){
        produced_amount = Y_CAPACITY;
    }
    else if(strcmp(plantName,"Plant_Z")==0){
        produced_amount = Z_CAPACITY;
    }
    

    printf(" __________________________________________________________________________\n");
    printf("|  %s (%d per day)                                                   |\n", plantName, produced_amount);
    printf("|  %s to %s                                                |\n", startdate, enddate);

    printf("|__________________________________________________________________________|\n");
    printf("|     Date     | Product Name |  Order Name  |   Quantity   |   Due Date   |\n");
    printf("|              |              |              |  (Produced)  |              |\n");
    printf("|______________|______________|______________|______________|______________|\n");

    int size = 13;
    char current_date[11];
    strcpy(current_date, startDate);

    int i;
    for(i = 0; i <= mx; i++){
        int flag = 0;
        printf("| %-*s", size, current_date);
        int j;
        for(j = 0; j < len; j++){
            if(strcmp(current_date, Plant[j][0]) == 0) {
                printf("| %-*s", size, Plant[j][1]);
                printf("| %-*s", size, Plant[j][2]);
                printf("| %-*s", size, Plant[j][3]);
                printf("| %-*s|", size, Plant[j][4]);
                flag = 1;
            }
        }
        if(flag == 0){
            printf("| %-*s| %-*s| %-*s| %-*s|", size, "NA", size, "", size, "", size, "");
        }
        printf("\n");
        printf("|______________|______________|______________|______________|______________|\n");
        IncrementDate(current_date);
    }
    printf("\n\n");
}

void printPlant(char* startDate, char* endDate, int orderNo){
    int mx = 0;
    int i;
    for(i = 0; i < X_pointer; i++){
        int tmp = convertDateToNumber(Plant_X[i][0]) - convertDateToNumber(startDate);
        if(mx < tmp){
            mx = tmp;
        }
    }
    for(i = 0; i < Y_pointer; i++){
        int tmp = convertDateToNumber(Plant_Y[i][0]) - convertDateToNumber(startDate);
        if(mx < tmp){
            mx = tmp;
        }
    }
    for(i = 0; i < Z_pointer; i++){
        int tmp = convertDateToNumber(Plant_Z[i][0]) - convertDateToNumber(startDate);
        if(mx < tmp){
            mx = tmp;
        }
    }
    printf("%d\n", mx);
    printPlantDetails("Plant_X", startdate, enddate, Plant_X, orderNo, mx);
    printPlantDetails("Plant_Y", startdate, enddate, Plant_Y, orderNo, mx);
    printPlantDetails("Plant_Z", startdate, enddate, Plant_Z, orderNo, mx);
}

float performanceCalculation(char* plantName, char Plant[30][5][11], int pointerLen, char orders[100][4][11], int orderNo, int produceRate, FILE* text){
    int totalDays = 0, totalQuantity = 0;
    int i;
    for(i = 0; i < orderNo; i++){
        int j;
        for(j = 0; j < pointerLen; j++){
            if(strcmp(orders[i][0], Plant[j][2]) == 0) {
                totalDays ++;
                totalQuantity += atoi(Plant[j][3]);
            }
        }
    }
    float tmp2 = produceRate * (convertDateToNumber(enddate) - convertDateToNumber(startdate) + 1);
    float tmp = totalQuantity / tmp2;

    tmp *= 100;
    printf("%.1f %.3f\n", tmp2, tmp);

    fprintf(text, "%s:\n", plantName);
    fprintf(text, "        Number of days in use:                    %d days\n", totalDays);
    fprintf(text, "        Number of products produced:              %d (in total)\n", totalQuantity);
    fprintf(text, "        Utilization of the plant:                 %.3f %c\n\n", tmp, '%');

    return tmp;
}

void printAnalysisReport(char* command, char* textFile, char orders[100][4][11], char rejectedProducts[50][4][11]) {
    FILE* text = fopen(textFile, "w");
    fprintf(text, "***PLS Schedule Analysis Report***\n\n");
    fprintf(text, "Algorithm used: %s\n\n", command);
    fprintf(text, "There are %d Orders ACCEPTED. Details are as follows:\n\n", orderno2 - rejected);

    fprintf(text, "ORDER NUMBER  START         END            DAYS      QUANTITY     PLANT\n");
    fprintf(text, "==========================================================================\n");  
    int i;
    for(i = 0; i < orderno2; i++) {
        int j;
        int X_days = 0, Y_days = 0, Z_days = 0;
        char* X_start_date;
        char *Y_start_date;
        char *Z_start_date;

        int X_quantity = 0, Y_quantity = 0, Z_quantity = 0;
        for(j = 0; j < X_pointer; j++) {
            if(strcmp(orders[i][0], Plant_X[j][2]) == 0) {
                if(X_days == 0){
                    X_start_date = Plant_X[j][0];
                }
                X_quantity += atoi(Plant_X[j][3]);
                X_days++;
            }
        }
        for(j = 0; j < Y_pointer; j++) {
            if(strcmp(orders[i][0], Plant_Y[j][2]) == 0) {
                if(Y_days == 0){
                    Y_start_date = Plant_Y[j][0];

                }
                Y_quantity += atoi(Plant_Y[j][3]);
                Y_days++;
            }
        }
        for(j = 0; j < Z_pointer; j++) {
            if(strcmp(orders[i][0], Plant_Z[j][2]) == 0) {
                if(Z_days == 0){
                    Z_start_date = Plant_Z[j][0];

                }
                Z_quantity += atoi(Plant_Z[j][3]);
                Z_days++;
            }
        }

        printf("X_days = %d\nY_days = %d\nZ_days = %d\n", X_days, Y_days, Z_days);
        if(X_days != 0){
            char* X_end_date = X_start_date;
            int k;
            for(k = 0 ; k < X_days-1; k++){
                X_end_date = get_next_day(X_end_date);
            }

            fprintf(text, "%s          %s    %s        %d        %d ", orders[i][0], X_start_date, X_end_date, X_days, X_quantity);
            fprintf(text, "       Plant_X\n");
        }
        if(Y_days != 0){
            char* Y_end_date = Y_start_date;
            int k;
            for(k = 0 ; k < Y_days-1; k++){
                Y_end_date = get_next_day(Y_end_date);
            }
            fprintf(text, "%s          %s    %s        %d        %d ", orders[i][0], Y_start_date, Y_end_date, Y_days, Y_quantity);
            fprintf(text, "       Plant_Y\n");
        }
        if(Z_days != 0){
            char* Z_end_date = Z_start_date;
            int k;
            for(k = 0 ; k < Z_days-1; k++){
                Z_end_date = get_next_day(Z_end_date);

            }
            fprintf(text, "%s          %s    %s        %d        %d ", orders[i][0], Z_start_date, Z_end_date, Z_days, Z_quantity);
            fprintf(text, "       Plant_Z\n");
        }
    }

    fprintf(text, "\n");
    fprintf(text, "                                   - End -                               \n");
    fprintf(text, "==========================================================================\n");
    fprintf(text, "\n\n");

    fprintf(text, "There are %d Orders REJECTED. Details are as follows:\n\n", rejected);
    fprintf(text, "ORDER NUMBER      PRODUCT NAME      DUE DATE      QUANTITY\n");
    fprintf(text, "==========================================================================\n");
    
    for(i = 0; i < orderno2; i++){
        int j;
        for(j = 0; j < rejected; j++){
            if(strcmp(orders[i][0], rejectedProducts[j][0]) == 0){
                fprintf(text, "%s              %s             ", rejectedProducts[j][0], rejectedProducts[j][1]);
                fprintf(text, "%s      %s\n", rejectedProducts[j][2], rejectedProducts[j][3]);
            }
        }
    }
    fprintf(text, "\n");
    fprintf(text, "                                   - End -                               \n");
    fprintf(text, "==========================================================================\n");
    fprintf(text, "\n\n");

    fprintf(text, "***PERFORMANCE\n\n");
    
    float utilPercentage = 0.0;
    utilPercentage += performanceCalculation("Plant_X", Plant_X, X_pointer, orders, orderno2, 300, text);
    utilPercentage += performanceCalculation("Plant_Y", Plant_Y, Y_pointer, orders, orderno2, 400, text);
    utilPercentage += performanceCalculation("Plant_Z", Plant_Z, Z_pointer, orders, orderno2, 500, text);

    fprintf(text, "Overall of utilization:                           %.2f %c\n\n", (utilPercentage / 3), '%');
    fclose(text);
}

void FCFS(int orderno)
{
    int quantity;
    char* currentdate = startdate;
    int plant_produced_fcfs[3] = {0,0,0};
    int days_used_fcfs[3] = {0,0,0};
    int i = 0;
    for(i = 0; i < orderno; i++)
    {
        sscanf(orders[i][2], "%d", &quantity);
        char using_Currdate[11], using_Duedate[11];
        strcpy(using_Currdate, currentdate);
        strcpy(using_Duedate, orders[i][1]);
        int number_using_Currdate = convertDateToNumber(using_Currdate);
        int number_using_Duedate = convertDateToNumber(using_Duedate);

        int remaining_Days = (number_using_Duedate + 1) -  number_using_Currdate;

        if (quantity > remaining_Days * 1200)
        {
            store_Rejected_Products(i);
            continue;
            }
        
        while(quantity != 0)
        {
            if(quantity!=0 && quantity <= 700 && quantity > 500)
            {
                updateY(quantity,i, currentdate);
                quantity  -= 400;
                plant_produced_fcfs[1] += 1;
                days_used_fcfs[1] += 1;
                updateX(quantity,i, currentdate);
                plant_produced_fcfs[0] += 1;
                days_used_fcfs[0] += 1;
                quantity = 0;

            }
            if(quantity!=0 && quantity > 400){
            
                updateZ(quantity,i, currentdate);
                plant_produced_fcfs[2] += 1;
                days_used_fcfs[2] += 1;
                if(quantity >= Z_CAPACITY)
                {
                    quantity -= Z_CAPACITY;
                }
                else
                {
                    quantity = 0;
                }

            }
            if(quantity!=0 && quantity <= 400 && quantity > 300 || quantity >400)
            {
                updateY(quantity,i, currentdate);
                plant_produced_fcfs[1] += 1;
                days_used_fcfs[1] += 1;
                if(quantity >= Y_CAPACITY)
                {
                    quantity -= Y_CAPACITY;
                }
                else
                {
                    quantity = 0;
                }



            }
            if(quantity!=0 && quantity<= 300 || quantity > 300)
            {
                updateX(quantity,i, currentdate);
                plant_produced_fcfs[0] += 1;
                days_used_fcfs[0] += 1;
                if(quantity >= X_CAPACITY)
                {
                    quantity -= X_CAPACITY;
                }
                else
                {
                    quantity = 0;
                }


            }
            currentdate = get_next_day(currentdate);
        }
    }

}


void copyOrdersArray(char source[100][4][11], char destination[100][4][11], int orderno) {
    int i;
    for ( i = 0; i < orderno; ++i) {
        int j;
        for (j = 0; j < 4; ++j) {
            strcpy(destination[i][j], source[i][j]);
        }
    }
}
void SJF(int orderno){
    // Auxiliary array of pointers for sorted order
    char revert[100][4][11];
    copyOrdersArray(orders,revert,orderno);
    // Sort without changing the original array

    sortOrders(orders,orderno);
    int i;
    // for (i = 0; i < orderno; i++) {
    //     printf("Order %d:\n", i + 1);
    //     int j;
    //     for (j = 0; j < 4; j++) {
    //         // Assuming the strings are null-terminated,
    //         // you can use %s to print the string at arr[i][j]
    //         printf("\t%s\n", orders[i][j]);
    //     }
    //     printf("\n"); // Print a newline for better separation between orders
    // }
    FCFS(orderno);
    copyOrdersArray(revert, orders,orderno);
}

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

// auxiliary parts necessary for bonus scheduler
typedef struct {
    int year;
    int month;
    int day;
    int quantity;
} Order;

void extractOrder(char order[4][11], Order *o) {
    // auxiliary function for the bonus scheduler
    sscanf(order[1], "%d-%d-%d", &o->year, &o->month, &o->day);
    sscanf(order[2], "%d", &o->quantity);
}

int compareOrders(Order a, Order b) {
    // auxiliary function for the bonus scheduler
    if (a.year != b.year)
        return a.year - b.year;
    if (a.month != b.month)
        return a.month - b.month;
    if (a.day != b.day)
        return a.day - b.day;
    return a.quantity - b.quantity;
}

void swapOrders(char orders[100][4][11], int i, int j) {
    // auxiliary function for the bonus scheduler
    char temp[4][11];
    int k;

    for (k = 0; k < 4; k++) {
        strcpy(temp[k], orders[i][k]);
        strcpy(orders[i][k], orders[j][k]);
        strcpy(orders[j][k], temp[k]);
    }
}
// BONUS SCHEDULER - Closest Deadlines ordered by shortest jobs
void deadlinePriority(int orderno, char orders[100][4][11]) {
    // Auxiliary array of pointers for sorted order
    char revert[100][4][11];
    copyOrdersArray(orders,revert,orderno);

    // Sort the orders array with deadlines as the main priority and order size as the secondary priority
    int i, j;
    Order current, next;

    for (i = 0; i < orderno - 1; i++) {
        for (j = 0; j < orderno - i - 1; j++) {
            extractOrder(orders[j], &current);
            extractOrder(orders[j + 1], &next);

            if (compareOrders(next, current) < 0) {
                // Swap current and next
                swapOrders(orders, j, j + 1);
            }
        }
    }

    FCFS(orderno);
    copyOrdersArray(revert, orders,orderno);
}

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


void processBatchFile(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }
    
    char line[50];
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Remove the newline character from the line
        line[strcspn(line, "\n")] = '\0';

        // Check if the line starts with "addORDER"
        if (strncmp(line, "addORDER", 8) == 0) {
            // Process the order
            char *token = strtok(line, " ");
            token = strtok(NULL, " "); // Skip the "addORDER" command

            int field = 0;
            while (token != NULL && field < 4) {
                if (orderno2 < 0 || orderno2 >= MAX_ORDERS) {
                    fprintf(stderr, "Invalid order number\n");
                    return;
                }
                strcpy(orders[orderno2][field], token);
                token = strtok(NULL, " ");
                field++;
            }

            if (field != 4) {
                fprintf(stderr, "Invalid order format in batch file\n");
                continue;
            }

            // printf("The orders[%d][0]: %s\n", orderno2, orders[orderno2][0]);
            // printf("The orders[%d][1]: %s\n", orderno2, orders[orderno2][1]);
            // printf("The orders[%d][2]: %s\n", orderno2, orders[orderno2][2]);
            // printf("The orders[%d][3]: %s\n", orderno2, orders[orderno2][3]);

            orderno2++;
        } else {
            fprintf(stderr, "Invalid command in batch file: %s\n", line);
        }
    }

    fclose(fp);
}


int main(){

    printf("\t\t\t\t\t\tWELCOME TO PLS!\n");
    char command1[] = "addPEIOD";
    char command2[] = "addORDER";
    char command3[] = "addBATCH";
    char command4[] = "runPLS";
    char command5[] = "printREPORT";
    char command6[] = "exitPLS";

    pipe(scheduler_to_main);
    pipe(main_to_scheduler);


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
                    // printf("The StartDate is: %s\n", startdate);
                    // printf("The EndDate is: %s\n", enddate);
                } else {
                    fprintf(stderr, "Invalid command format for addPERIOD\n");
                }
            } else {
                fprintf(stderr, "Invalid command format for addPERIOD\n");
            }
        } else if (substring(input, command2)) {
            // Update orders
            if (orderno2 >= MAX_ORDERS) {
                fprintf(stderr, "Maximum number of orders reached\n");
                continue;
            }

            char *token = strtok(input, " ");
            token = strtok(NULL, " "); // Skip the command
            int field = 0;
            while (token != NULL && field < 4) {
                strcpy(orders[orderno2][field], token);
                token = strtok(NULL, " ");
                field++;
            }

            if (field != 4) {
                fprintf(stderr, "Invalid command format for addORDER\n");
                continue;
            }
            printf ("The orders[%d][0]: %s\n", orderno2, orders[orderno2][0]);
            printf ("The orders[%d][1]: %s\n", orderno2, orders[orderno2][1]);
            printf ("The orders[%d][2]: %s\n", orderno2, orders[orderno2][2]);
            printf ("The orders[%d][3]: %s\n", orderno2, orders[orderno2][3]);

            orderno2++;
        } else if (substring(input, command3)) {
            // Read the batch file
            char *token = strtok(input, " ");
            token = strtok(NULL, " "); // Skip the command
            if (token != NULL) {
                printf("Reading batch data from file: %s\n", token);
                processBatchFile(token);

                // Print the orders added from the batch file
            } else {
                fprintf(stderr, "Invalid command format for addBATCH\n");
            }
        }
        else if (substring(input, command4)){
            // Running the algorithm
            char scheduler[5][20];
            char *token = strtok(input, " ");
            token = strtok(NULL, " ");
            int field = 0;
            while (token != NULL && field < 5) {
                strcpy(scheduler[field], token);
                token = strtok(NULL, " ");
                field++;
            } // Skip the command
                printf("Running the production planning algorithm: %s.\n", scheduler[0]);
                // printf("ORDERNO2 = %d\n", orderno2);
                int childpid = fork();
                if(childpid ==0){
                    close(scheduler_to_main[0]);
                    if(strcmp(scheduler[0], "FCFS")==0){
                        FCFS(orderno2);
                    }
                    else if(strcmp(scheduler[0],"SJF")==0){
                        SJF(orderno2);
                    }
                    else if(strcmp(scheduler[0],"SID")==0){
                        deadlinePriority(orderno2, orders);
                    }
                    write(scheduler_to_main[1],"F",2);
                    char buff[10];
                    exit(0);
                    //END
                }
                else{ //PIPE RUNNING IN PARENT
                    close(main_to_scheduler[0]);
                    char buf[80];
                    int j;
                    char temp[5][11];
                    while(1){
                        read(scheduler_to_main[0],buf,2);
                        if(buf[0] == 'F'){
                            wait();
                            break;
                        }
                        if(buf[0] == 'X'){
                        for(j = 0; j <5; j++){
                            int n = read(scheduler_to_main[0],buf,80);
                            strcpy(temp[j],buf);
                            write(main_to_scheduler[1], "done", 5);
                        }
                        strcpy(Plant_X[X_pointer][0], temp[0]);
                        strcpy(Plant_X[X_pointer][1], temp[1]);
                        
                        strcpy(Plant_X[X_pointer][2], temp[2]);

                        strcpy(Plant_X[X_pointer][4], temp[3]);

                        strcpy(Plant_X[X_pointer][3], temp[4]);

                        X_pointer++;
                        }
                        else if(buf[0] == 'Y'){
                        for(j = 0; j <5; j++){
                            int n = read(scheduler_to_main[0],buf,80);
                            strcpy(temp[j],buf);
                            write(main_to_scheduler[1], "done", 5);
                        }
                        strcpy(Plant_Y[Y_pointer][0], temp[0]); // setting current date as the date
                        strcpy(Plant_Y[Y_pointer][1], temp[1]);
                        strcpy(Plant_Y[Y_pointer][2], temp[2]);
                        strcpy(Plant_Y[Y_pointer][4], temp[3]);
                        strcpy(Plant_Y[Y_pointer][3], temp[4]);
                        Y_pointer++;
                        }
                        else if(buf[0] == 'Z'){
                        for(j = 0; j <5; j++){
                            int n = read(scheduler_to_main[0],buf,80);
                            strcpy(temp[j],buf);
                            write(main_to_scheduler[1], "done", 5);
                        }
                        
                        strcpy(Plant_Z[Z_pointer][0], temp[0]); // setting current date as the date
                        strcpy(Plant_Z[Z_pointer][1], temp[1]);
                        strcpy(Plant_Z[Z_pointer][2], temp[2]);
                        strcpy(Plant_Z[Z_pointer][4], temp[3]);
                        strcpy(Plant_Z[Z_pointer][3], temp[4]);
                        Z_pointer++;
                        }
                    }
                    //PARENT PROCESS
                    wait(NULL);

                
                    
                }

                
                printPlant(startdate,enddate,orderno2);
                printAnalysisReport(scheduler[0], scheduler[4], orders, rejected_Products);
                // Implement the algorithm logic here
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

