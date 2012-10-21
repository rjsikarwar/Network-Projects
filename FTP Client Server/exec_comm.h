/*Assignmet 3 FTP Server And Client
  Written By: Rajkumar Singh (09010138)*/

#include "server.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>


/** 
 * 	
 *
 */

// The client leaves the server
#define QUIT 0     

//The client sends its data port
#define PORT 1

//The client wants the file list in the rep. current
#define LIST 2

// The client wants to change the current directory
#define CWD  3


//The client wants to download a file
#define RETR 4

// 
#define RGET 5


//The client wants to send a file on the server
#define STOR 6

#define MGET 7

/* Prototypes des fonctions */

void exec_comm(int);	//execute the commands

void my_list(int);	//execute a command
void stor(int,char *);	//has recovered a file from the client
void retr(int,char *);	//send a file to the client
void rget(int, char *);

