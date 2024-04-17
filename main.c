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
#define PLANT_ATTRIBUTES 5

char argv[ARGV_SIZE][100];
// char startDate[MAX_ORDERS];
// char endDate[MAX_ORDERS];
char startdate[11]; // period start date
char enddate[11]; // period end date


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
    for (i = 0; i < 2; i++) 
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

void printPlantDetails(char* plantName, char* startDate, char* endDate, char Plant[30][5][11], int len){
    // Formatting for the Plant
    printf(" __________________________________________________________________________\n");
    printf("|  %s (300 per day)                                                   |\n", plantName);
    printf("|  %s to %s                                                |\n", startdate, enddate);

    printf("|__________________________________________________________________________|\n");
    printf("|     Date     | Product Name |  Order Name  |   Quantity   |   Due Date   |\n");
    printf("|              |              |              |  (Produced)  |              |\n");
    printf("|______________|______________|______________|______________|______________|\n");

    // Printing Plant
    int i;
    for (i = 0; i < len; i++) {
        int j;
        for (j = 0; j < PLANT_ATTRIBUTES; j++) {
            printf("| ");
            int k;
            for (k = 0; k < 11; k++) {
                printf("%c", Plant[i][j][k] ? Plant[i][j][k] : ' ');
                // Need to implement NA checker
            }
            if(j == PLANT_ATTRIBUTES - 1) printf("  |");
            else printf("  ");
        }
        printf("\n");
        printf("|______________|______________|______________|______________|______________|\n");
    }
    printf("\n\n");
}

void performanceCalculation(char* plantName, char Plant[30][5][11], int pointerLen, char orders[100][4][11], int orderNo, int produceRate, float *utilPercentage, int *utilCounter, FILE* text){
    int totalDays = 0, totalQuantity = 0;
    int i;
    for(i = 0; i < orderNo; i++){
        int j;
        for(j = 0; j < pointerLen; j++){
            if(strcmp(orders[i][0], Plant[j][2]) == 0) {
                int diff = convertDateToNumber(Plant[j][4]) - convertDateToNumber(Plant[j][0]);
                totalDays += diff;
                totalQuantity += atoi(Plant[j][3]);
            }
        }
    }
    
    float tmp = totalQuantity / produceRate * totalDays;
    *utilPercentage += tmp;
    (*utilCounter)++;

    fprintf(text, "%s:\n", plantName);
    fprintf(text, "        Number of days in use:                    %d days\n", totalDays);
    fprintf(text, "        Number of products produced:              %d (in total)\n", totalQuantity);
    fprintf(text, "        Utilization of the plant:                 %.1f %c\n\n", tmp, '%');
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
        for(j = 0; j < X_pointer; j++) {
            if(strcmp(orders[i][0], Plant_X[j][2]) == 0) {
                int diff = convertDateToNumber(Plant_X[j][4]) - convertDateToNumber(Plant_X[j][0]);
                fprintf(text, "%s          %s    %s        %d        %s ", Plant_X[j][2], Plant_X[j][0], Plant_X[j][4], diff, Plant_X[j][3]);
                fprintf(text, "       Plant_X\n");
            }
        }
        for(j = 0; j < Y_pointer; j++) {
            if(strcmp(orders[i][0], Plant_Y[j][2]) == 0) {
                int diff = convertDateToNumber(Plant_Y[j][4]) - convertDateToNumber(Plant_Y[j][0]);
                fprintf(text, "%s          %s    %s        %d        %s ", Plant_Y[j][2], Plant_Y[j][0], Plant_Y[j][4], diff, Plant_Y[j][3]);
                fprintf(text, "       Plant_Y\n");
            }
        }
        for(j = 0; j < Z_pointer; j++) {
            if(strcmp(orders[i][0], Plant_Z[j][2]) == 0) {
                int diff = convertDateToNumber(Plant_Z[j][4]) - convertDateToNumber(Plant_Z[j][0]);
                fprintf(text, "%s          %s    %s        %d        %s ", Plant_Z[j][2], Plant_Z[j][0], Plant_Z[j][4], diff, Plant_Z[j][3]);
                fprintf(text, "       Plant_Z\n");
            }
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
    int utilCounter = 0;
    performanceCalculation("Plant_X", Plant_X, X_pointer, orders, orderno2, 300, &utilPercentage, &utilCounter, text);
    performanceCalculation("Plant_Y", Plant_Y, Y_pointer, orders, orderno2, 300, &utilPercentage, &utilCounter, text);
    performanceCalculation("Plant_Z", Plant_Z, Z_pointer, orders, orderno2, 300, &utilPercentage, &utilCounter, text);

    fprintf(text, "Overall of utilization:                           %.1f %c\n\n", (utilPercentage / utilCounter), '%');
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
        // if(remaining_Days == 0)
        // {
        //     remaining_Days++;
        // }

        printf("Remaining days: %d\n", remaining_Days);
        if (quantity > remaining_Days * 1200)
        {
            store_Rejected_Products(i);
            printf("Rejected a product\n");
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

    printPlantDetails("Plant_X", startdate, enddate, Plant_X, 6);
    printPlantDetails("Plant_Y", startdate, enddate, Plant_Y, 6);
    printPlantDetails("Plant_Z", startdate, enddate, Plant_Z, 6);

}


void copyOrdersArray(const char source[100][4][11], char destination[100][4][11], int orderno) {
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
    for (i = 0; i < orderno; i++) {
        printf("Order %d:\n", i + 1);
        int j;
        for (j = 0; j < 4; j++) {
            // Assuming the strings are null-terminated,
            // you can use %s to print the string at arr[i][j]
            printf("\t%s\n", orders[i][j]);
        }
        printf("\n"); // Print a newline for better separation between orders
    }
    FCFS(orderno);
    copyOrdersArray(revert, orders,orderno);
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

    char line[150];
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
                strcpy(orders[orderno2][field], token);
                token = strtok(NULL, " ");
                field++;
            }

            if (field != 4) {
                fprintf(stderr, "Invalid order format in batch file\n");
                continue;
            }

            printf("The orders[%d][0]: %s\n", orderno2, orders[orderno2][0]);
            printf("The orders[%d][1]: %s\n", orderno2, orders[orderno2][1]);
            printf("The orders[%d][2]: %s\n", orderno2, orders[orderno2][2]);
            printf("The orders[%d][3]: %s\n", orderno2, orders[orderno2][3]);

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

    int scheduler_to_main[2];
    int main_to_scheduler[2];

    // pipe(scheduler_to_main);
    // pipe(main_to_scheduler);

    while (1){
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
                int i;
                // Print the orders added from the batch file
                for (i = orderno2 - (MAX_ORDERS - orderno2); i < orderno2; i++) {
                    printf("The orders[%d][0]: %s\n", i, orders[i][0]);
                    printf("The orders[%d][1]: %s\n", i, orders[i][1]);
                    printf("The orders[%d][2]: %s\n", i, orders[i][2]);
                    printf("The orders[%d][3]: %s\n", i, orders[i][3]);
                }
            } else {
                fprintf(stderr, "Invalid command format for addBATCH\n");
            }
        }
        else if (substring(input, command4)) {
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
                printf("ORDERNO2 = %d\n", orderno2);
                // if(fork()==0){
                    if(strcmp(scheduler[0], "FCFS")==0){
                        FCFS(orderno2);
                    }
                    else if(strcmp(scheduler[0],"SJF")==0){
                        SJF(orderno2);
                    }
                    else if(strcmp(scheduler[0],"SID")==0){
                        deadlinePriority(orderno2, orders);
                    }
                    //END
                //     kill(2);
                // }
                // else{
                //     continue;//PARENT PROCESS
                // }
                
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

