#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>


/*********************************************************/
/* this is the server for lab0. it will receive a msg    */
/* from the client, and print it out. Make this your     */
/* own. Comment it, modularize it, learn it, fix it, etc */
/*********************************************************/

struct keyValue
{
    char key[20];
    char value[100];
};

struct keyValue rec[1000];
int openIndex = 0;

void parseMsg(char * input)
{
	//string to hold the key and the value
    char key[512] = "";
    char value[512] = ""; 
	int keyStart = 0;  //where the key value starts
	bool msgBegin = false; //indicates a " has been seen and we are looking at a message
    int limit = 0;		//key stop at limit -1 and value starts at limit + 1
	int i;				//variable for loop
 
	//loop through the string
	for(i = 0; i < strlen(input);i++){
		

		//if the character of the string is a punctation and not a '"' or '-' or the message of the string has begun
		if(input[i] == ':' && input[i] != '"' && input[i] != '-' && !msgBegin)
		{
			limit = i;
		}
		//if we see the '"' character
		else if(input[i] == '"')
		{
			//see the second quotation mark end the message
			if(msgBegin)
			{
				msgBegin = false;
			}
			//see the first quotation mark begin message
			else
			{
				msgBegin = true;
			}
		}
		//space character ends a key:value pair, only look at if not in the message
		else if(input[i] == ' ' && !msgBegin)
		{
			//the delimiter will be found at a greater index than the start of a key
			//only enter if this is true, else the input is bad and we ignore it
			if(limit > keyStart)
			{
				//reset strings
				memset(key, 0, 512);
				memset(value, 0, 512);
				
				//copy values from input to key and value
				strncpy(key, input + keyStart, limit - keyStart);
				strncpy(value, input + limit+1, i - limit - 1);
				
				//store key and value
				strncpy(rec[openIndex].key, key, strlen(key));
				strncpy(rec[openIndex].value, value,strlen(value));
				
				//index that is open in the struct
				openIndex++;
				
				//print out the key and value
				printf("%20s", key);
				printf("%20s\n", value);
			}
			
			keyStart = i;
			
		}
	   
	}
	//for last key value pair if input is valud
	if(limit > keyStart){
		
		//reset strings	
		memset(key, 0, 512);
		memset(value, 0, 512);
		
		//copy values from input to key and value
		strncpy(key, input + keyStart, limit - keyStart);
		strncpy(value, input + limit+1, i - limit - 1);
		
		//store key and value
		strncpy(rec[openIndex].key, key, strlen(key));
		strncpy(rec[openIndex].value, value,strlen(value));
		
		//index that is open in the struct		
		openIndex++;
		
		//print out the key and value
		printf("%20s", key);
		printf("%20s\n", value);
		
	}
	
	printf("\n");
}


int main(int argc, char *argv[])
{

	int openIndex = 0;
	int sd; /* socket descriptor */
	int i;  /* loop variable */
	struct sockaddr_in server_address; /* my address */
	struct sockaddr_in from_address;  /* address of sender */
	char bufferReceived[1000]; // used in recvfrom
	int portNumber; // get this from command line
	int rc; // always need to check return codes!
	socklen_t fromLength;
	int flags = 0; // used for recvfrom
  
	/* first, decide if we have the right number of parameters */
	if (argc < 2){
		printf("usage is: server <portnumber>\n");
		exit (1);
	}


	/* first create a socket */
	sd = socket(AF_INET, SOCK_DGRAM, 0); /* create a socket */

	/* always check for errors */
	if (sd == -1){ /* means some kind of error occured */
		perror ("socket");
		exit(1); /* just leave if wrong number entered */
	}

	/* now fill in the address data structure we use to sendto the server */
	for (i=0;i<strlen(argv[1]); i++){
		if (!isdigit(argv[1][i]))
		{
			printf ("The Portnumber isn't a number!\n");
			exit(1);
		}
	}

	portNumber = strtol(argv[1], NULL, 10); /* many ways to do this */

	if ((portNumber > 65535) || (portNumber < 0)){
		printf ("you entered an invalid port number\n");
		exit (1);
	}

	server_address.sin_family = AF_INET; /* use AF_INET addresses */
	server_address.sin_port = htons(portNumber); /* convert port number */
	server_address.sin_addr.s_addr = INADDR_ANY; /* any adapter */

	/* the next step is to bind to the address */
	rc = bind (sd, (struct sockaddr *)&server_address,
		 sizeof(struct sockaddr ));

	if (rc < 0){
		perror("bind");
		exit (1);
	}


	/* NOTE - you MUST MUST MUST give fromLength an initial value */
	fromLength = sizeof(struct sockaddr_in);
	
	printf("%20s","Name");
	printf("%20s\n","Value");

	while(true){
		memset (bufferReceived, 0, 1000); // zero out the buffers in C
		
		rc = recvfrom(sd, bufferReceived, 1000, flags,
			(struct sockaddr *)&from_address, &fromLength);

	  /* check for any possible errors */
		if (rc <=0){
			perror ("recvfrom");
			printf ("leaving, due to socket error on recvfrom\n");
			exit (1);
		}
		//parse the message received
		parseMsg(bufferReceived);
		/* print out what i get */
		//printf ("received '%s'\n", bufferReceived);
    }

} // end of main()
