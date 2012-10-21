/*Assignmet 3 FTP Server And Client
  Written By: Rajkumar Singh (09010138)*/



#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>


#define BUFF_LEN 128

#define CMD_LEN  1024	

//Maximum number of clients that can connect
#define MAX_USER_LIMIT	10

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))


/**
 *	Structure: customer
  * Represents the structure used by the server to store information
  * Has the necessary communication with the client
 */

typedef struct {
  	
	int sock,pid; 		//communication socket; process pid son serving the customer

  	struct in_addr addrip;	//client IP address. s_addr
	
   	unsigned short int dataport;	//port for the connection for transferring data (files, list files in a dir)

  	char curdir[256];	//directory in which lies the Client

} client_t, *client;


extern client clients[];   //table for clients	
extern int nb_users;       //users currently signed
extern stop;		  //variable test: used to stop the server

/* prototypes des fonctions */

void init_conn(void); 	//accept; procedure that accepts / rejects a new customer
		
void waitchild(int);	//child waiting for a process
	
void main_loop(void);	//main loop

void exec_comm(int i);	

