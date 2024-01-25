##Lab 1##

##Project description##

The is project consist of two file client1 and server1. The client1 program should read from a file that has been provided to us. 
The client1 program will then send the file one line at a time, with each line containing a set of key:value pairs. Each key:value pair is separated by a space,
there will be at most one msg:"sample text" per line. The server1 program will then receive the lines of the file and parse the line into key:value pairs and store
them. The server will then printout the key/values it received, should be printed out in field of 20 characters each. Client1 will take the ip address and port 
number as parameters and the server should run and continue to accept message from anywhere.

##how to use##

1. open lab1 folder in multiple shells
	- files: client1.c, server1.c, message.txt, readme.txt, makefile

2. $ make

3. start server by $ ./server1 <port number> in one shell

4. start client by $ ./client1 <ipaddress> <portnumber> in another shell


##important notes## 
txt file must be name messages.txt or client1 will not be able to run
