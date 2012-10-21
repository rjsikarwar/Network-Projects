
/* UDP client in the internet domain */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define PORT 3555
void error(const char *msg)
{
    perror(msg);
    exit(0);
}
int main(int argc, char *argv[])
{
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[256],message[256];
   char buffer2[1024];
   short connection_id = htons(-1);
   short type = htons(0);  /*Type is encoded as Request:0,Request_Ack:1,DONE:2,DONE_ACK:3*/
   short status = htons(0);
  
   if (argc != 3) { printf("Usage: path of file name File name\n");
                    exit(1);
   }
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(PORT);
   length=sizeof(struct sockaddr_in);
   write(1,"Trying ",7);
   write(1,argv[1],strlen(argv[1]));
   write(1,"...\n",4);
   bzero(buffer2,1024);
   int i = 0 , p = strlen(argv[2]);
   char buffer3[sizeof(short)];
   memcpy((void*)buffer2, (void *)&connection_id, sizeof(short));
   memcpy((void*)(buffer2+2), (void *)&type, sizeof( short)); 
   memcpy((void *)(buffer2+4), (void *)&status, sizeof(short));
   memcpy((void *)(buffer2+6), (void*)argv[2], strlen(argv[2]));
   
       n=sendto(sock,buffer2,sizeof(buffer2),0,(const struct sockaddr *)&server,length);
		if (n < 0) error("Sendto");
       bzero(&buffer2,1024);		
       n = recvfrom(sock,buffer2,1024,0,(struct sockaddr *)&from, &length);
		if (n < 0) error("recvfrom");
       bzero(&buffer,256); 

        memcpy((void*)&connection_id,(void*)buffer2,2);
	connection_id = ntohs(connection_id); 
	memcpy((void*)&type,(void*)(buffer2+2),2);
	type = ntohs(type); 
	memcpy((void*)&status,(void*)(buffer2+4),2);
	status = ntohs(status); 
	memcpy((void*)buffer,(void*)(buffer2+6),256);
//	printf("%d",connection_id);
        if(type==1){	 
   		write(1,"Server ",7); write(1,argv[1] ,strlen(argv[1])); write(1,"is querying for the timestamp of ",33);write(1,argv[2],strlen(argv[2]));write(1,"..\n",4);
   		write(1,buffer,256);
   		}
       //bzero(buffer2,1024);
       //bzero(message,256);
       n = recvfrom(sock,buffer2,1024,0,(struct sockaddr *)&from, &length);
   		if (n < 0) error("recvfrom");
       bzero(buffer,256); 
       memcpy((void*)&connection_id,(void*)buffer2,2);
	connection_id = ntohs(connection_id); 
	memcpy((void*)&type,(void*)(buffer2+2),2);
	type = ntohs(type); 
	memcpy((void*)&status,(void*)(buffer2+4),2);
	status = ntohs(status); 
	memcpy((void*)buffer,(void*)(buffer2+6),256);
       
      // printf("%d\n%d\n",status,type);
       if((type==2)&&(status==0)){
       		write(1,"Time Stamp of ",15);
       		write(1,argv[2], strlen(argv[2]));
       		write(1," is ",4);
       		write(1,buffer,256);//sizeof(message));
  		}
       else if((type ==2)&&(status==1))
        	error(message);
  
       type = htons(3);
       status = htons(0);
       connection_id = htons(connection_id);
       //for(i=0;i<256;i++)
      	 // message[i] = 0;
      
       //bzero(&buffer2,1024);
       bzero(&message,256);
          memcpy((void *)buffer2, (void *)&connection_id, sizeof(short));
	  memcpy((void *)(buffer2+2), (void *)&type, sizeof( short)); 
	  memcpy((void *)(buffer2+4), (void *)&status, sizeof(short));
	  memcpy((void *)(buffer2+6), (void*)message, sizeof(message));
          n=sendto(sock,buffer2,sizeof(buffer2),0,(const struct sockaddr *)&server,length);
   	    if(n<0) error("Sending Ack");
     
   close(sock);
   return 0;
}


