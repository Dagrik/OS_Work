//  Joshua Sears
//  CS 344 Fall 2016

//  Program 4 - OTP

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg) { fprintf(stderr,"%s\n", msg); exit(1); }

int main(int argc, char *argv[])
{
	int socketFD, port, charsWritten, charsRead;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[100000];
    
	if (argc < 4) { fprintf(stderr,"USAGE: %s plaintext key port\n", argv[0]); exit(0); } // Check usage & args

	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	port = atoi(argv[3]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; 
	serverAddress.sin_port = htons(port); // Store the port number
	serverHostInfo = gethostbyname("localhost"); // Host is localhost
	if (serverHostInfo == NULL) { error("ERROR, no such host\n"); }
	memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

	// Set up the socket
	socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (socketFD < 0) error("ERROR opening socket");
	
	// Connect to server
	if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
		error("ERROR connecting");
    
    //Handshake
    charsWritten = send(socketFD, "#", 2, 0); // Write to the server
    if (charsWritten < 0) error("ERROR writing to socket");
    if (charsWritten < strlen(buffer)) printf("WARNING: Not all data written to socket!\n");
    
    memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
    charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); 
    if (charsRead < 0) error("ERROR reading from socket");
    if(strcmp(buffer, "Send")){
    	error("ERROR: otp_dec cannot use otp_enc_d");
        close(socketFD);
        exit(1);
    }
    
	// Build message of plaintext filename and key filename to send to otp_enc_d
    memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
    strcpy(buffer, argv[1]);
    strcat(buffer, " ");
    strcat(buffer, argv[2]);

//Send file info to dec_d
	charsWritten = 0;
    int offset = 0;
    while(charsWritten < strlen(buffer)){
        charsWritten = send(socketFD, buffer+offset, strlen(buffer)-offset, 0); 
        if (charsWritten < 0) error("ERROR writing to socket");
        if (charsWritten < strlen(buffer)){
            offset += charsWritten;
        }
    }

	// Get encrypted string from opt_enc_d
	memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer again for reuse
	charsRead = recv(socketFD, buffer, sizeof(buffer) - 1, 0); // Read data from the socket
	if (charsRead < 0) error("ERROR reading from socket");
    if(strcmp(buffer, "^")){
        printf("%s\n", buffer);
    }else{	//Error for bad characters in file
    memset(buffer, '\0', sizeof(buffer)); // Clear out the buffer array
    strcpy(buffer, "ERROR: Bad characters in ");
    strcat(buffer, argv[1]);
    error(buffer);
    }
	close(socketFD); // Close the socket
	return 0;
}
