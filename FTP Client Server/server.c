/*Assignmet 3 FTP Server And Client
  Written By: Rajkumar Singh (09010138)*/
#include "server.h"


client clients[MAX_USER_LIMIT];

int stop = 0;
int sd_server=0;		//socket to listen on the server
const int maxusers= MAX_USER_LIMIT;	// max users can connect
int nb_users=0;       		//users currently signed
int max_user_sock = 0;    //maximum value of socket
int serverport = 2100;		//listening port of the server
char *prompt_symb = "serv>";	//command prompt for server administration


/**
 *	Procedure: prompt
 
 *	Displays the command prompt "serv>"
 */

void prompt(void) {
  
	printf("%s",prompt_symb);
  
	fflush(stdout);
}

/**
 *	Procedure: init_conn
 
 *	procedure that accepts / rejects a new customer
 */

void init_conn(void){


	int i, sd_client, len;
	char buff[BUFF_LEN];
	struct sockaddr_in cl_addr;
	struct hostent *host;

	len = sizeof(cl_addr);
	printf("Requesting\n");
 	if( 0 > (sd_client = accept(sd_server, (struct sockaddr *)&cl_addr, &len)) ){

		perror("accept");
		return;
	} 

	//test if the number of clients connected maximum is reached
	
	if(nb_users ==  maxusers){

		//the maximum number of users connected is reached!
		printf("\nthe maximum number of users connected is reached!\n");

		memset(buff, '\0', BUFF_LEN);
		sprintf(buff, "The server is responsible\r\n");
		send(sd_client, buff, strlen(buff), 0);
		close(sd_client);
		return;
	}
	else{ //it accepts the connection and filling the structure of the client
	
		clients[nb_users]->addrip.s_addr = cl_addr.sin_addr.s_addr; // you must copy the client's IP address in the structure 		
		clients[nb_users]->sock = sd_client;
		strcpy(clients[nb_users]->curdir,"/home/raj");		
		clients[nb_users]->pid = 0;
		clients[nb_users]->dataport = 0;
		printf("\nA new client has connected\n");
		max_user_sock = MAX(max_user_sock, clients[nb_users]->sock);
		nb_users++;
		printf("Number of clients connected: %d\n", nb_users);
		prompt();
	}
	
	memset(buff, '\0', BUFF_LEN);
	sprintf(buff, "welcome\r\n");
	
	if(send(sd_client, buff, strlen(buff), 0)<0){
		perror("error in sending\n");
		exit(0);
		}
}

/**
 *	Procedure: waitchild
 *child waiting for a process
 */

void waitchild(int n) {

	while (wait3(NULL, WNOHANG, NULL) > 0);
	
}

/**
 *	Procedure: main_loop
 *	main loop of the server: bind, listen, select
 */


void main_loop(void) 
{
  	int i,n,addr;
  	unsigned int sock_size;
  	fd_set readfds, testfds;
  	char cmd[CMD_LEN],buffer[1000];
  	struct sockaddr_in sa;

  	signal(SIGPIPE,SIG_IGN);
  	signal(SIGCHLD,waitchild);

  	/* server init */

  	printf("Starting the server ...\n");

  	if ((sd_server=socket(AF_INET,SOCK_STREAM,0)) < 0) {
    
		perror("socket");
    		exit(1);
  	}

  	sock_size=sizeof(struct sockaddr_in);
  	memset(&sa,0,sock_size);
  
  	sa.sin_family=AF_INET;
  	sa.sin_port=htons(serverport);
  	addr=INADDR_ANY;
  	memcpy(&sa.sin_addr.s_addr,&addr,sizeof(int));
  
  	if (bind(sd_server,(struct sockaddr *)&sa,sock_size)<0) {

    		perror("bind");
		exit(1);
  	}
	//printf("binded to socket\n");
  	if (listen(sd_server,5)<0) {
    
		perror("listen");
    		exit(1);
  	}
	init_conn(); 
    	//printf("OK\n");
    	//recv(sd_server,buffer,strlen(buffer),0);
	//printf("%s",buffer);
    	//prompt();

	/*FD_ZERO(&readfds);
    	FD_SET(1,&readfds);
    	FD_SET(sd_server,&readfds);
	*/
	stop = 0;
  	while (!stop) {	
			testfds = readfds;
		    	for (i = 0; i < nb_users; i++)
			{
      			if (clients[i]->sock)
				FD_SET(clients[i]->sock,&testfds);
				exec_comm(i);
			}
		//selectoin an active descriptor
       		if( 0 > (n = select(max_user_sock, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0))){					
      		//	if (errno!=EINTR || n != -1) 
                //		exit(1);
		}
		//reading information from clients
   		/*for (i = 0; i <= nb_users; i++) {
   			printf("clients are there\n");
      			if (clients[i]->sock) {
				//if (FD_ISSET(clients[i]->sock,&testfds))
	  			//exec_comm(i); //i - number of client in the client table
				prompt();
      			}
    		}*/
  	}
close(sd_server);
printf("server stops\n");
}

/**
 *	Fonction: main
 */

int main(int argc, char *argv[]){

	int i;

	for (i = 0; i < maxusers; i++) {
    
		clients[i]=(client_t *)malloc(sizeof(client_t));
    		clients[i]->sock=0;
  	}

	main_loop();
	

	for(i = 0; i < maxusers; i++) {
      	
		if (clients[i]) {
			free(clients[i]);
			clients[i]=(client_t *)NULL;
      		}

	}
return(0);
}
