#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include<pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include<sys/stat.h>
#include<time.h>
#define PORT 3555
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[])
{
   int sock, length, n, i;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];
   char buffer[256],message[256];
   char spcl_buff[4];
   struct stat tsp;

   if (argc < 1) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(PORT);
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   while (1) {
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
	       if (n < 0) error("recvfrom");	       
       write(1,"Received a datagram: ",21);
       bzero(buffer,256); 
       short int connection_id,connection_idf,type,status;
	memcpy((void*)&connection_id,(void*)buf,2);
	connection_id = ntohs(connection_id); 
	memcpy((void*)&type,(void*)(buf+2),2);
	type = ntohs(type); 
	memcpy((void*)&status,(void*)(buf+4),2);
	status = ntohs(status); 
	memcpy((void*)buffer,(void*)(buf+6),256);//strlen(buf)-6);
//	connection_id = ntohs(connection_id); 
	
	
 	write(1,buffer,256);
  	//printf("%d",connection_id);					
  	//printf("%d\nrajas\n",connection_id); 
  	if((connection_id == -1)&&(type==0))
  	{	
  		connection_idf = htons(1);
  		status = htons(0);
  		type = htons(1);
  		bzero(&message,256);
  		bzero(&buf,1024);
  		memcpy((void*)buf, (void *)&connection_idf, sizeof(short));
   		memcpy((void*)(buf+2), (void *)&type, sizeof( short)); 
   		memcpy((void *)(buf+4), (void *)&status, sizeof(short));
   		memcpy((void *)(buf+6), (void*)message, 256);
   
  	       	n = sendto(sock,buf,sizeof(buf),0,(const struct sockaddr *)&from,fromlen);
	        	if (n < 0) error("Error in Sending Ack");
	       } 	
       bzero(&tsp,sizeof(tsp));
//       bzero(&message,256);
       if(stat(buffer,&tsp)!=0){
       		status = htons(1);
       		strcpy(message,"error ");
       		//error("File not exist\n");
       		
       		}
       else {
           status = htons(0);		
           strcpy(message,ctime(&tsp.st_atime));
           }
       short int type1 = htons(2);

       bzero(&buf,1024);
//       strcpy(message,ctime(&t
       connection_idf = htons(1);
       memcpy((void*)buf, (void *)&connection_idf, sizeof(short));
       memcpy((void*)(buf+2), (void *)&type1, sizeof( short)); 
       memcpy((void *)(buf+4), (void *)&status, sizeof(short));
       memcpy((void *)(buf+6), (void*)message, 256);//strlen(message));
       //puts(message);
       //write(1,"\n",1);
       n = sendto(sock,buf,sizeof(buf),0,(const struct sockaddr *)&from,fromlen);	 
                if (n  < 0) error("sendto");
                
       bzero(buf,1024);
       connection_idf = ntohs(connection_idf);
       n=recvfrom(sock,buf,1024,0,(struct sockaddr *)&from, &fromlen);
       		if(n<0) error("Recieving Ack\n");
        bzero(&buffer,256); 
        memcpy((void*)&connection_id,(void*)buf,2);
	connection_id = ntohs(connection_id); 
	memcpy((void*)&type,(void*)(buf+2),2);
	type = ntohs(type); 
	memcpy((void*)&status,(void*)(buf+4),2);
	status = ntohs(status); 
	memcpy((void*)buffer,(void*)(buf+6),256); 
	//printf("%d\t%d\t%dsajdkad",type,status,connection_id);      		
       if((connection_id == connection_idf)&&(type==3)){
       		write(1,"\nRecieved an DONE_ACK from Client: ",35);
                //write(1,buffer,256);		 
   	}
	}
   return 0;
 }

