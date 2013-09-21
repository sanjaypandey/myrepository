#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream> 
#include <cmath>
#include <algorithm>
#include <pthread.h>


#define N 5
using namespace std;
pthread_t tid[N];

void error(char *);

void* handler(void *arg)
{
     char buffer[256];
     int n;
     int newsockfd=((int )arg); 
     printf("\nhandler function of thread is called..newfd:%d\n",
             newsockfd);
     bool stop=true;
    while(stop)
    {
       n=read(newsockfd,buffer,sizeof(buffer));
       if(n<0)
        {
 	    error("\nsome error in \n");
	   return (void *)1;
         }
       if(buffer[0]=='z')
          {
            stop=false;
	}
       printf("\nread message:%s\n",buffer);
       bzero(buffer,sizeof(buffer));
       fgets(buffer,255,stdin);
       n=write(newsockfd,buffer,strlen(buffer));
         if(n<0)
	{
          error("\nsome problem in writing..\n");
	 return (void *)1;
	}     

    }
     return (void *)0;
}

void error(char *msg)
{
   printf("\nsome error in reading the port.:%s\n",msg);
   //exit(1);
}

int main(int argc, char* argv[])
{
 //  printf("\nserver programm is running ..\n");
   static int i=0;  
    int sockfd,newsockfd,portno,clilen,n;
    char buffer[256];
    struct sockaddr_in server_addr,client_addr;
    if(argc<2)
  {
     error("Not sufficient argument given to the server..");
     return 1;
  }
   
   sockfd=socket(AF_INET,SOCK_STREAM,0);
   if(sockfd<0)
 {
   error("some error in openning the socket");
   return 1;
 }
   portno=atoi(argv[1]);
   bzero((char *)&server_addr,sizeof(server_addr));
   server_addr.sin_family=AF_INET;
   server_addr.sin_addr.s_addr=INADDR_ANY;
   server_addr.sin_port=htons(portno); 
   if(bind(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
  {
    error("some error in binding the socket to the internet address.");
    return 1;
  }
  listen(sockfd,5);
  bool stop=true;
  while(stop){
  clilen=sizeof(client_addr);
  newsockfd=accept(sockfd,(struct sockaddr *)&(client_addr),
   (socklen_t *)&clilen);
  pthread_create(&tid[i++],NULL,handler,(void *)newsockfd);
 // pthread_join(tid[i++],NULL);
  if(newsockfd<0)
  {
    error("some problem in accepting the new client.");
    return 1;
  } 
 
 /*
  bzero(buffer,sizeof(buffer));
  n=read(newsockfd,buffer,sizeof(buffer));
  if(n<0)
  {	
     error("some error in reading the client.");
     return 1;
  }
   if(buffer[0]=='z') stop=false;
   printf("\nhere is the message:%s\n",buffer);
   bzero(buffer,sizeof(buffer));
   fgets(buffer,sizeof(buffer)-1,stdin);
   n=write(newsockfd,buffer,sizeof(buffer));
   if(n<0)
   {
     printf("\nsome problem in sending the message to particular client     .\n");
   return 1;
   }
*/   

  } 
  close(sockfd); 
  return 0;
}
