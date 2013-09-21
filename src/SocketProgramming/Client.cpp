#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

void error(char *msg)
{
  printf("\nsome error: %s\n",msg);
}

int main(int argc,char *argv[])
{
  int sockfd,portno,n;
  struct sockaddr_in server_addr;
  struct hostent *server;
  char buffer[256];
  if(argc<3)
 {

   error("not sufficient argument..");
   return 1;
  }
   portno=atoi(argv[2]);
   sockfd=socket(AF_INET,SOCK_STREAM,0);
   if(sockfd<0)
  {
    error("some problem in socket.");
    return 1;
  }
  server=gethostbyname(argv[1]);
  if(server==NULL)
 {
   error("server is not specified..");
   return 1;
  }
  bzero((char *)&server_addr,sizeof(server_addr));
  server_addr.sin_family=AF_INET;
  bcopy((char *)server->h_addr,(char *)&server_addr.sin_addr.s_addr,
   server->h_length);
   server_addr.sin_port=htons(portno);
   if(connect(sockfd,(struct sockaddr *)&server_addr,
      sizeof(server_addr))<0)
  {
   error("some problem in connection..");
   return 1;
  }
   bool stop=true;
   while(stop){
   printf("\nplease enter the message..\n");
   bzero(buffer,sizeof(buffer));
   fgets(buffer,sizeof(buffer)-1,stdin);
   n=write(sockfd,buffer,strlen(buffer));
   if(n<0)
 {
    error("some error in writing to the port..");
 }
  bzero(buffer,sizeof(buffer));
  n=read(sockfd,buffer,sizeof(buffer));
  if(n<0)
 {
   error("\nsome problem in reading the file..\n");
  return 1;
 }
  printf("\nreceived the meggage fron server:%s\n",buffer);
  
 }
  return 0;
}
