/*------------------------------------------------------------------------------------------------------------------------------------
Program 2: Food Web Analysis with Dynamic Memory
    A food web is built from user-specified predator-prey relationships to create a dynamically allocated struct of array of organisms 
    which is then used to identify the relationships between the organisms in the food web. The user can modify the food web 
    by adding new organisms, removing organisms from the food web. At any stage, the user can choose to view food web characteristics.
Course: CS 211, Fall 2025, UIC
Author: Coral Fragoso Herrera
------------------------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Org_struct {
    char name[20];
    int* prey; //dynamic array of indices  
    int numPrey;
} Org;

// This function adds a new organism to the web using a pointer to a dynamically allocated array of Orgs,
// a passed-by-pointer parameter for the number of organisms, and the name of the new organism.
// The Org array is reallocated for the new organism and number of organisms increases by one.
void addOrgToWeb(Org** pWeb, int* pNumOrgs, char* newOrgName) {
    // allocates memory one size bigger than current array of Orgs to temp
    Org* temp = (Org*)malloc(((*pNumOrgs) + 1) * sizeof(Org));
    for (int i = 0; i < (*pNumOrgs); i++) {
        temp[i] = (*pWeb)[i]; // assigns values or current array of Orgs to temp
    }

    free(*pWeb);              // frees allocated memory for pWeb
    *pWeb = temp;             // pWeb points to temp

    strcpy((*pWeb)[*pNumOrgs].name, newOrgName); // copy newOrgName to the name subitem of the Org at the last index
    (*pWeb)[*pNumOrgs].prey = NULL; // set prey subitem to NULL for new organism
    (*pWeb)[*pNumOrgs].numPrey = 0; // set numPrey subitme to 0 for new organism
    (*pNumOrgs)++;                  // update the number of organisms in food web
    
    return; 
}

// This function adds a predator-prey relation to the food web using a dynamically allocated array if Orgs, 
// nymber of organisms, predator index in web, and prey index in web. Returns true if the new relation was
// successfully added to the web while implicitly returning and updating array, or false if any issue occurs 
// that prevent creating a new relation.
bool addRelationToWeb(Org* web, int numOrgs, int predInd, int preyInd) {
    if ((preyInd >= numOrgs) || (preyInd < 0)) {      // prey index is out of bounds
        return false;
    }
    else if ((predInd >= numOrgs) || (predInd < 0)) { // predator index is out of bounds
        return false;
    }
    else if (predInd == preyInd) {                    // prey index and predator index are equivalent
        return false;
    }
    else {
        for (int i = 0; i < web[predInd].numPrey; i++) {
            if (web[predInd].prey[i] == preyInd) { // checks if relation already exists
                printf("Invalid predator and/or prey index. No relation added to the food web.\n");
                return false;
           } 
        }
    }
    
    int* temp;

    // Checks if prey subitem is empty
    if (web[predInd].prey == NULL) {
        // allocates memory for only one space
        temp = (int*)malloc(sizeof(int));
        temp[0] = preyInd;        // assigns prey index to temp
        web[predInd].prey = temp; // organism's prey subitem now points to temp
        web[predInd].numPrey = 1; // number of prey increases by one
        return true;
    }
    // allocates memory for one more space that current prey subitem
    temp = (int*)malloc((web[predInd].numPrey + 1) * sizeof(int));
    for (int i = 0; i < web[predInd].numPrey; i++) {
        temp[i]= web[predInd].prey[i]; // assigns all values of prey subitem to temp
    }

    temp[web[predInd].numPrey] = preyInd; // new prey index is appended
    free(web[predInd].prey);              // frees allocated memory for prey subitem
    web[predInd].prey = temp;             // prey subitem now points to temp
    
    (web[predInd].numPrey)++;             // number of prey increases by one

    return true;
}

// This function removes the organism associated with index from the web using a pointer to a dynamically allocated array of Orgs,
// a passed-by-pointer parameter for the number of organisms, and an organism index in the array to remove. pWeb is reallocated with
// less memory onced organism is removed and pNumOrgs decreases by one after removal. Returns true if the organism was successfully
// removed from the web, or false if any issue occurs that prevents removal of an organism.
bool removeOrgFromWeb(Org** pWeb, int* pNumOrgs, int index) {
    if ((index < 0) || (index >= *pNumOrgs)) { // checks if index is out of bounds
        printf("Invalid extinction index. No organism removed from the food web.\n");
        return false;
    }
        
    if (*pNumOrgs == 1) {              // removess the only organism in web
        (*pNumOrgs)--;                 // decreases number of organisms by one
        if ((*pWeb)[0].prey != NULL) { // checks if prey subitem is not empty
            free((*pWeb)[0].prey);     // frees allocated memory for prey subitem
            (*pWeb)[0].prey = NULL;    // sets prey subitem to NULL
        }
        free(*pWeb);                   // frees allocated memory for array of Org structs
        *pWeb = NULL;                  // sets web to NULL
    }
    else {
        int* place;

        // removing organism from prey array
        for (int i = 0; i < *pNumOrgs; i++) {
            if((*pWeb)[i].numPrey == 0) { continue; } // if it is a producer then skip
            for(int j = 0; j < (*pWeb)[i].numPrey; j++) {
                // removes the only prey in prey subitem
                if((*pWeb)[i].prey[j] == index && (*pWeb)[i].numPrey == 1) {
                    free((*pWeb)[i].prey);  // frees allocated memory for prey subitem
                    (*pWeb)[i].prey = NULL; // sets prey subitem to NULL
                    (*pWeb)[i].numPrey--;   // decrements numPrey subitem by one
                    break;
                }
                else if((*pWeb)[i].prey[j] == index) { // removes organsim found in prey subitems
                    place = (int*)malloc(sizeof(int) * ((*pWeb)[i].numPrey - 1)); // allocates memory for one less space than current array of Org structs
                    int position = 0;

                    // loops through each prey in prey subitem
                    for(int k = 0; k < (*pWeb)[i].numPrey; k++) {
                        if((*pWeb)[i].prey[k] == index) { // if prey is the removed prey, skip
                            continue;
                        }
                        place[position] = (*pWeb)[i].prey[k]; // assigns each prey to place
                        position++; // increments the next position in the array for assignment
                    }
                    free((*pWeb)[i].prey);   // frees allocated memory for prey subitem
                    (*pWeb)[i].prey = place; // prey subitem now points to place
                    (*pWeb)[i].numPrey--;    // decrements the number of prey by one
                    break;
                }
            }
            
        }
        // decrements prey index for each prey in subitem iff it is greater than the index parameter and prey array not empty
        for (int i = 0; i < *pNumOrgs; i++) {
            if ((*pWeb)[i].prey != NULL) {
                for (int j = 0; j < (*pWeb)[i].numPrey; j++) {
                    if ((*pWeb)[i].prey[j] > index) {
                        (*pWeb)[i].prey[j]--;
                    }
                }
            }
        }

        Org* temp = (Org*)malloc(((*pNumOrgs) - 1) * sizeof(Org)); // allocates memory for one less space than current array of Org structs
        int position = 0;

        // loops through all organisms in food web
        for (int i = 0; i < *pNumOrgs; i++) {
            if (i == index) {          // checks for organism to remove
                free((*pWeb)[i].prey); // frees allocated memory for organism's prey subitem
                continue;
            }
            temp[position] = (*pWeb)[i]; // assigns all other organisms to temp
            position++; // increments the next position for assignment in temp
        }
        free(*pWeb);    // frees allocated memory for array of Org structs
        *pWeb = temp;   // array of Org structs now points to temp
        (*pNumOrgs)--;  // decrements number of organisms by one
    }

    return true; 
}

// This function frees all malloc'd memory associated with web using number of organisms, to prevent potential leaks before program terminates
void freeWeb(Org* web, int numOrgs) {
    // loops through all organisms        
    for (int i = 0; i < numOrgs; i++) {
        free(web[i].prey); // frees allocated memory for an organism's prey subitem
    }
    free(web); // frees allocated memory for array of Org structs
}

// This function prints the Food Web Organisms with what they eat using web, a dynamically allocated 
// array of Org structs, and numOrgs, the number of organism in a food web as parameters
void printWeb(Org* web, int numOrgs) {
    // loops through all organisms in web
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].numPrey == 0){ // checks if an organism is a producer
            printf("  (%d) %s", i, web[i].name);      // displays organism
        }

        else if (web[i].numPrey == 1) {               // checks if an organism has one prey
            printf("  (%d) %s ", i, web[i].name);     // displays organism
            printf("eats ");
            printf("%s", web[(web[i].prey[0])].name); // displays prey
        }
        else { 
            printf("  (%d) %s ", i, web[i].name);     // displays organism
            printf("eats ");

            // lists all prey
            for (int j = 0; j < (web[i].numPrey-1);j++) {
                printf("%s, ", web[(web[i].prey[j])].name); // displays prey
            }
            int num = (web[i].numPrey) - 1;             // gets last index of prey array 
            printf("%s", web[(web[i].prey[num])].name); // displays last prey
        }
        printf("\n");
    }
}

// This function prints the Food Web Organsims that are producers using web, a dynamically allocated 
// array of Org structs, and numOrgs, the number of organisms in a food web as parameters
void producers(Org*web, int numOrgs) {
    printf("  Producers:\n");

    int producer[numOrgs]; // stores array of producers

    // initializes array
    for (int i = 0; i < numOrgs; i++) {
        producer[i] = 0;
    }

    // loops through the all organisms
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].numPrey == 0) { // checks if organism has no prey
            producer[i] = 1;       // assign value of one for producer
        }
    }
    for (int i = 0; i < numOrgs; i++) {
        if (producer[i] == 1) {              // is a producer
            printf("    %s\n", web[i].name); // displays producer
        }
    }
}

// This function prints the Food Web Organisms that are herbivores using web, a dynamically allocated 
// array of Org structs, and numOrgs, the number of organisms in a food web as parameters
void herbivores(Org* web, int numOrgs) {
    printf("  Herbivores:\n");

    int herbivore[numOrgs]; // stores array of herbivores

    // initializes array
    for (int i = 0; i < numOrgs; i++) {
        herbivore[i] = 0;
    }

    int producerPrey = 0; // stores number of producer prey

    // loops through all organisms
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].numPrey == 0) { // if there are no prey skip
            continue;
        }
        // loops through all prey
        for (int j = 0; j < web[i].numPrey; j++) {
            if (web[web[i].prey[j]].numPrey == 0) { // checks if a prey is a producer
                producerPrey++;                     // increments producerPrey count by one
            }
        }
        if (producerPrey == web[i].numPrey) { // checks if the number of prey is equal to the number of producer prey
            herbivore[i] = 1;                 // assigns value of one for herbivore
        }
        producerPrey = 0;                     // resets number of producer prey
    }

    // loops through all organisms
    for (int i = 0; i < numOrgs; i++) {
        if (herbivore[i] == 1) {             // checks for herbivores
            printf("    %s\n", web[i].name); // displays herbivore
        }
    }
}

// This function prints the Food Web Organsims that are omnivores using web, a dynamically allocated 
// array of Org structs, and numOrgs, the number of organisms in a food web as parameters
void omnivores(Org* web , int numOrgs) {
    printf("  Omnivores:\n");

    int producerPrey = 0; // stores number of producer prey
    int otherPrey = 0;    // stores number of non-producer prey

    // loops through all organisms
    for (int i = 0; i < numOrgs; i++) {
        // loops through all prey
        for (int j = 0; j < web[i].numPrey; j++) {
            if (web[web[i].prey[j]].numPrey == 0) { // checks if a prey is a producer
                producerPrey++;                     // increments producer prey by one
            }
            else {
                otherPrey++;   // otherwise, non-producer prey count is incremented
            }
        }
        if ((producerPrey > 0) && (otherPrey > 0)) { // checks if an organisms has producer and non-producer prey
            printf("    %s\n", web[i].name);         // displays omnivore
        }
        producerPrey = 0; // resets number of producer prey
        otherPrey = 0;    // resets number of non-producer prey
    }
}

// This function prints the Food Web Organisms that are carnivores using web, a dynamically allocated 
// array of Org structs, and numOrgs, the number of organisms in a food web as parameters
void carnivores(Org* web, int numOrgs) {
    printf("  Carnivores:\n");
    
    int producerPrey = 0; // stores number of producer prey

    // loops through all organisms
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].numPrey == 0) { // if organism is a producer skip
            continue;
        }
        // loops through all prey
        for (int j = 0; j < web[i].numPrey; j++) {
            if (web[web[i].prey[j]].numPrey == 0) { // checks if prey is a producer
                producerPrey++;                     // increments producer prey number by one
            }
        }
        if (producerPrey == 0) {             // checks for no producers
            printf("    %s\n", web[i].name); // displays carnivore
        }
        producerPrey = 0;                    // resets number of producer prey
    }
}

// This function prints the Food Web Organisms that are apex predators using web, a dynamically allocated 
// array of Org structs, and numOrgs, the number of organisms in a food web as parameters
void apexPred(Org* web, int numOrgs) {
    int allPrey[numOrgs]; // stores all prey organisms

    // initializes allPrey array
    for (int i = 0; i < numOrgs; i++) {
        allPrey[i] = 0;
    }

    // loops through all organisms
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].numPrey > 0) { // checks if an organism has prey
            // loops through all prey
            for (int j = 0; j < web[i].numPrey; j++) { 
                allPrey[web[i].prey[j]] = -1; // assigns a value of -1 for existing prey indexes
            }
        }
    }

    // loops through all organisms
    for (int i = 0; i < numOrgs; i++){
        if (allPrey[i] == 0) {             // checks for apex predator (no counts as a prey to others)
            printf("  %s\n", web[i].name); // displays apex predator
        }
    }
}

// This function prints the Food Web Organisms that are flexible eaters using web, a dynamically allocated
// array of Org structs, and numOrgs, the number of organisms in a food web as parameters
void FlexibleEater(Org* web, int numOrgs) {
    int max = 0; // stores max number of prey for organisms

    // loops through all organisms
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].numPrey > max) { // checks if numPrey is greater than current max
            max = web[i].numPrey;   // reassigns prey with new max
        }
    }

    // loops through all organism
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].numPrey == max) {       // finds the organism with the most prey
            printf("  %s\n", web[i].name); // displays most flexible eater
        }
    }
}

// This function prints the Food Web Organisms that are the tastiest using web, a dynamically allocated 
// array of Org structs, and numOrgs, the number of organisms in a food web as parameters
void tastiest(Org* web, int numOrgs) {
    int taste[numOrgs]; // stores array of prey counts

    // initializes taste array
    for (int i = 0; i < numOrgs; i++) {
        taste[i] = 0;
    }
    // loops through all organisms
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].numPrey > 0) { // checks for non-producers
            // loops through prey array
            for (int j = 0; j < web[i].numPrey; j++) {
                taste[web[i].prey[j]]++; // increments counts of a prey by one
            }
        }
    }
    int maxTaste = 0; // gets the most common prey

    // loops through all organisms
    for (int i = 0; i < numOrgs; i++) {
        if (taste[i] > maxTaste) { // checks if a prey has a larger count that maxTaste
            maxTaste = taste[i];   // reassigns maxTaste to that prey's count
        }
    }
    
    // loops through all organisms
    for (int i = 0; i < numOrgs; i++) {
        if (taste[i] == maxTaste) {        // finds the most common prey
            printf("  %s\n", web[i].name); // displays tastiest food
        }
    }  
}

// This function prints the Food Web Organisms web height using web, a dynamically allocated 
// array of Org structs, and numOrgs, the number of organisms in a food web as parameters
void webHeight(Org* web, int numOrgs) {
    int height[numOrgs]; // stores web height of organisms

    // initializes height array
    for (int i = 0; i < numOrgs; i++) {
        height[i] = 0;
    }

    int maxHeight = 0; // gets the max height of a prey
    int temp[numOrgs];

    // loop through all the organisms
    for (int i = 0; i < numOrgs; i++) {
        temp[i] = height[i]; // stores current web height of an organism

        // loop through non-producer's prey
        if (web[i].numPrey > 0) {
            for (int j = 0; j < web[i].numPrey; j++) {
                if (height[web[i].prey[j]] > maxHeight) { // checks if height of prey is greater than current maxHeight
                    maxHeight = height[web[i].prey[j]];   // reassigns maxHeight with largest height
                }
            }
            height[i] = maxHeight + 1;                    // new height of organism is one more than max height of prey
        }
        else { // producers are fixed to 0;
            height[i] = 0;
        }
        maxHeight = 0; // reset maxHeight

        // reset loop to adjust for modifications
        if (temp[i] != height[i]) {
            i = -1;
        }
    }

    // loops through all organisms
    for (int i = 0; i < numOrgs; i++) {
        printf("  %s: %d\n", web[i].name, height[i]); // displays web height
    }
}

// This function displays all food web characteristics
void displayAll(Org* web, int numOrgs, bool modified) {

    if (modified) printf("UPDATED ");
    printf("Food Web Predators & Prey:\n");
    printWeb(web,numOrgs); 
    printf("\n");

    // prints the organisms not eaten by any others
    if (modified) printf("UPDATED ");
    printf("Apex Predators:\n");
 
    apexPred(web, numOrgs);
    printf("\n");

    // prints the organisms that eat no other organisms
    if (modified) printf("UPDATED ");
    printf("Producers:\n");
    for (int i = 0; i < numOrgs; i++) {
        if (web[i].numPrey == 0) {
            printf("  %s\n", web[i].name);
        }
    }
    printf("\n");

    // prints the organism(s) with the most prey
    if (modified) printf("UPDATED ");
    printf("Most Flexible Eaters:\n");
    FlexibleEater(web, numOrgs);
    printf("\n");

    // prints organism(s) eaten by the most other organisms
    if (modified) printf("UPDATED ");
    printf("Tastiest Food:\n");
    tastiest(web, numOrgs);
    printf("\n");

    // prints the length of the longest chain 
    // from any producer to each organism
    if (modified) printf("UPDATED ");
    printf("Food Web Heights:\n");
    webHeight(web, numOrgs);
    printf("\n");

    // prints each group (producers, herbivores, omnivores, & carnivores)
    if (modified) printf("UPDATED ");
    printf("Vore Types:\n");
    producers(web, numOrgs);
    herbivores(web, numOrgs);
    omnivores(web, numOrgs);
    carnivores(web, numOrgs);
    printf("\n");

}

// This function processes command-line arguments & updates basicMode, debugMode, and quietMode
// using the passed-by-pointer parameters pBasicMode, pDebugMode, and pQuietMode. Returns false by 
// default or if invalid or duplicate command-line argument, otherwise true for corresponding mode. 
// basicMode = TRUE, then only build the initial web and analyze it; no modifications
// debugMode = TRUE, then print out full web as it is built & modified at each step
// quietMode = TRUE, then suppress printed user-input prompt messages        
bool setModes(int argc, char* argv[], bool* pBasicMode, bool* pDebugMode, bool* pQuietMode) {
    if (argc > 4) { //checks for invalid number of command-line argument
        return false;
    }

    // loops through all user-specified command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "-b", 2) == 0) { // user specificies for basicMode
            if (*pBasicMode == true) {        // checks for duplication of basicMode
                return false;
            }
            else {
                *pBasicMode = true;
            }
        }
        else if (strncmp(argv[i], "-d", 2) == 0) { // user specificies for debugMode
            if (*pDebugMode == true) {             // checks for duplication of debugMode
                return false;
            }
            else {
                *pDebugMode = true;
            }
        }
        else if (strncmp(argv[i], "-q", 2) == 0) { // user specificies for quietMode
            if (*pQuietMode == true) {             // checks for duplication of quietMode
                return false;
            }
            else {
                *pQuietMode = true;
            }
        }
        else {
            return false;
        }
    }
  
    return true;
}

// This function prints on or off for program settings if the mode parameter is set to true
void printONorOFF(bool mode) {
    if (mode) {
        printf("ON\n");
    } else {
        printf("OFF\n");
    }
}

int main(int argc, char* argv[]) {  
    // default mode values
    bool basicMode = false;
    bool debugMode = false;
    bool quietMode = false;

    // checks if command-line argument are invalid
    if (!setModes(argc, argv, &basicMode, &debugMode, &quietMode)) {
        printf("Invalid command-line argument. Terminating program...\n");
        return 1; // ends program
    }

    printf("Program Settings:\n");
    printf("  basic mode = ");
    printONorOFF(basicMode); // prints "ON" or "OFF" depending on true or false value of basicMode
    printf("  debug mode = ");
    printONorOFF(debugMode); // prints "ON" or "OFF" depending on true or false value of debugMode
    printf("  quiet mode = ");
    printONorOFF(quietMode); // prints "ON" or "OFF" depending on true or false value of quietMode
    printf("\n");

    
    int numOrgs = 0; // gets the number of organisms in food web
    printf("Welcome to the Food Web Application\n\n");
    printf("--------------------------------\n\n");

    Org* web = NULL; // stores array of Org structs

    printf("Building the initial food web...\n");
    
    // displays input prompt for new organism if quietMode is off
    if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
    char tempName[20] = "";
    scanf("%s",tempName); // reads in user-specified organism to add
    if (!quietMode) printf("\n");

    // continues adding new organism while input is not "DONE"
    while (strcmp(tempName,"DONE") != 0) {
        addOrgToWeb(&web,&numOrgs,tempName); // adds new organism to web
        if (debugMode) {           // checks if debugging mode is on
            printf("DEBUG MODE - added an organism:\n");
            printWeb(web,numOrgs); // displays the web as it is built and modified
            printf("\n");
        }
        // prompts for new organism while quietMode is off
        if (!quietMode) printf("Enter the name for an organism in the web (or enter DONE): ");
        scanf("%s",tempName); // reads in new organism
        if (!quietMode) printf("\n");
    }

    // prints prompt messages for predator/prey relationships while quietMode is off
    if (!quietMode) printf("\n");

    if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
    if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
    if (!quietMode) printf("The format is <predator index> <prey index>: ");
        
    int predInd, preyInd; // gets the pred index and prey index of a relation
    scanf("%d %d",&predInd, &preyInd); // reads in predator/prey relation
    if (!quietMode) printf("\n");

    // continues adding new relations to web while the indexes are valid
    while (predInd >= 0 && preyInd >= 0 && predInd < numOrgs &&  preyInd < numOrgs && predInd != preyInd) {
        addRelationToWeb(web,numOrgs,predInd,preyInd); // adds relation to web
        if (debugMode) { // checks if debugging mode is on
            printf("DEBUG MODE - added a relation:\n");
            printWeb(web,numOrgs); // displays web as it is built and modified
            printf("\n");
        }

        // displays prompting message for new predator/prey relation when quietMode is off
        if (!quietMode) printf("Enter the pair of indices for a predator/prey relation.\n");
        if (!quietMode) printf("Enter any invalid index when done (-1 2, 0 -9, 3 3, etc.).\n");
        if (!quietMode) printf("The format is <predator index> <prey index>: ");
        
        scanf("%d %d",&predInd, &preyInd); // reads in predator/prey relation 
        if (!quietMode) printf("\n");
    }
    printf("\n");

    printf("--------------------------------\n\n");
    printf("Initial food web complete.\n");
    printf("Displaying characteristics for the initial food web...\n");
    
    displayAll(web,numOrgs,false); // displays food web characteristics for unmodified (initial) web

    if (!basicMode) { // checks if basicMode is off
        printf("--------------------------------\n\n");
        printf("Modifying the food web...\n\n");
        char opt = '?'; // stores web modification option

        // continues prompting and reading in modifications until 'q' is read
        while (opt != 'q') {
            if (!quietMode) { // checks if quietMode is off
                // displays options
                printf("Web modification options:\n");
                printf("   o = add a new organism (expansion)\n");
                printf("   r = add a new predator/prey relation (supplementation)\n");
                printf("   x = remove an organism (extinction)\n");
                printf("   p = print the updated food web\n");
                printf("   d = display ALL characteristics for the updated food web\n");
                printf("   q = quit\n");
                printf("Enter a character (o, r, x, p, d, or q): ");
            }
            scanf(" %c", &opt); // reads in modification option
            if (!quietMode) printf("\n\n"); 

            if (opt == 'o') { // adds a new organism
                char newName[20]; // stores name of new organism
                if (!quietMode) printf("EXPANSION - enter the name for the new organism: ");
                scanf("%s",newName); // reads in new organism
                if (!quietMode) printf("\n");
                printf("Species Expansion: %s\n", newName);
                addOrgToWeb(&web,&numOrgs,newName); // adds organism to web
                printf("\n");

                if (debugMode) { // checks if debugging mode is on
                    printf("DEBUG MODE - added an organism:\n");
                    printWeb(web,numOrgs); // displays web as it is built and modified
                    printf("\n");
                }

            } else if (opt == 'x') { // removes an organism
                int extInd; // stores index of extinct organism
                if (!quietMode) printf("EXTINCTION - enter the index for the extinct organism: ");
                scanf("%d",&extInd); // reads in extinct organism index
                if (!quietMode) printf("\n");
                if (extInd >= 0 && extInd < numOrgs) { // checks for valid extinct organism index
                    printf("Species Extinction: %s\n", web[extInd].name);
                    removeOrgFromWeb(&web,&numOrgs,extInd); // removes organism from food web
                } else {
                    printf("Invalid index for species extinction\n");
                }
                printf("\n");
                
                if (debugMode) { // checks if debugging mode is on
                    printf("DEBUG MODE - removed an organism:\n");
                    printWeb(web,numOrgs); // displays web as it is built and modified
                    printf("\n");
                }

            } else if (opt == 'r') { // adds a new predator/prey relation
                if (!quietMode) printf("SUPPLEMENTATION - enter the pair of indices for the new predator/prey relation.\n");
                if (!quietMode) printf("The format is <predator index> <prey index>: ");
                scanf("%d %d",&predInd, &preyInd); // reads in predator and prey index
                if (!quietMode) printf("\n");

                if (addRelationToWeb(web,numOrgs,predInd,preyInd)) { // displays new food source if relation is successfully added
                    printf("New Food Source: %s eats %s\n", web[predInd].name, web[preyInd].name);
                };
                printf("\n");
                if (debugMode) { // checks if debugging mode is on
                    printf("DEBUG MODE - added a relation:\n");
                    printWeb(web,numOrgs); // displays web as it is built and modified
                    printf("\n");
                }

            } else if (opt == 'p') { // prints updated food web
                printf("UPDATED Food Web Predators & Prey:\n");
                printWeb(web,numOrgs); // displays updated food web
                printf("\n");
                
            } else if (opt == 'd') { // displays all characteristics for updated food web
                printf("Displaying characteristics for the UPDATED food web...\n\n");
                displayAll(web,numOrgs,true); // displays all characteristics of web

            }
            printf("--------------------------------\n\n");
        
        }
        
    }

    freeWeb(web,numOrgs);

    return 0;
}