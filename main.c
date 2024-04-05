/*
    Author: Osman

    Date:02/04/2024

    Description: A program that takes in data from text files and convert this data into structures.

    TASK 1: Merge sort used to prepare a table for each line. Sorted first by Product ID, then Issue Code, then date and time order (time converted into one numeral)

    TASK 2: Finds all of the issues for each unique product ID's and Line codes combination

    TASK 3: Find earliest occurance of an issue for a product ID across all lines

    TASK 4: Looks through a sorted list of all the issues and counts all the issues across all lines for each product ID

*/

#include <stdio.h>
#include <string.h>

#define MAX_CHAR 101
#define DATA_SIZE 10
#define MAX_ISSUES 40
#define RUN 32

struct date_time {
	int day;
	int hour;
	int min;
	int time;
};

struct issue {
	int code;
	char description[MAX_CHAR];
};

struct resolution {
	int code;
	char description[MAX_CHAR];
};

struct QAlog {
	int line_code; 
	int batch_code;
	struct date_time dt;
	int product_ID;
	struct issue iss;
	struct resolution res;
	int employee_ID;
};

struct issue_list {
    int product_ID;
    int line_code;
    int issuecode_list[DATA_SIZE];
    int issuecode_list_len; //How many issues are found for unique products in line
};

void read_loginfo(struct QAlog *read_struct, FILE **file);
void printStructure (struct QAlog A[]);
void mergesort(struct QAlog A[], int n);
void merge(struct QAlog A[], struct QAlog L[], struct QAlog R[],int nL, int nR);
void findIssues(struct QAlog data[DATA_SIZE], struct issue_list issue_log[MAX_ISSUES], int *count);
void printIssueLog (struct issue_list issue_log[MAX_ISSUES], int count);
int findEarliestOccurance (struct QAlog data[DATA_SIZE], int search);
int summariseData (struct issue_list issue_log[MAX_ISSUES], int count, int);



int main (void){

    //structure for all the lines
    struct QAlog firstline_log[DATA_SIZE];
    struct QAlog secondline_log[DATA_SIZE];
    struct QAlog thirdline_log[DATA_SIZE];
    struct QAlog fourthline_log[DATA_SIZE];

    struct issue_list issue_log[MAX_ISSUES]; //structure for sumarrising the issues across all lines

    int count = 0; //keep track of unique product id and line code combinations
    int result = 0; //keep track of if product code is found for TASK 3
    int search = 0; //valuable to search for
    int buffer = 0; //valuable to store user's input data to move along through the tasks and act as an extra variable for TASK 4

    //OPEN FIRST FILE
    FILE *file = fopen("qalog1.txt", "r");
    read_loginfo(firstline_log, &file);
    fclose(file);

    //OPEN SECOND FILE
    file = fopen("qalog2.txt", "r");
    read_loginfo(secondline_log, &file);
    fclose(file);

    //OPEN THIRD FILE
    file = fopen("qalog3.txt", "r");
    read_loginfo(thirdline_log, &file);
    fclose(file);

    //OPEN FOURTH FILE
    file = fopen("qalog4.txt", "r");
    read_loginfo(fourthline_log, &file);
    fclose(file);

    //Show all the logs at the beginning before doing anyhting
    printf("\nFirst Line Log\n");
    printStructure(firstline_log);
    printf("\nSecond Line Log\n");
    printStructure(secondline_log);
    printf("\nThird Line Log\n");
    printStructure(thirdline_log);
    printf("\nFourth Line Log\n");
    printStructure(fourthline_log);

    printf("Beginning of TASK 1 if you wish to continue, type any number and press enter\n");
    scanf("%d", &buffer);

    //TASK 1: order by product ID, issue code, date and time
    //then display the sorted report
    mergesort(firstline_log, DATA_SIZE);
    mergesort(secondline_log, DATA_SIZE);
    mergesort(thirdline_log, DATA_SIZE);
    mergesort(fourthline_log, DATA_SIZE);
    printf("\nSORTED OUT\n");
    printf("\nFirst Line Log\n");
    printStructure(firstline_log);
    printf("\nSecond Line Log\n");
    printStructure(secondline_log);
    printf("\nThird Line Log\n");
    printStructure(thirdline_log);
    printf("\nFourth Line Log\n");
    printStructure(fourthline_log);

    //TASK 2
    printf("Beginning of TASK 2 if you wish to continue, type any number and press enter\n");
    scanf("%d", &buffer);
    findIssues(firstline_log, issue_log, &count);
    findIssues(secondline_log, issue_log, &count);
    findIssues(thirdline_log, issue_log, &count);
    findIssues(fourthline_log, issue_log, &count);

    printIssueLog(issue_log, count);

    //TASK3
    printf("Beginning of TASK 3 if you wish to continue, type any number and press enter\n");
    scanf("%d", &buffer);
    //Prompt for search ID
    printf("\nWhat product's ID issue code would you like to find?\n");
    scanf("%d", &search);

    //Get the result
    result = findEarliestOccurance(firstline_log, search);
    //If returned with negative, means not found
    if (result == -1){
        printf("Product ID not found in line 1, searching in line 2...\n");
        //Get the result and so on...
        result = findEarliestOccurance(secondline_log, search);
        if (result == -1){
            printf("Product ID not found in line 2, searching in line 3...\n");
            result = findEarliestOccurance(thirdline_log, search);
            if (result == -1){
                printf("Product ID not found in line 3, searching in line 4...\n");
                result = findEarliestOccurance(fourthline_log, search);
                if (result == -1){
                    printf("\nProduct ID not found in any of the lines\n");
                }
                //If it is possitive, then that means it was found
                else {
                    printf("\nProduct found in line 4, product issue: %d\n", fourthline_log[result].iss.code);
                }
            }
            else {
                printf("\nProduct found in line 3, product issue: %d\n", thirdline_log[result].iss.code);
            }
        }
        else {
            printf("\nProduct found in line 2, product issue: %d\n", secondline_log[result].iss.code);
        }
    }
    else {
        printf("\nProduct found in line 1, product issue: %d\n", firstline_log[result].iss.code);
    }


    printf("Beginning of TASK 4 if you wish to continue, type any number and press enter\n");
    scanf("%d", &buffer);

    printf("Enter a valid product ID that you want to summarise the data of.\n");
    scanf("%d", &search);
    //TASK 4
    //Sort look through the issue log and count how many times the product_ID had issues
    buffer = summariseData(issue_log, count, search);

    printf("The product with the ID of: %d was found to have a total of %d issues across all production lines", search, buffer);



    return 0;


}

//Function to read the files into the structures
void read_loginfo(struct QAlog *read_struct, FILE **file){

    for (int i = 0; i < DATA_SIZE && feof(*file) == 0; i++){

        fscanf(*file, "%d", &read_struct[i].line_code);
        fscanf(*file, "%d", &read_struct[i].batch_code);
        fscanf(*file, "%d", &read_struct[i].dt.day);
        fscanf(*file, "%d", &read_struct[i].dt.hour);
        fscanf(*file, "%d", &read_struct[i].dt.min);

        //Convert the day, hour and minute markers into one single digit to make it easier for comparison
        read_struct[i].dt.time = read_struct[i].dt.hour * 60 + read_struct[i].dt.min + read_struct[i].dt.day * 24 * 600;
        //we multiple the day by 600 just to ensure that the day has more isgnificance to time
        //previous algorithm had 24 * 60, which proved to be problematic on extreme times, this algorithm is full proof

        //Continue reading
        fscanf(*file, "%d", &read_struct[i].product_ID);
        fscanf(*file, "%d", &read_struct[i].iss.code);

        fgetc(*file); //get rid of the space between number and text
        strcpy(read_struct[i].iss.description,""); //initialise to nothing so that random characters don't appear

        //Loop for issue description
        do
        {
            char read = fgetc(*file);
            //stop when '.' read
            if (read == '.')
            {
                break;
            }
            else {
                strncat(read_struct[i].iss.description, &read, 1);
            }
        } while (1);

        fscanf(*file, "%d", &read_struct[i].res.code);

        fgetc(*file); //get rid of the space between number and text
        strcpy(read_struct[i].res.description,""); //initialise to nothing so that random characters don't appear

        //Loop for resolution description
        do
        {
            char read = fgetc(*file);
            //stop when '.' read
            if (read == '.')
            {
                break;
            }
            else {
                strncat(read_struct[i].res.description, &read, 1);
            }
        } while (1);

        fscanf(*file, "%d", &read_struct[i].employee_ID);
    }

}

//Print out the structures to see them initially
void printStructure (struct QAlog A[]){
    printf("Ln Bt Day Hour Min Pr_ID IssCode Desc ResCode Desc Emp_ID\n");
    for (int i = 0; i < DATA_SIZE; i++){
        printf("%d ", A[i].line_code);
        printf("%3d ", A[i].batch_code);
        printf("%3d ", A[i].dt.day);
        printf("%3d ", A[i].dt.hour);
        printf("%3d ", A[i].dt.min);
        printf("%4d ", A[i].product_ID);
        printf("%4d ", A[i].iss.code);
        printf("%s ", A[i].iss.description);
        printf("%d ", A[i].res.code);
        printf("%s ", A[i].res.description);
        printf("%d \n", A[i].employee_ID);
    }
}

//Merge sort for TASK 1 
void mergesort(struct QAlog A[], int n)    //n is the length of a[]
{
    if (n < 2)
        return;     //BASE CASE
    int mid = n / 2;
    struct QAlog left[mid];
    struct QAlog right[n-mid];

    for (int i = 0; i < mid; i++)
        left[i] = A[i]; //copy the data into an array

    for (int i = mid; i < n; i++)
        right[i-mid] = A[i]; //copy the data into the other array

    mergesort(left, mid);
    mergesort(right, n - mid);
    merge(A, left, right, mid, n - mid); 
}

//Merging algorithm for TASK 1
void merge(struct QAlog A[], struct QAlog L[], struct QAlog R[],int nL, int nR)
{
    int i = 0, j = 0, k = 0;

    while (i < nL && j < nR)
    {
        //first check product id
        if (L[i].product_ID < R[j].product_ID){ 
            A[k++] = L[i++];
        }
        //then if product id is the same check issue code
        else if (L[i].product_ID == R[j].product_ID&&
        L[i].iss.code < R[j].iss.code){ 
            A[k++] = L[i++];
        }
        //then if theyre both equal check the time
        else if (L[i].product_ID == R[j].product_ID&&
        L[i].iss.code == R[j].iss.code&&
        L[i].dt.time <= R[j].dt.time){ 
            A[k++] = L[i++];
        }
        else {
            A[k++] = R[j++];
        }
    }
    while (i < nL){
        A[k++] = L[i++];
    }
    while (j < nR){
        A[k++] = R[j++];
    }
}

//Function to find all the issues linearly as we already have an ordered list. For TASK 2
void findIssues(struct QAlog data[DATA_SIZE], struct issue_list issue_log[MAX_ISSUES], int *count){

    int k = 0, i, j;

    //Go through the N amount of data once
    for (i = 0; i < DATA_SIZE; i+=k){
        //k is for counting how many issues each product has in a line
        k = 0;
        //count is to keep track of how many unique produc + line combinations there are
        issue_log[*count].line_code = data[i].line_code;
        issue_log[*count].product_ID = data[i].product_ID;
        //in this for loop i have conditions to make sure we go through the list only once
        for (int j = i; j < DATA_SIZE; j++){
            //check if the product id's match
            if (issue_log[*count].product_ID == data[j].product_ID)
            {
                issue_log[*count].issuecode_list[k] = data[j].iss.code;
                k++;
            }
            //if they dont math, break out of the loop
            else {
                break;
            }
        }
        issue_log[*count].issuecode_list_len += k; //at least one
        *count = *count + 1; //up the count
    }
}

//Function to print the list of issues for TASK 2
void printIssueLog (struct issue_list issue_log[MAX_ISSUES], int count){

    for (int i = 0; i < count; i++){
        if (issue_log[i].line_code != issue_log[i-1].line_code){
            printf("\nNew line \n");
        }

        printf("Line: %d Product ID: %d\nIssue Codes:\n", issue_log[i].line_code, issue_log[i].product_ID);
        
        for (int j = 0; j < issue_log[i].issuecode_list_len; j++){
            printf("%d ", issue_log[i].issuecode_list[j]);
        }
        printf("\n\n");
    }

}

//Function for finding the earliest occurance through the lines. TASK 3. This is binary search
int findEarliestOccurance (struct QAlog data[DATA_SIZE], int search){

    int low = 0;
    //high is one less because of the arrays (its 0-9 instead of 1-10)
    int high = DATA_SIZE - 1;

    //While we haven't gotten to less than one number
    while (low <= high){
        //divide
        int mid = low + (high - low) / 2;

        //Check if it is the same
        if (data[mid].product_ID == search){
            return mid; //if it is, return the found index
        }

        //if it is a small number, then move the low up
        else if (data[mid].product_ID < search){
            low = mid + 1;
        }

        //move the high down
        else {
            high = mid - 1;
        }
    }

    return -1;
    
}

//TASK 4. when the issue_log's are sorted by product ID, it is easy to find issues for each product across  all lines
int summariseData (struct issue_list issue_log[MAX_ISSUES], int count, int search){

    int sum = 0;

    for (int i = 0; i < count; i++){
        if (search == issue_log[i].product_ID){
            sum+= issue_log[i].issuecode_list_len;
        }
    }

    return sum;

    
}