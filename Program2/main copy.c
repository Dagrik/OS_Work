//
// LOOK AT MAKING ROOM SELECTION CONTAINED IN FIRST STEPS
//  main.c
//  HW2
//
//  Created by Josh Sears on 10/19/16.
//  Copyright © 2016 Josh Sears. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void fileCreator(char* name1, char* name2, char* name3, char* name4, char* name5, char* name6, char* name7, const char* dirBuffer, int* statArray){
    
    int roomarray[7][7] = {{0}};
    int rmCount[7] = {0};
    int t1, t2;
    int counter=0;
    int complete = 0;
    int file_descriptor;
    
    while(!complete){
        
        do{
            t1 = rand() % 7;
            t2 = rand() % 7;
        } while (t1 == t2);
        if(rmCount[t1] < 6 && rmCount[t2] < 6){
            if(roomarray[t1][t2] != 1){
                roomarray[t1][t2] = 1;
                roomarray[t2][t1] = 1;
                rmCount[t1] +=1;
                rmCount[t2] +=1;
            }
        }
        complete = 1;
        for(int count=0; count<7; count++){
            if(rmCount[count] < 3){
                complete=0;
                break;
            }
        }
    }
    
    
    //Randomly choose the 7 rooms
    int nameArray[10]={0};
    
    for(int count=0; count<7; count++){
        int choice = rand() % 10;
        while(nameArray[choice]==1){
            choice = rand() % 10;
        }
        nameArray[choice]=1;
    }
    
    
    
    
    
    
    //Decide start and end points
    int startspot, endspot;
    
    do{
        startspot=rand()%7;
        endspot=rand()%7;
    } while (startspot == endspot);
    
    statArray[startspot]=1;
    statArray[endspot]=2;
    
    
    
    
    //Form directory for files
    
    
    
    
    int result = mkdir(dirBuffer, 0777);
    
    
    //create file
    char nameBuff[20];
    counter = 0;
    for(int count=0; count<10; count++){
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
            switch(counter){
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
            
            
            
            char tempBuff[255];
            strcpy(tempBuff, dirBuffer);
            strcat(tempBuff, nameBuff);
            strcat(tempBuff, ".txt");
            
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
            write(file_descriptor, tempBuff, 255);
            memset(tempBuff, 0, sizeof(tempBuff));
        }
        
    }
    
    
    //Cycle back through files
    char tempBuff[255];
    
    for(int cycle=1; cycle<8;cycle++){
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
        lseek(file_descriptor, -1, SEEK_END);
        if (file_descriptor < 0)
        {
            fprintf(stderr, "Could not open %s\n", tempBuff);
            perror("in main");
            exit(1);
        }
        counter = 0;
        int row = 0;
        
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
        for(int count=0; count<7; count++){
            if (roomarray[row][count]==1){
                counter++;
                switch (count){
                    case 0:
                        snprintf(tempBuff,255, "CONNECTION %d: %s \n", counter, name1);
                        break;
                    case 1:
                        snprintf(tempBuff,255, "CONNECTION %d: %s \n", counter, name2);
                        break;
                    case 2:
                        snprintf(tempBuff,255, "CONNECTION %d: %s \n", counter, name3);
                        break;
                    case 3:
                        snprintf(tempBuff,255, "CONNECTION %d: %s \n", counter, name4);
                        break;
                    case 4:
                        snprintf(tempBuff,255, "CONNECTION %d: %s \n", counter, name5);
                        break;
                    case 5:
                        snprintf(tempBuff,255, "CONNECTION %d: %s \n", counter, name6);
                        break;
                    case 6:
                        snprintf(tempBuff,255, "CONNECTION %d: %s \n", counter, name7);
                        break;
                        
                }
                
                write(file_descriptor, tempBuff, 255);
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
        lseek(file_descriptor, -1, SEEK_END);
        memset(tempBuff, 0, sizeof(tempBuff));
        
        if(statArray[checkStat] == 1){
            snprintf(tempBuff, 255, "ROOM TYPE: START_ROOM");
        } else if (statArray[checkStat] == 2){
            snprintf(tempBuff, 255, "ROOM TYPE: END_ROOM");
        } else {
            snprintf(tempBuff, 255, "ROOM TYPE: MID_ROOM");
        }
        
        write(file_descriptor, tempBuff, 255);
        memset(tempBuff, 0, sizeof(tempBuff));
        memset(nameBuff,0,sizeof(nameBuff));
    }

    
    return;
}

int main() {
    
    char name1[20] = "";
    char name2[20] = "";
    char name3[20] = "";
    char name4[20] = "";
    char name5[20] = "";
    char name6[20] = "";
    char name7[20] = "";
    int file_descriptor;
    
    char dirBuffer[50];
    snprintf ( dirBuffer, 49, "./searsjo.rooms.%d/", getpid() );
    int statArray[7]={0};
   
    
    srand(time(NULL));
    
    fileCreator(&name1, &name2, &name3, &name4, &name5, &name6, &name7, &dirBuffer, &statArray);
    
    

    
    
    
        return 0;

}
