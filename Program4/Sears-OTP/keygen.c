//  Joshua Sears
//  CS 344 Fall 2016

//  Program 4 - OTP

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

int main (int argc, char* argv[]){
    long val;
    char* strPtr = NULL;
    int nextVal = 0;
    char newChar;
    int count  = 0;
    
    srand(time(NULL));  //Create random values each run
    
    val = atoi(argv[1]); //, &strPtr, 0);  //strtol separates numbers from string input
   /* if(val == 0 || strcmp(strPtr, "")){
        perror("keygen: invalid input");
        exit(1);
    }
    */
    char keystring[val+1];  //Creates string with "+1" for newline character
    
    for(count= 0; count < val; count++){
        nextVal = rand()%27 + 65;   //Into range of A
        if(nextVal >= 91){          //If nextVal passed Z, make Z as space char (ASCII 30)
            nextVal = 32;
        }
        newChar = nextVal;          //Convert int to char
        keystring[count] = newChar; //Store char in array
    }
    
    keystring[count] = '\n';        //Add newline char
    write(STDOUT_FILENO, keystring, val+1);
    
    return 0;
}
