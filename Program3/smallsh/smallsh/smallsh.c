//  Joshua Sears
//  Program 3 - smallsh - unix shell
//  CS344 - Fall 2016
//
//  Runs a simple shell in Unix to handle commands with up to 512 args, and 2048 characters
//  handles input and output redirects "<"  or ">", and creating background processes with "&"
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

/***********************
 ** Function: Main
 ** Desc: Run a basic shell in unix to handle commands with up to
 **       2048 characters or 512 arguments, including input and output redirects
 **       and to run commands in the background
 **Prereqs: None
 **Post: None
 **********************/

int main(){
    int childExitMethod;
    pid_t spawnid = -5;
    char curPath[512] = "";
    char* cwdCheck = "";
    char tempPath[2048] = "";
    char userCmd[2048]="";
    char tempCmd[2048]="";
    char* tempStr = "";
    char* inStr = malloc(255*sizeof(char));
    char* outStr = malloc(255*sizeof(char));
    char* temp = "";
    char** cmdLine = malloc(512*sizeof(char*));
    int exitStat = 0;
    int count = 0;
    int isOutRedir = 0;
    int isInRedir = 0;
    int ctr, fd, fd2, fd3, fd4;
    int isBG=0;
    int pidArray[20] = {0};
    int pidCount = 0;
    
    
    //Create sigaction struct to control SIGINT
    struct sigaction catcher;
    
    //Get initial home directory
    chdir(getenv("HOME"));
    cwdCheck = getcwd(curPath, strlen(curPath));
    
    
    //Do while loop to run shell program, loop until command "exit" given by user
    do{
        
        //Parent to ignore signals
        catcher.sa_handler=SIG_IGN;
        sigfillset(&catcher.sa_mask);
        catcher.sa_flags=0;
        sigaction(SIGINT, &catcher, NULL);
        
        //Reset count and empty command and path storage strings
        count = 0;
        memset(tempPath, 0, 2048);
        memset(userCmd, 0, 2048);
        memset(tempCmd, 0, 2048);
        memset(inStr, 0, strlen(inStr));
        memset(outStr, 0, strlen(outStr));
        
        //Print new command line for user, obtain commands
        printf(": ");
        fflush(stdout);
        fgets(tempCmd, 2047, stdin);
        fflush(stdin);
        
        //Only proceed if first char of input not "#" and string not empty
        if((strncmp(tempCmd, "#", 1)) && (strncmp(tempCmd, "", strlen(tempCmd)-1))){
        
            //Drop newline from end of user input
            strncpy(userCmd, tempCmd, strlen(tempCmd)-1);

            //***INTREPRET COMMANDS***//
            
            //IF exit command given
            if(!strcmp(userCmd, "exit") || !strcmp(userCmd, "Exit")){
                break;
            
            //IF status command, print last exit value or termination value
            }else if(!strcmp (userCmd, "status")){
                
                printf("Exit Value %d\n", exitStat);
                fflush(stdout);
            
            //IF change directory
            }else if(!strncmp (userCmd, "cd", 2)){
                //empty char*
                if(tempStr != NULL){
                    memset(tempStr, 0, strlen(tempStr));
                }
                
                int check=strlen(userCmd);
                
                //If command longer than "cd "
                if(strlen(userCmd)>3){
                    
                    //Check if filepath starts with ".", "~", or "/"
                    if((!strncmp(userCmd+3, ".", 1)) || (!strncmp(userCmd+3, "~", 1)) || (!strncmp(userCmd+3, "/", 1))
                       || (!strncmp(userCmd+3, curPath, strlen(userCmd+3)))){
                    
                        
                        memset(curPath, 0, 512);
                        strcpy(curPath, userCmd+3);
                        check = chdir(curPath);
                        if(check != 0){
                            printf("Error, destination doesn't exist");
                            exitStat=1;
                            fflush(stdout);
                        }
                        strcpy(tempPath, curPath);
                        cwdCheck = getcwd(curPath, sizeof(curPath));
                        printf("\nCurrent Path: %s\n", cwdCheck);
                        fflush(stdout);
                        if(cwdCheck == NULL){
                            printf("\nError: directory does not exist!\n");
                            exitStat=1;
                            fflush(stdout);
                        }
                        
                    //Else, add "/" in front of suggested path
                    } else {
                        if(strcmp(curPath, "/")){
                            strcat(curPath, "/");
                        }
                        strcat(curPath, userCmd+3);
                        check = chdir(curPath);
                        if(check != 0){
                            printf("Error, destination doesn't exist");
                            fflush(stdout);
                            exitStat=1;
                        }
                        strcpy(tempPath, curPath);
                        cwdCheck = getcwd(curPath, sizeof(curPath));
                        fflush(stdout);
                        if(cwdCheck == NULL){
                            printf("\nError: directory does not exist!\n");
                            fflush(stdout);
                            exitStat=1;
                        }
                        
                    }
                
                // Else command is only "cd", send to home directory
                } else {
                    check = chdir(getenv("HOME"));  //evaluate environment variable "HOME"
                    if(check != 0){
                        printf("Error, destination doesn't exist");
                        fflush(stdout);
                        exitStat=1;
                    }
                    cwdCheck = getcwd(curPath, sizeof(curPath));
                    if(cwdCheck == NULL){
                        printf("\nError: directory does not exist!\n");
                        fflush(stdout);
                        exitStat=1;
                    }
                }
                
             //else use fork and exec commands
            
            } else {
                
                //Build string array for fork using strtok
                if(tempStr != NULL ){
                memset(tempStr, 0, strlen(tempStr));
                }
                
                tempStr=strtok(userCmd, " ");
                
                //If command longer than one word
                while(tempStr != NULL){
                    
                    //If in/out redirect or background operator found
                    if((!strcmp(tempStr, "<")) || (!strcmp(tempStr, ">")) || (!strcmp(tempStr, "&"))){
                        if(!strcmp(tempStr, "<")){
                            isInRedir = 1;  //set flag
                            tempStr=strtok(NULL, " ");
                            strcpy(inStr, tempStr);
                            break;  //Skip adding operator to string
                        }
                        if(!strcmp(tempStr, ">")){
                            isOutRedir = 1; //set flag
                            tempStr=strtok(NULL, " ");
                            strcpy(outStr,tempStr);
                            break;  //Skip adding operator to string
                        }
                        if(!strcmp(tempStr, "&")){
                            isBG=1; //set flag
                            
                            break;  //Skip adding operator to string
                        }
                    }
                    else if(!strncmp(tempStr, "$", 1)){
                            temp = getenv(tempStr+1);
                            memset(tempStr, 0, 2048);
                            strcpy(tempStr, temp);
                        
                    }
                    
                    //Dynamically add to char array and increment count, get next value
                    cmdLine[count] = strdup(tempStr);
                    count+=1;
                    tempStr=strtok(NULL, " ");
                }
                cmdLine[count] = NULL;
                
                spawnid = fork();
                if(isBG){
                    pidArray[pidCount]=spawnid;
                    pidCount++;
                }
                
                switch (spawnid){
                    case -1:
                        printf("\nError in command\n");
                        exit (1);
                        break;
                        
                    case 0:
                        //Changes process to default for SIGINT on foreground child process
                        if(!isBG){
                            catcher.sa_handler=SIG_DFL;
                            sigfillset(&catcher.sa_mask);
                            catcher.sa_flags=0;
                            sigaction(SIGINT, &catcher, NULL);
                        }
                        
                        //Set output redirect
                        if(isOutRedir){
                            fd = open(outStr, O_WRONLY|O_CREAT|O_TRUNC, 0644);
                            if(fd == -1){
                                perror("Unable to create or write to file");
                                exitStat=1;
                                exit(1);
                            }
                            fd2 = dup2(fd, 1);
                            if(fd2 == -1){
                                perror("dup2");
                                exitStat=1;
                                exit(1);
                            }
                        }
                        
                        //set input redirect
                        if(isInRedir){
                            fd3 = open(inStr, O_RDONLY);
                            if(fd3 == -1){
                                perror("Error opening file for reading");
                                exitStat=1;
                                exit(1);
                            }
                            fd4 = dup2(fd3, 0);
                            if(fd4 == -1){
                                perror("dup2");
                                exitStat=1;
                                exit(1);
                            }
                        }
                        
                        //Background redirect, checks for in or out redirect first
                        if(isBG){
                            printf("background pid is %d\n", getpid());
                            fflush(stdout);
                            
                            if(!isOutRedir){
                                fd = open("/dev/null", O_WRONLY|O_CREAT|O_TRUNC, 0644);
                                if(fd == -1){
                                    perror("Error opening file for reading");
                                    exitStat=1;
                                    exit(1);
                                }
                                fd2 = dup2(fd, 1);
                                if(fd2 == -1){
                                    perror("dup2");
                                    exitStat=1;
                                    exit(1);
                                }
                            }
                            if(!isInRedir){
                                fd3 = open("/dev/null", O_RDONLY);
                                if(fd3 == -1){
                                    perror("Error opening file for reading");
                                    exitStat=1;
                                    exit(1);
                                }
                                fd4 = dup2(fd3, 0);
                                if(fd4 == -1){
                                    perror("dup2");
                                    exitStat=1;
                                    exit(1);
                                }
                            }
                        }
                        
                        //Exec and error check
                        int checker = execvp(cmdLine[0], cmdLine);
                        if(checker == -1){
                            perror(cmdLine[0]);
                            exitStat=1;
                            exit(1);
                        }
                        break;
                        
                    default:
                        
                        //Call waitpid on foreground process, gets exit or termination code
                        if(!isBG){
                            waitpid(spawnid, &childExitMethod, 0);
                            if(WIFEXITED(childExitMethod)){
                                exitStat=WEXITSTATUS(childExitMethod);
                                
                                
                            } else if(WIFSIGNALED(childExitMethod)){
                                exitStat=WTERMSIG(childExitMethod);
                                printf("\nProcess %d was terminated with signal %d\n", spawnid, exitStat);
                                
                            }
                            fflush(stdout);
                        }
                }
                
                //Reset flags
                isOutRedir=0;
                isInRedir=0;
                isBG = 0;
                
                
                //Check for bg processes, provide exit or termination codes
                for(ctr=0; ctr<pidCount;ctr++){
                    if((waitpid(pidArray[ctr], &childExitMethod, WNOHANG)) && (pidArray[ctr]!=0)){
                        if(WIFEXITED(childExitMethod)){
                            int exitStat2=WEXITSTATUS(childExitMethod);
                            printf("\nBackground Process %d exited with code %d\n", pidArray[ctr], exitStat2);
                        }else if(WIFSIGNALED(childExitMethod)){
                            int exitStat2=WTERMSIG(childExitMethod);
                            printf("\nBackground Process %d was terminated with signal %d\n", pidArray[ctr], exitStat2);
                        }
                        fflush(stdout);
                        pidArray[ctr] = 0;
                    }
                }
        }}
    } while (strcmp(userCmd, "exit"));  //Exit while loop
    
    //Cleanup dynamic allocated space
    for(ctr=0; ctr<count;ctr++){
        free(cmdLine[ctr]);
    }
    free(cmdLine);
    free(inStr);
    free(outStr);
    //Kill any outstanding processes before exit
    int countz = 0;
    for(countz=0; countz<pidCount; countz++){
        if(pidArray[countz] != 0){
            kill(pidArray[countz], SIGKILL);
            waitpid(pidArray[countz], &childExitMethod, 0);
        }
    }
    return 0;
}
