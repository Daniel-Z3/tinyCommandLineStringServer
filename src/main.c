#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

int main(void){

  char* MYPORT="19008";//Randomly chosen unassigned port
  int BACKLOG=5;
  struct addrinfo hints,          //address family socket type 
                  *res,           //Output for getaddrinfo
                  *p;             //Pointer for traversing res
 struct sockaddr_storage clientaddr;
  int status;                     //getaddrinfo() status
  int sockfd, clientsockfd;       //Socket descriptor
  socklen_t addrsize;
  char* msg="Hullooo";

  memset(&hints,0,sizeof hints);
    
  hints.ai_family=AF_UNSPEC;      //Maximum compatibility
  hints.ai_socktype=SOCK_STREAM;  //Using TCP for image transfer
  hints.ai_flags=AI_PASSIVE;

  //Filling in addrinfo 
  status=getaddrinfo(NULL,MYPORT,&hints, &res); 
  
  //checking port validity
  if(status!=0){
    //Add error logging
    printf("getaddrinfo Error: %s\n",gai_strerror(status));
    return 1;
  }
  //setting up socket
  sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if(sockfd==-1){
    //Add error logging
    printf("socket Error: %s\n", strerror(errno));
    return 2;
  }
  bind(sockfd,res->ai_addr,res->ai_addrlen);
  listen(sockfd,BACKLOG);
  
  addrsize=sizeof clientaddr;
  clientsockfd=accept(sockfd,(struct sockaddr *)&clientaddr, &addrsize);
  if(clientsockfd==-1){
    printf("Accept Error: %s\n",strerror(errno));
  }

  //Sending test message
  send(clientsockfd,msg,strlen(msg),0);

  close(clientsockfd);
  close(sockfd);
  return 0;
}
