/*Assignmet 3 FTP Server And Client
  Written By: Rajkumar Singh (09010138)*/
#include "exec_comm.h"


const char *commandes[8] = {

	"QUIT", "PORT", "LIST", "CWD", "RETR", "RGET",  "STOR","MGET"
};

int nb_users;

const char *ok = "OK\r\n";
 char msz[20],tmp[200];



void exec_comm(int cl){

	int size,j=0,i=0, port;
	char buff[1024], *param,*command, *param1,*filename;
	char commands[100],buff2[200],dir[200];
	fd_set fds;

	memset(buff, '\0', 1024);

	if((recv(clients[cl]->sock, buff, sizeof(buff), 0))<0){
		perror("error in Receiving");
		exit(1);
		}
	printf("%s\n",buff);
	command = strtok (buff," ");		
	
	//buff[size-2]=0;  // -'\r' et  -'\n'
	/*while(buff[j]){
		param[i]=buff[j+1];
		i++;j++;
	}*/
	//param = strchr(buff," "); // param <- commande
	param = strtok (NULL, " ");
	
	for (j = 0; j < strlen(command); j++)
    		command[j]=toupper(command[j]);
  	printf("%s\n",command);
  	
  	printf("%s\n",param);
	for (j = 0; j < 8; j++)

		if (!strcmp(command,commandes[j])) break;

	

	switch(j){

		case QUIT:	if (clients[cl]->pid) kill(clients[cl]->pid,SIGTERM);
				clients[cl]->pid=0;
				close(clients[cl]->sock);
				clients[cl]->sock=0; 
  				clients[cl]->dataport=0;
				nb_users--;
				printf("%s\n", commandes[j]);
				break;	

		case  PORT:	port = atoi(param);
				clients[cl]->dataport = port;
				printf("%s %s\n", commandes[j], param);
				send(clients[cl]->sock, ok, strlen(ok)+1, 0);
				break;	
	
		case CWD:	strcpy(tmp,clients[cl]->curdir);
				strcat(tmp,"/");
				strcat(tmp,param);
				if(chdir(tmp)<0){
					perror("error");
					memset(msz,'\0',sizeof(msz));
					sprintf(msz,"%s","No Such Directory");
					send(clients[cl]->sock, msz, strlen(msz)+1, 0);
				}
				else{	
					memset(clients[cl]->curdir, '\0', (strlen(clients[cl]->curdir)+1));
					strcpy(clients[cl]->curdir,tmp);
					memset(msz,'\0',sizeof(msz));
					sprintf(msz,"%s","Directory Changed");
					send(clients[cl]->sock, msz, strlen(msz)+1, 0);
					printf("%s  %s\n", commandes[j],clients[cl]->curdir);
				}	
				prompt();
				break;	

		

		case RGET:	//param1 = strtok(param,"-");
				//filename = strtok(NULL,"-");
				//port = atoi(param1);
				//clients[cl]->dataport = port;
				//send(clients[cl]->sock, ok, strlen(ok)+1, 0);
				//	printf("%s %s\n", commandes[j], filename);
				
				strcat(commands,"ls -R ");
				system("ls");
				strcat(commands,param);
				printf("%s",param);
				system(commands);
				FILE *fd = popen(commands,"r");
				while(fgets(buff, sizeof(buff), fd)!=NULL){
					if(buff[(strlen(buff)-1)]==':'){
						//strcpy(dir,buff);
						//printf("%s",dir);
						memcpy(dir,buff,strlen(buff)-2);
						if(send(clients[cl]->sock,buff, strlen(buff), 0)<0)
								perror("Send");
					}
					else{
					strcat(dir,"param/");
					strcat(dir,buff);
					strcpy(buff,dir);
					if(send(clients[cl]->sock,buff, strlen(buff), 0)<0)
						perror("Send");
					}		
				}
				//rget(cl,param);
				break;
		 
		 case MGET:				
				strcat(commands,"ls ");
				//system("ls");
				strcat(commands,param);
				printf("%s",param);
				//system(commands);
				fd = popen(commands,"r");
				while(fgets(buff, sizeof(buff), fd)!=NULL){
					printf("\n%s",buff);
					if(send(clients[cl]->sock,buff, strlen(buff), 0)<0)
						perror("Send");
					}
				
				//rget(cl,param);
				break;
		
		
		// Since the list of files can be a considerable size, it is better to create a child
		// Which will deal with the client
		case LIST:	port = atoi(param);
				printf("%d",port);
				clients[cl]->dataport = port;
				if( (clients[cl]->pid = fork()) == 0 ){ 
					send(clients[cl]->sock, ok, strlen(ok)+1, 0);
					my_list(cl);
				}
				//send(clients[cl]->sock, ok, strlen(ok)+1, 0);
				printf("%s %s\n", commandes[j], clients[cl]->curdir);
				break;

		// Because a file can be of great size, it is better to create a child Which will deal with the sending to the client
		case RETR:	param1 = strtok(param,"-");
				filename = strtok(NULL,"-");
				port = atoi(param1);
				clients[cl]->dataport = port;
				if( (clients[cl]->pid = fork()) == 0 ) { 
					send(clients[cl]->sock, ok, strlen(ok)+1, 0);
					retr(cl, filename);
				}
				printf("%s %s\n", commandes[j], param);
				break;

		// Because a file can be of great size, it is better to create a child Which will deal with the recovery from the client

		case STOR:	param1 = strtok(param,"-");
				filename = strtok(NULL,"-");
				port = atoi(param1);
				clients[cl]->dataport = port;
				if( (clients[cl]->pid = fork()) == 0 ){
					if(send(clients[cl]->sock, ok, strlen(ok)+1, 0)<0)
						perror("send");
					stor(cl, filename);
				}
				printf("%s %s\n", commandes[j], param);	
				break;

		// Because a file can be of great size, it is better to create a child Which will deal with the removal

		
		
		//By default, we consider that if a client sends commands incomprehensible, it must be disconnected
		default:	if (clients[cl]->pid) kill(clients[cl]->pid,SIGTERM);
				if (clients[cl]->pid) kill(clients[cl]->pid,SIGTERM);
  				clients[cl]->pid=0;
				close(clients[cl]->sock);
				clients[cl]->sock = 0;
				clients[cl]->dataport = 0;
				nb_users--;
				printf("Client %d Disocnnected\n",cl);
				break;
				prompt();
	}	
}


/**
 *	Procedure: my_list
 
 *	sends the list of files in a directory to the client
 */


void my_list(int cl){

	struct dirent *dirp;
	DIR *dirfd;
	int sd, tolen, nb;
	struct sockaddr_in to;
	struct hostent *host;

	to.sin_family = AF_INET;
	to.sin_port = clients[cl]->dataport;
 	to.sin_addr.s_addr = clients[cl]->addrip.s_addr; //IP
	tolen = sizeof(to);

	if( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){

		perror("socket");
		exit(1);
	}

	//the server connects to the client
	if(connect(sd, (struct sockaddr*)&to, tolen)<0){

		perror("connect MY_LIST");
		exit(1);
	}

	//the server opens the named directory desired by the customer
	if( !(dirfd = opendir(clients[cl]->curdir))){

		perror("opendir");
		exit(1);
	}

	//the server sends what he has read from the directory to the client
	while( (dirp = readdir(dirfd)) != NULL) {

		send(sd, dirp->d_name, 1024*sizeof(char), 0);
	}

closedir(dirfd);
close(sd);

exit(0);
}
void rget(int cl, char* expr){
	struct dirent *dirp;
	DIR *dirfd;
	int sd, tolen, nb;
	struct sockaddr_in to;
	struct hostent *host;

	to.sin_family = AF_INET;
	to.sin_port = clients[cl]->dataport;
 	to.sin_addr.s_addr = clients[cl]->addrip.s_addr; //IP
	tolen = sizeof(to);

	if( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){

		perror("socket");
		exit(1);
	}

	//the server connects to the client
	if(connect(sd, (struct sockaddr*)&to, tolen)<0){

		perror("connect List Of Mput");
		exit(1);
	}

	//the server opens the named directory desired by the customer
	char commands[100],buff[200],buff2[200],dir[200];
	strcpy(commands,"ls -R ");
	strcat(commands,expr);
	FILE *fd = popen(commands,"r");
	while(fgets(buff, strlen(buff), fd)!=NULL){
		if(buff[(strlen(buff)-1)]==':'){
			memcpy(dir,buff,strlen(buff)-2);
			if(send(sd,buff, strlen(buff), 0)<0)
				perror("Send");
		}
		else{
			strcat(dir,"/");
			strcat(dir,buff);
			strcpy(buff,dir);
			if(send(sd,buff, strlen(buff), 0)<0)
				perror("Send");
		}		
	}
	

	//the server sends what he has read from the directory to the client
	


close(sd);
fclose(fd);
exit(0);
}

/*
   *
  * Get a file from the client. In fact the cclient want to send a file on the server.
  * To do this, he sent port (data port) on which it waits for a connection
  * From the server.
  * From the server side, when the port has received the data, we try to  a connection to the client.
  * Once the connection is established, the server is ready to receive data from the client.
  * The file will be received in the current directory of the client (server side)
  */

void stor(int cl, char *filename){

	int fd, nb0=1, nb1=1;
	char buff[1024];
	char fichier[1024]; //contains the absolute path
	int s, tolen, nb;
	struct sockaddr_in to;
	struct hostent *host;
	
	memset(fichier, '\0', 1024*sizeof(char));
	strcat(fichier, clients[cl]->curdir);
	strcat(fichier,"/");
	strcat(fichier, filename);
	
	printf("stor FILE: %s\n", fichier);

	//fd = open(fichier, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);  //read/write

	to.sin_family = AF_INET;
	to.sin_port = clients[cl]->dataport;
 	to.sin_addr.s_addr = clients[cl]->addrip.s_addr; //IP
	tolen = sizeof(to);

	if( (s = socket(AF_INET, SOCK_STREAM, 0)) < 0){

		perror("socket");
		exit(1);
	}
	
	if(0 > connect(s, (struct sockaddr*)&to, tolen)){

		perror("connect STOR");
		exit(1);
	}
	FILE *f1;
	if((f1 = fopen(fichier,"w"))<0)
		perror("Opening a File");  //read/write
	memset(buff, '\0', 1024*sizeof(char));
	
	while( nb1 > 0 ){
                nb1 = recv(s, buff, sizeof(buff), 0 ); //sizeof() + 1
                if(strcmp(buff,"finish")==0)
                	break;
                else	
	                fprintf(f1,"%s",buff);
		
		//printf("%s",buff);
        	 // nb1 = ; //strlen + 1  
        	//fprintf(f1,"%s",buff);
		
              }

	close(s);
	fclose(f1);
	s = 0;
	//exit(1);
	//prompt();
}

/**
 *	Procedure: retr
 *
  * Get a file from the server. In fact the client want to download a file from the server.
  * To do this, he sent premieremt port (data port) on which it waits for a connection
  * From the server.
  * From the server side, when the port has received the data, we try to  a connection to the client.
  * Once the connection is established, the server is ready to send data to client.
  * The file will be received in the directory where the client was started.
 */

void retr(int cl, char *filename){

	int fd, nb0, nb1;
	char buff[1024];
	char fichier[1024];
	int sd, tolen, nb;
	struct sockaddr_in to;
	struct hostent *host;

	memset(fichier, '\0', 1024*sizeof(char));
	strcat(fichier, clients[cl]->curdir);
	strcat(fichier,"/");
	strcat(fichier, filename);

	printf("retr FILE: %s\n", fichier);	
	
	if( (fd = open(fichier, O_RDONLY)) == -1 ){  //read

		perror("open");
		exit(1);

	}

	to.sin_family = AF_INET;
	to.sin_port = clients[cl]->dataport;
 	to.sin_addr.s_addr = clients[cl]->addrip.s_addr; //IP
	tolen = sizeof(to);

	if( (sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){

		perror("socket");
		exit(1);
	}

	if(0 > connect(sd, (struct sockaddr*)&to, tolen)){

		perror("connect");
		exit(1);
	}

	while( (nb = read(fd, buff, 1024)) > 0 ){

		//printf("sent %d\n", nb);
                send(sd, buff, nb, 0);
		printf("sent %d\n", nb);	
        }
	
	close(sd);
	sd=0;

exit(0);
}






