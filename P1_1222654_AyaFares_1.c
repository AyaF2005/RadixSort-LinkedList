// Aya Abdelrahman Fares 1222654 section 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Letters_SIZE 53

void displayMenu();
////////////////////////////////////////////////////

// define the struct for the town
struct nodeTown { // Node to store town information
    char townName[Letters_SIZE];// Name of the town
    int population; // Population of the town
    struct nodeTown* next;  // Pointer to the next town in the list

};
typedef struct nodeTown* TP;// typedef for the town pointer to use it in the linked list of towns
typedef TP townList; // typedef for the town list to use it in the district struct

//////////////////////////////////////////////////////////////////////
// define the struct for the district
struct nodeDistrict { // Node to store district information
    char districtsName[Letters_SIZE];
    int TotalPopulation;
    townList towns;// Pointer to the first town in the list of towns in the district
    struct nodeDistrict* next;
    struct nodeDistrict* previous;
};

typedef struct nodeDistrict* DP ;// typedef for the district pointer to use it in the linked list of districts
typedef DP districtList; // typedef for the district list to use it in the FullInfo struct

districtList unsortedDistrictList; // Global variable to store the unsorted districts

/////////////////////////////////////////////////////////////////////////

// define the struct for the full information of the district and town
struct nodeFullInfo { // Node to store the full information of the district and town
    char districtsName[Letters_SIZE];
    char townName[Letters_SIZE];
    int townPopulation;
    struct nodeFullInfo * next;
    struct nodeFullInfo * previous;
};
typedef  struct nodeFullInfo* FI_ptr; // typedef for the full information pointer to use it in the linked list of districts and towns
typedef FI_ptr mainList;// typedef for the main list to use it in the main list of districts and towns

mainList MainList; // Global variable to store the main list of districts and towns

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions to use for adding, deleting, sorting, and printing the districts and towns

// Function to insert a new town into the list of towns in a district
void AddToDistricts(districtList L, char districtName[Letters_SIZE], char townName[Letters_SIZE], int TownPopulation) {
    DP temp = L->next;// Pointer to the first district in the list
    while (temp != NULL) {// Iterate through the district list
        if (strcmp(temp->districtsName, districtName) == 0) {
            // Found the district, insert town into its town list
            TP new_Town = (TP)malloc(sizeof(struct nodeTown));
            strcpy(new_Town->townName, townName);// Insert the new town at the end
            new_Town->population = TownPopulation;// Update total population of the district
            new_Town->next = NULL;

            if (temp->towns == NULL) {
                // Town list is empty
                temp->towns = new_Town;
            } else {
                // Find the last town in the list
                TP LastTemp = temp->towns;
                while (LastTemp->next != NULL) {
                    LastTemp = LastTemp->next;
                }
                // Insert the new town at the end
                LastTemp->next = new_Town;
            }
            temp->TotalPopulation += TownPopulation; // Update total population of the district
            return;
        }
        temp = temp->next;
    }
    // If district not found, create a new one and insert the town
    DP newDistrict = (DP)malloc(sizeof(struct nodeDistrict));
    strcpy(newDistrict->districtsName, districtName);
    newDistrict->TotalPopulation = TownPopulation;
    newDistrict->next = L->next;
    if (L->next != NULL) {
        L->next->previous = newDistrict;
    }
    newDistrict->previous = L;
    newDistrict->towns = (townList)malloc(sizeof(struct nodeTown));
    strcpy(newDistrict->towns->townName, townName);
    newDistrict->towns->population = TownPopulation;
    newDistrict->towns->next = NULL;
    L->next = newDistrict;


}

///////////////////////////////////////////////////////////////
// Create a new district list
districtList NewDistrictList() {
    districtList L;
    L = (districtList)malloc(sizeof(struct nodeDistrict));
    if (L == NULL) {
        printf("Could not create list. List is not found..\n");
        return NULL;
    } else {
        L->next = NULL;
        L->previous = NULL;
        L->TotalPopulation = 0;
    }
    return L;
}
typedef struct nodeFullInfo* mainList;// typedef for the main list to use it in the main list of districts and towns

///////////////////////////////////////////////////////////////////////////////////////////

// Function to print the list of districts and towns from the main list
// will use it in case 2 to print the loaded information before sorting
void printInputList(mainList L){

    if(L==NULL) {
        printf("List Not Found..\n");
        return;
    }else if(L->next==NULL){
        printf("List is empty, nothing to print..\n");
        return;
    }

    FI_ptr temp=L->next; // Pointer to the first string in the list

    while(temp != NULL){
        printf("%s| ", temp->districtsName);
        printf("%s| ", temp->townName);
        printf("%d\n", temp->townPopulation);
        temp=temp->next;
    }
    printf("\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function to insert a new district and town into the main list to use it in case 6 and case 7
void insertToMain(mainList L, char districtName[Letters_SIZE], char townName[Letters_SIZE], int TownPopulation){

    FI_ptr newNode=(FI_ptr)malloc(sizeof(struct nodeFullInfo)); // Create a new node to store the district and town information
    strcpy(newNode->districtsName,districtName); // Copy the district name to the new node
    strcpy(newNode->townName,townName); // Copy the town name to the new node
    newNode->townPopulation=TownPopulation; // Copy the town population to the new node
    newNode->previous=NULL;
    newNode->next=NULL;

    if(L==NULL){
        printf("List Not Found..\n");
        return;

    }else if(L->next==NULL){
        newNode->next=L->next;
        L->next=newNode;
        newNode->previous=L;
        return;
    }

    FI_ptr temp = L->next; //pointer to the first string in the list

    while(temp->next != NULL){
        temp=temp->next;
    }

    newNode->next=temp->next;
    temp->next=newNode;
    newNode->previous=temp;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Create a new main list witch will store the districts and towns and mainList is the head of the list
mainList createMainList(){

    mainList L;
    L=(mainList)malloc(sizeof(struct nodeFullInfo));
    if(L==NULL){
        printf("Could not create list. List is not found..\n");
        return NULL;
    }else{
        L->next=NULL;
        L->previous=NULL;
    }

    return L;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Radix Sort Algorithm that sorts the districts alphabetically not using arrays using linked list
void RadixSort(districtList L) {
    int i;
    DP Storage[Letters_SIZE], current, tail, head; // Array of Storages to store the districts starting with each letter of the alphabet

    // Initialize the Storage to NULL which means its empty at the beginning
    for (i = 0; i < Letters_SIZE; i++) {
        Storage[i] = NULL;
    }

    // Distribute the elements from the unsorted list to the Storage dependes on the first letter of the district name
    current = L->next; // Pointer to the first district in the list
    while (current != NULL) { // Iterate through the district list
        head = current; // Store the current district
        current = current->next; // Move to the next district
        head->next = NULL; // then let the next of the current district to be NULL
        // Calculate the index of the first letter of the district name
        int Letter_index = head->districtsName[0] - 'A'; // the way of calaculating is to subtract the ASCII value of 'A' from the first letter of the district name to get the Letter index
        if (Storage[Letter_index] == NULL) {
            Storage[Letter_index] = head;
            Storage[Letter_index]->previous = NULL;
            Storage[Letter_index]->next = NULL;
        } else {
            tail = Storage[Letter_index]; // Store the last district in the Storage to be the tail, so we can add the new district to the end of the Storage
            // loop to find the last district in the Storage
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = head;
            head->previous = tail;
        }
    }

    // Merge the districts from the Storage to the sorted list
    tail = NULL;
    for (i = 0; i < Letters_SIZE; i++) {
        if (Storage[i] != NULL) {
            if (tail == NULL) {
                L->next = Storage[i];
                Storage[i]->previous = NULL;
            } else {
                tail->next = Storage[i];
                Storage[i]->previous = tail;
            }
            while (Storage[i]->next != NULL) {
                Storage[i] = Storage[i]->next;
            }
            tail = Storage[i];
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Function to sort the towns in each district based on population in ascending order
void sortTown_Population(districtList AddToDistricts) {
    DP ptr = AddToDistricts->next; // Pointer to the first district in the list
    while (ptr != NULL) {
        TP current = ptr->towns; // Point to the first town in the district
        TP next = NULL;
        int Done ;// Flag to check if the towns are swapped
        do { // Loop to sort the towns in the district based on population in ascending order
            Done = 0;
            current = ptr->towns;
            while (current->next != next) { // while loop to iterate through the towns in the district
                if (current->population > current->next->population) { // Compare the population of the current town with the next town
                    int tempPopulation = current->population;
                    char tempTownName[Letters_SIZE];
                    strcpy(tempTownName, current->townName); // strcpy to store the name of the town in a temp Array
                    current->population = current->next->population; // Swap the population of the towns
                    strcpy(current->townName, current->next->townName); // Swap the name of the towns
                    current->next->population = tempPopulation;
                    strcpy(current->next->townName, tempTownName);
                    Done = 1;
                }
                current = current->next;
            }
            next = current; // Move to the next town
        } while (Done);
        ptr = ptr->next; // Move to the next district
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

// input file function to read the districts and towns from the input file and store them in the unsorted list
void input_File() {
    FILE* in = fopen("districts.txt", "r");
    if (in == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char str[500]; // Array to store the line read from the file
    while (fgets(str, sizeof(str), in)) { // now we will read the file line by line
        char *line = strtok(str, "|"); // but we will split the line by the delimiter "|" and store the result in the line variable
        char districtsName[Letters_SIZE];
        char townName[Letters_SIZE];
        int population;
        //
        strcpy(districtsName, line);// copy the first part of the line to the districtsName variable
        line = strtok(NULL, "|"); // move to the next part of the line
        strcpy(townName, line); // copy the second part of the line to the townName variable
        line = strtok(NULL, "|"); // move to the next part of the line
        population = atoi(line); // convert the third part of the line to an integer and store it in the population variable
        AddToDistricts(unsortedDistrictList, districtsName, townName, population);
        insertToMain(MainList, districtsName, townName, population);

    }

    fclose(in);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// case 5 print the sorted information

void printSortedInformation() {
    DP ptrDistrict = unsortedDistrictList->next; // Pointer to the first district in the list
    while (ptrDistrict != NULL) {
        printf("%s District, Population = %d\n", ptrDistrict->districtsName, ptrDistrict->TotalPopulation);
        TP currentTown = ptrDistrict->towns; // Pointer to the first town in the district
        while (currentTown != NULL) {
            printf("%s, %d\n", currentTown->townName, currentTown->population);
            currentTown = currentTown->next; // Move to the next town in the district
        }
        ptrDistrict = ptrDistrict->next;// Move to the next district in the list
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////
// 6. Add a new district to the list of sorted districts (and sort the list)

void addNewDistrict() {
    char districtName[Letters_SIZE];
    char townName[Letters_SIZE];
    int population;
    printf("Enter the district name: ");
    while(getchar()!='\n');
    scanf("%99[^\n]",districtName); // Read the district name and the name could be more than one word
    printf("Enter the town name: ");
    while(getchar()!='\n');
    scanf("%99[^\n]",townName); // Read the town name and the name could be more than one word
    printf("Enter the town population: ");
    scanf("%d", &population);

    AddToDistricts(unsortedDistrictList, districtName, townName, population); // Add the new district to the list of districts
    insertToMain(MainList, districtName, townName, population); // Add the new district to the main list
    RadixSort(unsortedDistrictList); // Sort the districts alphabetically using Radix Sort
}
/////////////////////////////////////////////////////////////////////////////////////////////

// 7. Add a new town to a certain district
void addNewTownToDistrict() {
    char districtName[Letters_SIZE];
    char townName[Letters_SIZE];
    int population;
    printf("Enter the district name: ");
    while(getchar()!='\n');
    scanf("%99[^\n]",districtName);
    printf("Enter the town name: ");
    while(getchar()!='\n');
    scanf("%99[^\n]",townName);
    printf("Enter the town population: ");
    scanf("%d", &population);

    AddToDistricts(unsortedDistrictList, districtName, townName, population); // Add the new town to the district
    insertToMain(MainList, districtName, townName, population);// Add the new town to the main list
    sortTown_Population(unsortedDistrictList);// Sort the towns for each district based on population in ascending order
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

// 8. Delete a town from a specific district, and it could be from the input file

void deleteTownFromDistrict(char districtName[], char townName[]) {
    DP districtptr = unsortedDistrictList->next; // Pointer to the first district in the list
    while (districtptr != NULL) {
        if (strcmp(districtptr->districtsName, districtName) == 0) { // Compare the district name with the input district name
            TP townptr = districtptr->towns;
            TP previousTown = NULL;
            while (townptr != NULL) {
                if (strcmp(townptr->townName, townName) == 0) {// Found the town in the district
                    if (previousTown == NULL) { // Check if the town is the first in the list
                        districtptr->towns = townptr->next; // if yes then move the head pointer to the next town
                    } else { // Town is not the first in the list
                        previousTown->next = townptr->next ; // Move the previous town pointer to the next town
                    }
                    districtptr->TotalPopulation -= townptr->population; // Update the total population of the district
                    free(townptr); // Free the memory of the deleted town
                    printf("Town '%s' got deleted from district '%s'.\n", townName, districtName);
                    return;
                }
                previousTown = townptr; // Move the previous town pointer to the current town
                townptr = townptr->next; // Move to the next town in the list
            }
            printf("Town '%s' this Town is not found in district '%s'.\n", townName, districtName);
            return;
        }
        districtptr = districtptr->next; // Move to the next district in the list
    }
    printf("District '%s' not found.\n", districtName); // if the district is not found
}

// Menu option function for deleting a town from a specific district
void DeleteTown() {
    char districtName[Letters_SIZE];
    char townName[Letters_SIZE];

    printf("Enter the district name: ");
    while(getchar()!='\n');
    scanf("%99[^\n]",districtName);
    printf("Enter the town name: ");
    while(getchar()!='\n');
    scanf("%99[^\n]",townName);

    deleteTownFromDistrict(districtName, townName);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 9. Delete a complete district and all towns belonging to it first and make changes in the main list
// Function to delete a complete district and all towns belonging to it

void deleteDistrictAndTowns(char districtName[]) {
    DP districtptr = unsortedDistrictList->next;
    DP previousDistrict = unsortedDistrictList;
    while (districtptr != NULL) {
        if (strcmp(districtptr->districtsName, districtName) == 0) { // if the district is found by comparing the district name
            unsortedDistrictList->TotalPopulation -= districtptr->TotalPopulation; // Update the total population of the district
            TP townptr = districtptr->towns;// Pointer to the first town in the district
            while (townptr != NULL) { // free the memory of the towns
                TP temp = townptr;
                townptr = townptr->next;
                free(temp);
            }
            // then Delete the district
            previousDistrict->next = districtptr->next; // Move the previous district pointer to the next district
            if (districtptr->next != NULL) { // Check if the district is not the last in the list
                districtptr->next->previous = previousDistrict; // Move the previous pointer of the next district to the previous district
            }
            free(districtptr); // Free the memory of the deleted district
            printf("District '%s' and all its towns deleted.\n", districtName);
            return;
        }
        previousDistrict = districtptr;
        districtptr = districtptr->next;
    }

    printf("District '%s' not found.\n", districtName); //if district not found
}

// Menu option function for deleting a district and all its towns
void DeleteDistrict() {
    char districtName[Letters_SIZE];
    printf("Enter the name of the district you want to delete: ");
    while(getchar()!='\n');
    scanf("%99[^\n]",districtName);
    deleteDistrictAndTowns(districtName);
}
///////////////////////////////////////////////////////////////////////////////////////////////////

// 10. Calculate the population ,the max and min town population.
void CalculatePopulationTotal_Max_Min() {
    DP ptr = unsortedDistrictList->next; // Pointer to the first district in the list
    int totalPopulation = 0;
    int maxPopulation = 0;
    int minPopulation = 1000000000;

    while (ptr != NULL) {
        totalPopulation += ptr ->TotalPopulation;

        TP townTemp = ptr->towns;
        while (townTemp != NULL) {
            if (townTemp->population > maxPopulation) {
                maxPopulation = townTemp->population;
            }
            if (townTemp->population < minPopulation) {
                minPopulation = townTemp->population;
            }
            townTemp = townTemp->next;
        }

        ptr = ptr->next;
    }
    printf("Total population of towns = %d\n", totalPopulation);
    printf("Max town population = %d\n", maxPopulation);
    printf("Min town population = %d\n", minPopulation);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 11. Print the districts and their total population (without towns details).
void Print_Districts_TotalPopulation() {
    DP temp = unsortedDistrictList->next;
    while (temp != NULL) {
        printf("%s District, Population = %d\n", temp->districtsName, temp->TotalPopulation);
        temp = temp->next;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 12. Change the population of a town, after prompting the user to enter the district name even if the town is from the input file
// Function to change the population of a town
void changeTownPopulation(char districtName[]) {
    char townName[Letters_SIZE];
    int newPopulation;

    DP districtptr = unsortedDistrictList->next;
    while (districtptr != NULL) {
        if (strcmp(districtptr->districtsName, districtName) == 0) { // Compare the district name with the input district name
            printf("Enter the name of the town: ");
            while(getchar()!='\n');// to clear the buffer
            scanf("%99[^\n]",townName);
            // Search for the town in the district
            TP townptr = districtptr->towns;// Pointer to the first town in the district
            while (townptr != NULL) {
                if (strcmp(townptr->townName, townName) == 0) { // Compare the town name with the input town name
                    printf("Enter the new population for town '%s': ", townName);
                    scanf("%d", &newPopulation);

                    townptr->population = newPopulation;// Update the population of the town
                    printf("Population of town '%s' in district '%s' updated to %d.\n", townName, districtName, newPopulation);
                    return;
                }
                townptr = townptr->next;// Move to the next town in the district
            }
            printf("Town '%s' not found in district '%s'.\n", townName, districtName);
            return;
        }
        districtptr = districtptr->next;
    }
    printf("District '%s' not found.\n", districtName);
}

// Menu option function for changing the population of a town
void ChangePopulation_Town() {
    char districtName[Letters_SIZE];
    printf("Enter the name of the district: ");
    while(getchar()!='\n');
    scanf("%99[^\n]",districtName);
    changeTownPopulation(districtName);
}

///////////////////////////////////////////////////////////////////////////////////////////

// 13. Save to output file, and it should be as the same way it ordered in the input file it should be sorted in the output file alphabetically
// Function to write the information to an output file
void outputFile() {
    FILE* out = fopen("sorted_districts.txt", "w");
    if (out == NULL) {
        printf("Error opening file.\n");
        return;
    }
    RadixSort(unsortedDistrictList); // Sort the districts alphabetically using Radix Sort

    DP ptr = unsortedDistrictList->next;
    while (ptr != NULL) {
        // Sort towns by population within the district
        sortTown_Population(ptr);


        // Write district name and total population to the output file
        fprintf(out, "%s District, Population = %d\n", ptr->districtsName, ptr->TotalPopulation);

        // Write towns and their populations to the output file
        TP townTemp = ptr->towns;
        while (townTemp != NULL) {
            fprintf(out, "\t%s, %d\n", townTemp->townName, townTemp->population);
            townTemp = townTemp->next;
        }
        ptr = ptr->next; // Move to the next district
    }

    fclose(out);
}
///////////////////////////////////////
// 14. Exit

void Exit() {
    exit(0);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{

    MainList=createMainList(); // a global variable to store the main list of districts and towns
    unsortedDistrictList = NewDistrictList(); // a global variable to store the unsorted districts

    int choice = 0 ;
    while( choice !=14){ //options >1 ||options < 15
        displayMenu();
        printf("Enter The Options :");
        scanf("%d",&choice);
        switch(choice){

            case 1:input_File();
                printf("file has been loaded \n");
                break;

            case 2:
                printInputList(MainList);
                printf("The loaded information before sorting has been printed.\n");
                break;
            case 3:
                RadixSort(unsortedDistrictList);
                printf("The districts have been sorted alphabetically.\n");
                break;
            case 4:
                sortTown_Population(unsortedDistrictList);// Sort the towns for each district based on population in ascending order
                printf("The towns have been sorted by population.\n");
                break;
            case 5:
                printSortedInformation();// print the sorted infos
                break;
            case 6:
                addNewDistrict();// Add a new district to the sorted lest
                printf("New district has been added and sorted.\n");
                break;
            case 7:
                addNewTownToDistrict();// add new town to a specific district
                printf("New town has been added to the district.\n");
                break;
            case 8: DeleteTown(); // delete a town from a specific district
                break;
            case 9: DeleteDistrict(); // delete a complete district
                break;
            case 10:CalculatePopulationTotal_Max_Min();// calculate the total, max and min town population
                break;
            case 11:Print_Districts_TotalPopulation();// print the districts and their total population
                break;
            case 12: ChangePopulation_Town(); // change the population of a chosen town
                break;
            case 13:outputFile();
                printf("The sorted information has been saved to the output file.\n");
                break;
            case 14:printf("Exit\n");
                Exit();
                break;
            default: printf("there is an error\n");
                displayMenu();
                break;
        }
    }
    return 0;
}
void displayMenu(){
    printf(" Choose one of this Display Menu options !  \n");
    printf("1) Load the input file\n");
    printf("2) Print the loaded information before sorting\n");
    printf("3) Sort the districts alphabetically using Radix sort\n");
    printf("4) Sorting the Town By Population\n");
    printf("5) Print the sort information\n");
    printf("6) Add a new district to the list of sorted districts\n");
    printf("7) Add a new town to a certain district\n");
    printf("8) Delete a town from a specific district\n");
    printf("9) Delete a complete district\n");
    printf("10) Calculate the population of Palestine(the max and min town population)\n");
    printf("11) Print the districts and their total population\n");
    printf("12) Change the population of a town, after prompting the user to enter the district name\n");
    printf("13) Save to output file\n");
    printf("14) Exit\n");

}


