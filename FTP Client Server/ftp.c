/*Assignmet 3 FTP Server And Client
  Written By: Rajkumar Singh (09010138)*/
#include "commands.h"

static char *commandes[NB_CMDS] = {


	"open","get", "put", "mget", "mput","cd", "lcd", "mget_", "mput_", "dir", "ldir", "rget", "rput"
};

int sd = 0;
int connection = 0;

int main(int argc, char *argv[]){

	char buff[N], *param; //1024
	int j = 0, nb;

	if( argc != 2){

		printf("Usage: %s <server host>\n", argv[0]);
		exit(1);
	}

	printf("commands\n");
	do{

		memset(buff, '\0', N);
		
		prompt();	
		gets(buff);
	
		param = strchr(buff,' '); // param <- commands
		if (param) {
    			*param=0;
    			param++;
  		}

		for(j = 0; j < NB_CMDS; j++)
			if( !strcmp(buff, commandes[j]))
				break;	
			
		switch(j){

 			case(OPEN):	if(connection) printf("already connected\n");
					else myconnect(argv[1]);
					break;
		
			case(GET):	if(!connection) printf("Not connected!\n"); 
					else get(param);
					break;

			case(PUT):	if(!connection) printf("Not connected!\n"); 
					else put(param);
					break;
				
			case(MGET):	if(!connection) printf("Not connected!\n"); 
					else mget(param);
					break;		
			case(MPUT):	if(!connection) printf("Not connected!\n"); 
					else mput(param);
					break;
			
			case(CD):	if(!connection) printf("NoT connecteD!\n"); 
					else cd(param);
					break;
			
			case(LCD):	lcd(param);
					break;
							
			case(MGET_):	if(!connection) printf("Not connected!\n"); 
					else mget_(param);
					break;		
			case(MPUT_):	if(!connection) printf("Not connected!\n"); 
					else mput_(param);
					break;
			

 			case(DIR):	if(!connection) printf("Not connecte!\n"); 
					else dir();
					break;
			case(LDIR):
					ldir();
					break;
			case(RGET):	if(!connection) printf("Not connected!\n"); 
					else rget(param);
					break;		
			case(RPUT):	if(!connection) printf("Not connected!\n"); 
					else rput(param);
					break;		

			//case (HELP):	aide();
			//		break;

			default:	printf("%s command invalid\n", buff);
					printf("Commands are as follows\n");
					printf(" get : To Get A file from Server\n");
					printf(" put : To Put A file to Server\n");
					printf(" cd : change Server's Current Directory\n");
					printf(" lcd : Change Client's current Directory\n");
					printf(" mget : To Get multiple files from Server\n");
					printf(" mput : To put multiple files from Server\n");
					printf(" dir : List the Server's current Directory\n");
					printf(" ldir : List the Client's current Directory\n");
					printf(" mget_ : Put files with wildcard support\n");
					printf("mput_ : get files with wildcard support\n");
					printf("rget : Get a directory recursively\n");
					printf("rput : Put a Directory recursiovely\n");
					break;
		}
		
	}
	while(1);
 	
return(0);
}

void myconnect(char *host_serv){

	struct sockaddr_in to;
	struct hostent *host;	
	int tolen, len;
	char buff[N];

	char user[N]; 	
	char pass[N]; 		
	char *cwd = "CWD /\n";

	to.sin_family = AF_INET;
	to.sin_port = htons(2100);
	host = gethostbyname(host_serv);
	memcpy(&(to.sin_addr), host->h_addr, sizeof(to.sin_addr));
	tolen = sizeof(to);

	sd = socket(AF_INET, SOCK_STREAM, 0); 
	
	if( 0 > connect(sd, (struct sockaddr *)&to, tolen)){

		perror("connect");
		exit(1);
	}
	
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else printf("%s\n",buff);

	
	//CWD 
	/*if( send(sd, cwd, strlen(cwd)+1, 0) < 0 ){

		perror("send");
		exit(2);
	}
	memset(buff, '\0', N*sizeof(char));*/
	connection = 1;
}
void cd(char *rep){

	char cwd[N];
	char buff[N];

	memset(cwd, '\0', N);
	strcat(cwd, "CWD ");
	strcat(cwd, rep);
	//strcat(cwd, "\n");

	if( send(sd, cwd, strlen(cwd)+1, 0) < 0 ){

		perror("send");
		exit(2);
	}
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else printf("cd %s\n",buff);
}
void lcd(char *rep){
	if(chdir(rep)<0)
		perror("changing Client directory");
	else
		printf("Directory changed to %s\n",rep);
}


void get(char *file){

	char port[N]; 
	char retr[N]; 
	char buff[N];
	int min_port = 1025;
	int max_port = 65535;
	struct sockaddr_in serv_addr, cl_addr;
	int sd_client, sd_server, len, nb0=1, nb1=1, fd, i;
	struct hostent *host;	

	memset(retr, '\0', N*sizeof(char));
	strcat(retr,"RETR ");
	
 	

	
        if( 0 > (sd_server = socket(AF_INET, SOCK_STREAM, 0))){

		perror("socket");
		exit(1);
	}

	//server becomes the client to the FTP server
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr  = INADDR_ANY;
	memset(&(serv_addr.sin_zero), '\0', 8);

        len = sizeof(cl_addr);

	for( i = min_port; i <= max_port; i++){

		serv_addr.sin_port = htons(i);

        	if(0 <= bind(sd_server, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) ) 
			break;
	}

	memset(buff, '\0', N*sizeof(char));
	memset(port, '\0', N*sizeof(char));
	sprintf(buff, "%d", serv_addr.sin_port);
	printf("GET %s %d\n", buff, htons(serv_addr.sin_port));
	
 	strcat(port, buff);
 	strcat(retr,buff);
	strcat(retr,"-");
	strcat(retr,file);
	//PORT
	/*if( send(sd, port, strlen(port)+1, 0) < 0 ){

		perror("send");
		exit(2);

	}
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else printf("port %s\n",buff);
	*/

	FILE *f1;
	if((f1 = fopen(file,"w"))<0)
		perror("Opening a File");  //read/write
	/*if(0 > (fd = open(file, O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR))) {  //read/write

		perror("open");
		exit(1);
	}*/	

	//RETR 
	if( send(sd, retr, strlen(retr)+1, 0) < 0 ){

		perror("send");
		exit(2);

	}
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else printf("retr %s\n",buff);

	if( 0 > listen(sd_server, 5) ){ 

		perror("listen");
		exit(1);
	}

    	if( 0 > (sd_client = accept(sd_server, (struct sockaddr *)&cl_addr, &len))){

		perror("accept");
		exit(1);
	}
	
	memset(buff, '\0', N*sizeof(char));
	while( nb1 > 0 ){

                nb1 = recv(sd_client, buff, sizeof(buff), 0 ); //sizeof() + 1
                fprintf(f1,"%s",buff);
                //nb0 = write(fd, buff, nb1);	

                if( nb0 == 0 ){

                        printf("fjsdfghk\n");
                        nb0 = 0;
                }
        }

	close(sd_server);
	close(sd_client);
	sd_server=0;
	sd_client=0;
	fclose(f1);
}

void put(char *file){

	char port[N]; 
	char stor[N]; 
	char buff[N];
	int min_port = 1025;
	int max_port = 65535;
	struct sockaddr_in serv_addr, cl_addr;
	int sd_client, sd_server, len, nb, fd, i;
	struct hostent *host;	

	memset(stor, '\0', N*sizeof(char));
	strcat(stor,"STOR ");
	//strcat(stor, file);
 	//strcat(stor,'-');
	printf("%s",stor);
	if( 0 > (sd_server = socket(AF_INET, SOCK_STREAM, 0))){

		perror("socket");
		exit(1);
	}

	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr  = INADDR_ANY;
	memset(&(serv_addr.sin_zero), '\0', 8);

        len = sizeof(cl_addr);

	for( i = min_port; i <= max_port; i++){

		serv_addr.sin_port = htons(i);

        	if(0 <= bind(sd_server, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) ) 
			perror("bind");
			break;
	}
	
	memset(buff, '\0', N*sizeof(char));
	memset(port, '\0', N*sizeof(char));
	sprintf(buff, "%d", serv_addr.sin_port);
	printf("PUT %s %d\n", buff, ntohs(serv_addr.sin_port));
	
 	strcat(stor, buff);
 	strcat(stor,"-");
 	strcat(stor,file);
	//strcat(stor,port);
	
	//PORT
	/*if( send(sd, port, strlen(port)+1, 0) < 0 ){

		perror("send");
		exit(2);

	}
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else printf("port %s\n",buff);
	*/
	if( 0 > (fd = open(file, O_RDONLY)) ){

		perror("open hjk");
		exit(1);
	}

	printf("sled OPEN\n");
	printf("the file to be send %s sur %d\n", stor, sd);
	//STOR
	if( send(sd, stor, strlen(stor)+1, 0) < 0 ){

		perror("send");
		exit(2);

	}
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else printf("stor %s\n",buff);

	if( 0 > listen(sd_server, 5) ){

		perror("listen");
		exit(1);
	}

        if( 0 > (sd_client = accept(sd_server, (struct sockaddr *)&cl_addr, &len) )){

		perror("accept");
		exit(1);
	}
	memset(buff, '\0', N*sizeof(char));
	while( (nb = read(fd, buff, N))  > 0){
		printf("sending file");
		if( 0 > send(sd_client, buff, nb, 0) ){

			perror("send");
			exit(1);
		}
	}
	memset(buff, '\0', N*sizeof(char));
	strcpy(buff,"finish");
	if( 0 > send(sd_client, buff, nb, 0) ){

			perror("send");
			exit(1);
		}
	close(sd_server); 
	close(sd_client); 
	sd_server=0;
	sd_client=0;
	close(fd);	
}

void dir(){

	char port[N]; 
	char list[N]; 
	char buff[N];
	int min_port = 1025;
	int max_port = 65535;
	struct sockaddr_in serv_addr, cl_addr;
	int sd_client, sd_server, len, i;
	struct hostent *host;	

	memset(list, '\0', N*sizeof(char));
	strcat(list,"LIST ");
 	
	//server becomes the client to the FTP server

	if( 0 > (sd_server = socket(AF_INET, SOCK_STREAM, 0) )){

		perror("socket");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr  = INADDR_ANY;
	memset(&(serv_addr.sin_zero), '\0', 8);

        len = sizeof(cl_addr);

	for( i = min_port; i <= max_port; i++){

		serv_addr.sin_port = htons(i);

        	if(0 <= bind(sd_server, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) ) 
			break;
	}
	
	memset(buff, '\0', N*sizeof(char));
	//memset(port, '\0', N*sizeof(char));
	sprintf(buff, "%d", serv_addr.sin_port);
	//printf("DIR %s %d\n", buff, htons(serv_addr.sin_port));
	//strcat(port,"PORT ");
 	strcat(list, buff);
	//strcat(port,"\n");
	
	//PORT
	/*if( send(sd, port, strlen(port)+1, 0) < 0 ){

		perror("send");
		exit(2);

	}
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else printf("port %s\n",buff);
	*/
	//LIST  
	if( send(sd, list, strlen(list)+1, 0) < 0 ){

		perror("send");
		exit(2);

	}
	memset(buff, '\0', N*sizeof(char));
	if( recv(sd, buff, sizeof(buff), 0 ) < 0){

		perror("recv");
		exit(5);
	}
	else printf("list %s\n",buff);

	if( 0 > listen(sd_server, 5) ){ 
	
		perror("listen");
		exit(1);
	}

        if( 0> (sd_client = accept(sd_server, (struct sockaddr *)&cl_addr, &len) )){

		perror("accept");
		exit(1);
	}

	memset(buff, '\0', N*sizeof(char));
	while( recv(sd_client, buff, N*sizeof(char), 0 ) > 0){

		printf("%s\n",buff);
		
	}

	close(sd_server);
	close(sd_client);
	sd_server=0;
	sd_client=0;
}
void prompt(void) {
  
	printf("%s",PROMPT);
  
	fflush(stdout);
}

void ldir(){
	system("ls");

}

void mget(char *param){
	char filename[30];
	int total = atoi(param);
	int i;
	for(i=0;i<total;i++)
		{
		 printf("Enter the %dth File Name to download from server\n",i+1);
		 gets(filename);
		 get(filename);
		 }
}
void mput(char *param){
	char filename[30];
	int total = atoi(param);
	int i;
	for(i=0;i<total;i++)
		{
		 printf("Enter the %dth File Name to send to the server\n",i+1);
		 gets(filename);
		 put(filename);
		 }	
}
void mget_(char *param){
	char port[N]; 
	char stor[N]; 
	char buff[N],filename[100];
	int min_port = 1025;
	int max_port = 65535;
	struct sockaddr_in serv_addr, cl_addr;
	int sd_client, sd_server, len, nb, i;
	struct hostent *host;
	FILE *fd;
	/*if( 0 > (sd_server = socket(AF_INET, SOCK_STREAM, 0))){

		perror("socket");
		exit(1);
	}

	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr  = INADDR_ANY;
	memset(&(serv_addr.sin_zero), '\0', 8);

        len = sizeof(cl_addr);

*/
	
	
	memset(buff, '\0', N*sizeof(char));
	memset(port, '\0', N*sizeof(char));
	sprintf(buff, "%d", serv_addr.sin_port);
	printf("PUT %s %d\n", buff, ntohs(serv_addr.sin_port));
	strcat(port,"MGET ");
	strcat(port,param);
	
	
	memset(buff, '\0', N*sizeof(char));

	if( send(sd, port, strlen(port)+1, 0) < 0 ){

		perror("send");
		exit(2);
	}
	memset(buff, '\0', N*sizeof(char));
	/*if( recv(sd, buff, sizeof(buff), 0 ) < 0){
		perror("recv");
		exit(5);
	}
	get(buff);*/
	
	while( recv(sd, buff, N*sizeof(char), 0 ) > 0){
		printf("%s\n",buff);
		get(buff);	
	}
 	close(sd_server); 
	close(sd_client); 
	sd_server=0;
	sd_client=0;
		
	
}
void mput_(char *param){
	FILE *fd;
	char buffer[100],filename[200];
	memset(buffer,'\0',100);
	//chdir(paraam);
	//printf("%s\n",param);
	sprintf(buffer,"%s","ls ");
	strcat(buffer,param);
	//system(buffer);
	fd = popen(buffer,"r");
	if(fd<0)
		perror("System Command");
	else
		printf("File Opened\n");
	while(fgets(filename, sizeof(filename), fd)!=NULL)
		{
		 //fscanf(fd, "%[^\n]", filename);
		 printf("1.%s\n",filename);
		 //if(filename[strlen(filename)]==':')
		 //	continue;
		 //else	
		 put(filename);
		}
		fclose(fd);
}		
void rget(char *param){
	char port[N]; 
	char stor[N]; 
	char buff[N],filename[100];
	int min_port = 1025;
	int max_port = 65535;
	struct sockaddr_in serv_addr, cl_addr;
	int sd_client, sd_server, len, nb=1, i;
	struct hostent *host;
	FILE *fd;
	if( 0 > (sd_server = socket(AF_INET, SOCK_STREAM, 0))){

		perror("socket");
		exit(1);
	}

	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr  = INADDR_ANY;
	memset(&(serv_addr.sin_zero), '\0', 8);

        len = sizeof(cl_addr);

	/*for( i = min_port; i <= max_port; i++){

		serv_addr.sin_port = htons(i);

        	if(0 <= bind(sd_server, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) ) 
			break;
	}*/
	
	memset(buff, '\0', N*sizeof(char));
	memset(port, '\0', N*sizeof(char));
	sprintf(buff, "%d", serv_addr.sin_port);
	printf("PUT %s %d\n", buff, ntohs(serv_addr.sin_port));
	strcat(port,"RGET ");
 	//strcat(port, buff);
	//strcat(port,"");
	strcat(port,param);
	
	if( send(sd, port, strlen(port)+1, 0) < 0 ){

		perror("send");
		exit(2);
	}
	printf("%s",port);
	char dir1[200],dir2[200];
	memset(buff, '\0', N*sizeof(char));
	while( nb>0){
		if((nb = recv(sd,buff,strlen(buff),0))<0){
			perror("error in receiving") ;
			exit;
		}
		printf("%s\n",buff);
		if(buff[strlen(buff)-1]==':'){
			memcpy(dir,buff,strlen(buff)-2);	
			strcpy(dir2,"mkdir ");
			strcat(dir2,dir1);
			system(dir2);
		}
		else
			get(buff);
	
	}
	fclose(fd);
 	close(sd_server); 
	close(sd_client); 
	sd_server=0;
	sd_client=0;
}
void rput(char *param){
	FILE *fd;
	char buffer[100],filename[100];
	memset(buffer,'\0',100);
	//chdir(param);
	sprintf(buffer,"ls -R ");
	strcat(buffer,param);
	//system(buffer);
	fd = popen(buffer,"r");
	while(fgets(filename,100,fd)!=NULL)
		{
		 
		 if(filename[strlen(filename)-1]==':'){
		 	printf("%s",filename);
		 	continue;
		 }	
		 else{	
		 	//chdir(param);
		 	memset(buffer,'\0',100);
		 	strcat(buffer,"raj/");
		 	strcat(buffer,filename);
		 	put(buffer);
		 }	
		}
	fclose(fd);

}
