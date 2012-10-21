/*Assignmet 3 FTP Server And Client
  Written By: Rajkumar Singh (09010138)*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>


#define N 1024
#define NB_CMDS	13

#define OPEN	0
#define GET 	1 	/*Get a file from server to client*/
#define PUT 	2 	/*Put a file into Server's current directory*/
#define MGET 	3 	/*get multiple files from server to client */
#define MPUT	4 	/*Put Multiple files in server's current directory*/
#define CD 	5 	/*change server's current Directory*/
#define LCD 	6 	/*Change client directory */
#define MGET_ 	7 	/* mget with wildcard support */
#define MPUT_	8 	/* mput with wildcard support */
#define DIR 	9 	/* show the content of server's current directory */
#define LDIR	10 	/* show the content of client's current directory */
#define RGET	11	/*Recursiverly get content of directory */
#define RPUT	12	/*Recursiverly put content to server's current directory */
const char *PROMPT="ftp> ";

void prompt(void);

void myconnect(char *);
void cd(char *);
void lcd(char *);
void dir(void);
void ldir(void);
void get(char *);
void put(char *);
void mget(char *);
void mput(char *);
void mget_(char *);
void mput_(char *);
void rget(char *);
void rput(char *);

