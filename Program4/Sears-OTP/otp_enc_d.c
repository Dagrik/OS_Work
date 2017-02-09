//  Joshua Sears
//  CS 344 Fall 2016
//  Program 4 - OTP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>

void error(const char *msg) { fprintf(stderr,"%s\n", msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int listenSocketFD, estabConFD, port, charsRead, spawnpid, fd, fd2, fd3, count;
	socklen_t sizeOfClientInfo;
	char buffer[100000];
	struct sockaddr_in serverAddress, clientAddress;
    int pidCount = 0;
    int pidArray[40] = {0};
    int ctr, childExitMethod;
    char pFile[256];
    char keyFile[256];
    char keyBuffer[100000];
    char encString[100000];
    char* tokStr = malloc(256* sizeof(char));
    int newval, origVal, encval;
    char newkey, newChar, origChar, encChar;
    
	if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

	// Set up the address struct
	memset((char *)&serverAddress, '\0', sizeof(serverAddress));
	port = atoi(argv[1]); // Get the port number from string
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Create socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSocketFD < 0) error("ERROR listen socket");

	// Enable bind and listen
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("ERROR on binding");
	listen(listenSocketFD, 5); //Socket on for listening
    
    //infinite loop
    while(1){
	sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client
    
    estabConFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
	if (estabConFD < 0) error("ERROR on accept");
    
    //Create child process to handle processing and store pid value
    spawnpid = fork();
    pidArray[pidCount] = spawnpid;
    pidCount++;

    switch(spawnpid){
        case 0:
 
        
        //Handshake to ensure authorized connection from otp_enc using symbol "@"
        
        memset(buffer, '\0', strlen(buffer));
        charsRead = recv(estabConFD, buffer, 255, 0); // Get client syn
        if (charsRead < 0) error("ERROR reading from socket");
        
         //Check for valid syn
        if(strcmp(buffer, "@")){
            charsRead = send(estabConFD, "Wrong", 5, 0);
            close(estabConFD); // Close the existing socket which is connected to the client
            exit(1);
        }
            
        //If good, send ack in response
        charsRead = send(estabConFD, "Send", 5, 0);
        memset(buffer, '\0', strlen(buffer));
        
        //Obtain message from client
        charsRead = recv(estabConFD, buffer, 255, 0); 
        if (charsRead < 0) error("ERROR reading from socket");
            
        //Break down buffer to get the filename for plaintext and the key file
       
        memset(pFile, '\0', 256);
        memset(keyFile, '\0', 256);
            memset(tokStr, '\0', 256);
            
        tokStr = strtok(buffer, " ");
            strcpy(pFile, tokStr);
            tokStr = strtok(NULL, " ");
            strcpy(keyFile, tokStr);
            
        //Open plaintext file
            
            fd = open(pFile, O_RDONLY);
            
            if(fd == -1){
                error("Error opening plaintext file");
                exit(1);
            }
            read(fd, buffer, 100000);
            
            
        //Open keyfile
            
            fd2= open(keyFile, O_RDONLY);
            if(fd2 == -1){
                error("Error opening key file");
                exit(1);
            }
            
            read(fd2, keyBuffer, 100000);
            
            
            
            //Check for invalid characters
            int counter=0;
            while(buffer[counter] != '\n'){
                int checker = buffer[counter];
                if(checker != 32){
                    if((checker < 65) || (checker > 90)){
                        charsRead = send(estabConFD, "^", 1, 0); // Send invalid key error
                        if (charsRead < 0) error("ERROR writing to socket");
                        close(estabConFD);
                        exit(1);
                    }
                }
                counter+=1;
            }
            
            //Check for key length
            if(strlen(keyBuffer) < strlen(buffer)) {
                charsRead = send(estabConFD, "*", 21, 0); // Send key-too-short failure
                if (charsRead < 0) error("ERROR writing to socket");
                close(estabConFD);
                exit(1);
            }
            count = 0;
        //Begin text encryption, then output text to stdout
            while(buffer[count] != '\n'){
                
                newkey = keyBuffer[count];
                newval = newkey;  //store char as int for value
                if(newval == 32){
                    newval = 0;
                }else {
                newval = newval - 64; //drop into range of 1 to 26
                }
                
                char origChar = buffer[count];
                origVal = origChar;
            if(origVal == 32){
                origVal=0;
            }else{
                origVal = origVal - 64;
            }
                
                //Convert origChar to encChar
                encval = origVal + newval;
                if(encval > 26){
                    encval = encval - 26;
                }
                
                if(encval == 0){
                    encval = 32;
                } else {
                    encval = encval + 64;
                    
                }
                encChar = encval;
                
               
                //Add to encString
                encString[count] = encChar;
                
                count += 1;
            }
            
        
            //Ensure message is completely sent
            //http://stackoverflow.com/questions/14184181/send-function-in-c-how-to-handle-if-not-all-bytes-are-sent
            charsRead = 0;
            int offset = 0;
            while(charsRead < strlen(encString)){
                charsRead = send(estabConFD, encString+offset, strlen(encString)-offset, 0); // Send encrypted message
                if (charsRead < 0) error("ERROR writing to socket");
                if(charsRead < strlen(encString)) {
                    offset += charsRead;
                }
            }
        close(estabConFD); // Close the existing socket
        exit(0);
    
    
        default:
        //Parent Process catch children
            for(ctr=0; ctr<40;ctr++){
                if((waitpid(pidArray[ctr], &childExitMethod, 0)) && (pidArray[ctr]!=0)){
                    pidArray[ctr] = 0;
                    pidCount -=1;
                }
            }
}
    }
        close(listenSocketFD); // Close the listening socket
        return 0; 
}
