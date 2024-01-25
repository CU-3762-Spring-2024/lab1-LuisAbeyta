#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>



int main(int argc, char *argv[])
{
	int sd; // the socket descriptor 
	struct sockaddr_in server_address;  // structures for addresses 
	struct sockaddr_in inaddr;  // structures for checking addresses 
	int rc;
	int i; // loop variable 
	char serverIP[20]; // provided by the user on the command line */
	int portNumber = 0; // provided by the user on the command line
	char bufferOut [512]; //string to hold a line of the file
	
	FILE* file  = fopen("messages.txt", "r"); //open the input file to be transfered
	
	//check file opened properly
	if(!file)
	{
		printf("\n unable to open");
		exit(1);
	}
	
	//check for correct amount of variables
	if (argc < 3){
		printf ("usage is client <ipaddr> <portnumber>\n");
		exit(1); /* just leave if wrong number entered */
	}
	
	//check for a valid ip address
	//dotted notation and has valid numbers
	if (!inet_pton(AF_INET, argv[1], &inaddr)){
		printf ("error, bad ip address\n");
		exit (1); /* just leave if is incorrect */
	}
  
	//create socket
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	
	//check that port number is a number
	for (i=0;i<strlen(argv[2]); i++){
		if (!isdigit(argv[2][i]))
		{
			printf ("The Portnumber isn't a number!\n");
			exit(1);
		}
	}
	
	portNumber = strtol(argv[2], NULL, 10); /* many ways to do this */
	
	// exit if a portnumber too big or too small  
	if ((portNumber > 65535) || (portNumber < 0)){
		printf ("you entered an invalid socket number\n");
		exit (1);
	}

	strcpy(serverIP, argv[1]); //put ipaddress into string

	server_address.sin_family = AF_INET; /* use AF_INET addresses */
	server_address.sin_port = htons(portNumber); /* convert port number */
	server_address.sin_addr.s_addr = inet_addr(serverIP); /* convert IP addr */
  
	//fgets(bufferOut, sizeof(bufferOut), file);
  
	//printf("%s", bufferOut);
	//rc = sendto(sd, bufferOut, strlen(bufferOut), 0,
	//      (struct sockaddr *) &server_address, sizeof(server_address));
  
	//read each line of the file into bufferOut
	//should terminate when eof is reached
	while(fgets(bufferOut, sizeof(bufferOut), file))
	{
		
		rc = sendto(sd, bufferOut, strlen(bufferOut), 0,
	      (struct sockaddr *) &server_address, sizeof(server_address));
		  
		//check return code of the sendto
		//if less than length of string there is an error
		if (rc < strlen(bufferOut)){
			perror ("sendto");
		// do i exit?
		}
  
		printf("%s", bufferOut);
		memset (bufferOut, 0, 512);
	}
	
	//check return code of the sendto
	//if less than length of string there is an error
	//close the file 
	fclose(file);
	
	return 0;
}