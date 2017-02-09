// CS344 Project 2
// Josh Sears
// Fall 2016
//
// Text adventure with reading and writing files
// File names based off key words from Jedi Code
//
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
/***********
/ Function: File Creator
/ Desc: Creates 7 random room files and places them in predetermined file folder
/ Pre: none
/ Post: name1 - name7, statArray
************/
void fileCreator(char* name1, char* name2, char* name3, char* name4, char* name5, char* name6, char* name7, const char* dirBuffer, int* statArray){
    
    int roomarray[7][7] = {{0}};    //2D array to chart pathways
    int rmCount[7] = {0};           //Tracks numbers of paths from each room
    int t1, t2;
    int counter=0;
    int complete = 0;
    int file_descriptor;
    int count, cycle;               //For loops variables

    //Pick two random rooms, place values in table so path goes both ways
    while(!complete){
        
        do{
            t1 = rand() % 7;
            t2 = rand() % 7;
        } while (t1 == t2);  //Two rooms that aren't the same
        if(rmCount[t1] < 6 && rmCount[t2] < 6){
            if(roomarray[t1][t2] != 1){  //If path doesn't already exist
                roomarray[t1][t2] = 1;
                roomarray[t2][t1] = 1;
                rmCount[t1] +=1;
                rmCount[t2] +=1;
            }
        }
        complete = 1;
        for(count=0; count<7; count++){
            if(rmCount[count] < 3){
                complete=0;
                break;
            }
        }
    }
    
    
    //Randomly choose the 7 rooms from 10 names
    int nameArray[10]={0};  //Stores which rooms picked
    
    for(count=0; count<7; count++){
        int choice = rand() % 10;
        while(nameArray[choice]==1){  //Ensure room only picked once
            choice = rand() % 10;
        }
        nameArray[choice]=1;
    }
    
    
    
    
    
    
    //Decide start and end points, store positions for reference when game runs
    int startspot, endspot;
    
    do{
        startspot=rand()%7;
        endspot=rand()%7;
    } while (startspot == endspot);
    
    statArray[startspot]=1;
    statArray[endspot]=2;
    
    
    
    
    //Form directory for files
    
    int result = mkdir(dirBuffer, 0777);  //Create directory "searsjo.rooms.[PID]"
    
    
    //create file
    char nameBuff[20];
    counter = 0;
    
    //Check nameArray for which files to create based off 7 names picked, make filename if nameArray val is 1
    for(count=0; count<10; count++){
        if (nameArray[count]==1){
            
            switch (count){
                case 0:
                    snprintf(nameBuff,20, "%s","Force");
                    break;
                case 1:
                    snprintf(nameBuff,20, "%s","Emotion");
                    break;
                case 2:
                    snprintf(nameBuff,20, "%s","Ignorance");
                    break;
                case 3:
                    snprintf(nameBuff,20, "%s","Knowledge");
                    break;
                case 4:
                    snprintf(nameBuff,20, "%s","Peace");
                    break;
                case 5:
                    snprintf(nameBuff,20, "%s","Chaos");
                    break;
                case 6:
                    snprintf(nameBuff,20, "%s","Harmony");
                    break;
                case 7:
                    snprintf(nameBuff,20, "%s","Passion");
                    break;
                case 8:
                    snprintf(nameBuff,20, "%s","Serenity");
                    break;
                case 9:
                    snprintf(nameBuff,20, "%s","Death");
                    break;
            }
            
            counter++;
            switch(counter){    //Store room names in name# char arrays for future use
                case 1:
                    strcpy(name1, nameBuff);
                    
                    break;
                case 2:
                    strcpy(name2, nameBuff);
                    
                    break;
                case 3:
                    strcpy(name3, nameBuff);
                    
                    break;
                case 4:
                    strcpy(name4, nameBuff);
                    
                    break;
                case 5:
                    strcpy(name5, nameBuff);
                    
                    break;
                case 6:
                    strcpy(name6, nameBuff);
                    
                    break;
                case 7:
                    strcpy(name7, nameBuff);
                    
                    break;
            }
            
            
            //Build tempBuff array, holds directory and file name information
            char tempBuff[255];
            strcpy(tempBuff, dirBuffer);
            strcat(tempBuff, nameBuff);
            strcat(tempBuff, ".txt");
            
            //Create file
            file_descriptor = open(tempBuff, O_WRONLY|O_CREAT, 0755);
            if (file_descriptor < 0)
            {
                fprintf(stderr, "Could not open %s\n", tempBuff);
                perror("in main");
                exit(1);
            }
            
            //Adds ROOM NAME entry to file
            memset(tempBuff, 0, sizeof(tempBuff));
            snprintf(tempBuff, 255, "ROOM NAME: %s\n", nameBuff);
            char* token = strtok(tempBuff, "'\0'");
            write(file_descriptor, token, strlen(token));
            memset(tempBuff, 0, sizeof(tempBuff));  //http://www.cplusplus.com/reference/cstring/memset/  Clears tempBuff
        }
        
    }
    
    
    //Cycle back through files to fill contents
    char tempBuff[255];
    
    for(cycle=1; cycle<8;cycle++){
        strcpy(tempBuff, dirBuffer);
        memset(nameBuff, 0, sizeof(nameBuff));
        switch(cycle){
                
            case 1:
                strcpy(nameBuff, name1);
                break;
            case 2:
                strcpy(nameBuff, name2);
                break;
            case 3:
                strcpy(nameBuff, name3);
                break;
            case 4:
                strcpy(nameBuff, name4);
                break;
            case 5:
                strcpy(nameBuff, name5);
                break;
            case 6:
                strcpy(nameBuff, name6);
                break;
            case 7:
                strcpy(nameBuff, name7);
                break;
        }
        
        strcat(tempBuff, nameBuff);
        strcat(tempBuff, ".txt");
        
        file_descriptor = open(tempBuff, O_WRONLY);
        lseek(file_descriptor, 0, SEEK_END);
        if (file_descriptor < 0)
        {
            fprintf(stderr, "Could not open %s\n", tempBuff);
            perror("in main");
            exit(1);
        }
        counter = 0;
        int row = 0;
        
        //Choose which 2d array row to look at
        if(!strcmp(nameBuff, name1)){
            row = 0;
        } else if (!strcmp(nameBuff, name2)){
            row = 1;
        }else if (!strcmp(nameBuff, name3)){
            row = 2;
        }else if (!strcmp(nameBuff, name4)){
            row = 3;
        }else if (!strcmp(nameBuff, name5)){
            row = 4;
        }else if (!strcmp(nameBuff, name6)){
            row = 5;
        }else {
            row = 6;
        }
        counter=0;
        memset(tempBuff, 0, sizeof(tempBuff));
        
        //If roomArray element is 1, add as connection in room file
        for(count=0; count<7; count++){
            if (roomarray[row][count]==1){
                counter++;
                switch (count){
                    case 0:
                        snprintf(tempBuff,255, "CONNECTION %d:%s\n", counter, name1);
                        break;
                    case 1:
                        snprintf(tempBuff,255, "CONNECTION %d:%s\n", counter, name2);
                        break;
                    case 2:
                        snprintf(tempBuff,255, "CONNECTION %d:%s\n", counter, name3);
                        break;
                    case 3:
                        snprintf(tempBuff,255, "CONNECTION %d:%s\n", counter, name4);
                        break;
                    case 4:
                        snprintf(tempBuff,255, "CONNECTION %d:%s\n", counter, name5);
                        break;
                    case 5:
                        snprintf(tempBuff,255, "CONNECTION %d:%s\n", counter, name6);
                        break;
                    case 6:
                        snprintf(tempBuff,255, "CONNECTION %d:%s\n", counter, name7);
                        break;
                        
                }
                char* token = strtok(tempBuff, "'\0'");
                write(file_descriptor, token, strlen(token));
                memset(tempBuff, 0, sizeof(tempBuff));
            }
            
            memset(tempBuff, 0, sizeof(tempBuff));
            
        }
        
        int checkStat=0;
        if(!strcmp(nameBuff, name1)){
            checkStat = 0;
        } else if (!strcmp(nameBuff, name2)){
            checkStat = 1;
        }else if (!strcmp(nameBuff, name3)){
            checkStat = 2;
        }else if (!strcmp(nameBuff, name4)){
            checkStat = 3;
        }else if (!strcmp(nameBuff, name5)){
            checkStat = 4;
        }else if (!strcmp(nameBuff, name6)){
            checkStat = 5;
        }else {
            checkStat = 6;
        }
        
        strcpy(tempBuff, dirBuffer);
        strcat(tempBuff, nameBuff);
        strcat(tempBuff, ".txt");
        file_descriptor = open(tempBuff, O_WRONLY);
        lseek(file_descriptor, 0, SEEK_END);
        memset(tempBuff, 0, sizeof(tempBuff));
        
        
        //Check statArray to see if room was chosen to be the start,end, or mid room and write to file
        if(statArray[checkStat] == 1){
            snprintf(tempBuff, 255, "ROOM TYPE: START_ROOM");
        } else if (statArray[checkStat] == 2){
            snprintf(tempBuff, 255, "ROOM TYPE: END_ROOM");
        } else {
            snprintf(tempBuff, 255, "ROOM TYPE: MID_ROOM");
        }
        char* token = strtok(tempBuff, "'\0'");
        write(file_descriptor, token, strlen(token));
        memset(tempBuff, 0, sizeof(tempBuff));
        memset(nameBuff,0,sizeof(nameBuff));
    }

    
    return; //return to main
}


/***********
 / Function: runGame
 / Desc: Runs the user through the rooms until END_ROOM is located
 / Pre: name1 - name7, dirBuffer, statArray
 / Post: pathBuffer, rmMoves
 ************/
void runGame (char* name1, char* name2, char* name3, char* name4, char* name5, char* name6, char* name7, char* dirBuffer, int* rmMoves, char* pathBuffer, int* statArray){
    
    char startRm[20] = "";  //Holds name of starting room
    char endRm[20] = "";    //Holds name of ending room
    char nameBuff[20] = "";
    char userInput[20] = "";     //Holds user input string
    char tempUserInput[20] = "";
    int ending = 0;
    int file_descriptor;
    int count, ch;
 
    //Determine start and end rooms
    for(count = 0; count < 7; count++){
        if(statArray[count]==1){  //If room chosen as start
            switch(count){
                    
                case 0:
                    strcpy(startRm, name1);
                    break;
                case 1:
                    strcpy(startRm, name2);
                    break;
                case 2:
                    strcpy(startRm, name3);
                    break;
                case 3:
                    strcpy(startRm, name4);
                    break;
                case 4:
                    strcpy(startRm, name5);
                    break;
                case 5:
                    strcpy(startRm, name6);
                    break;
                case 6:
                    strcpy(startRm, name7);
                    break;
            }
            
        }else if(statArray[count]==2){ //if room chosen as end
            switch(count){
                    
                case 0:
                    strcpy(endRm, name1);
                    break;
                case 1:
                    strcpy(endRm, name2);
                    break;
                case 2:
                    strcpy(endRm, name3);
                    break;
                case 3:
                    strcpy(endRm, name4);
                    break;
                case 4:
                    strcpy(endRm, name5);
                    break;
                case 5:
                    strcpy(endRm, name6);
                    break;
                case 6:
                    strcpy(endRm, name7);
                    break;
            }
        }
    }
    
    //Open start room
    char file[255]="";
    strcpy(file, dirBuffer);
    strcat(file, startRm);
    strcat(file, ".txt");
    
    //Game loop
    do{
        file_descriptor = open(file, O_RDONLY);
        if (file_descriptor < 0)
        {
            fprintf(stderr, "Could not open %s\n", file);
            exit(1);
        }
        
        char tempRead[255]="";
        char tempLocString[255]="";
        char locString[255]="";
        char* tempString;
        int roomFound=0;
        
        //Get room name for display
        read(file_descriptor, tempRead, 255);
        tempString = strtok(tempRead, "' :''\n'");
        tempString = strtok(NULL, "' :''\n'");
        tempString = strtok(NULL, "' :''\n'");
        
        printf("CURRENT LOCATION: %s\nPOSSIBLE CONNECTIONS: ", tempString);
        tempString = strtok(NULL, ":'\n'");
        
        //Display room connections and query user for next move
        while(tempString && !roomFound){
            
            if(!strncmp(tempString, "CONNECTION", 10)){
                tempString = strtok(NULL, ":'\n'");         //if tempString has "connection", tokenize to get next string
                printf("%s", tempString);
                strcat(locString, tempString);              //Add tempString to list of rooms
                strcat(locString, " ");
                tempString = strtok(NULL, ":'\n'");         //if next token is "connection", place comma after name
                if(!strncmp(tempString, "CONNECTION", 10)){
                    printf(", ");
                } else {
                    printf(".\n");
                }
            }
            if(!strcmp(tempString, "ROOM TYPE")){       //if next token is "room type"
                tempString = strtok(NULL, " :'\n'");
                
                //if ROOM TYPE is not "END_ROOM"
                if(strcmp(tempString, "END_ROOM")){
                    printf("WHERE TO? >");//Prompt user for input choice
                    memset(tempUserInput, 0, 20);
                    memset(userInput, 0, 20);
                    fgets(tempUserInput, 20, stdin);
                    strncpy(userInput, tempUserInput, strlen(tempUserInput)-1);
                    fflush(stdin);
                    strcpy(tempLocString, locString);
                    tempString=strtok(tempLocString, " ");
                    while(tempString && !roomFound){
                        if(!strcmp(tempString, userInput)){
                            strcat(pathBuffer, tempString);     //Add userInput to path history
                            strcat(pathBuffer, " ");
                            printf("\n");
                            strcpy(file, dirBuffer);            //build file path for next file
                            strcat(file, userInput);
                            strcat(file, ".txt");
                            (*rmMoves)++;                       //increment count of steps
                            roomFound=1;
                            if(!strcmp(endRm, userInput)){      //If room is END, return to main()
                                return;
                            }
                            break;
                        }
                        tempString=strtok(NULL, " ");
                    }
                    if(roomFound==0){  //Error if userInput is incorrect
                        printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
                    }
                } else {

                    return;
                }
            }
        }
    } while(!ending);
    

}


/***********
 / Function: printResults
 / Desc: Prints the total steps and rooms traveled
 / Pre: none
 / Post: name1 - name7, statArray
 ************/
void printResults(int* rmMoves, char* pathBuffer){
    printf("YOU HAVE FOUND THE END ROOM.  CONGRATULATIONS!\n");
    printf("YOU TOOK %d STEPS.  YOUR PATH TO VICTORY WAS:\n", *rmMoves);
    
    //Print list of rooms traveled for user.
    char* pathList = strtok(pathBuffer, " ");
    do{
        printf("%s\n", pathList);
        pathList = strtok(NULL, " ");
        
    } while (pathList);
}


//Main function
//Calls functions to create files, run the game, and print the results
int main() {
    
    char name1[20] = "";
    char name2[20] = "";
    char name3[20] = "";
    char name4[20] = "";
    char name5[20] = "";
    char name6[20] = "";
    char name7[20] = "";

    int rmMoves=0;
    char dirBuffer[50];
    int statArray[7]={0};
    char pathBuffer[512] = "";
    
    srand(time(NULL));      //Creates seed to randomize RAND() based off clock
    
    snprintf ( dirBuffer, 49, "./searsjo.rooms.%d/", getpid() );  //creates directory in dirBuffer
    
    //Function which creates files
    fileCreator(name1, name2, name3, name4, name5, name6, name7, dirBuffer, statArray);
    
    //Begin running game
    runGame(name1, name2, name3, name4, name5, name6, name7, dirBuffer, &rmMoves, pathBuffer, statArray);
    
    //Upon successful completion, display results
    printResults(&rmMoves, pathBuffer);
    
    return 0;

}
