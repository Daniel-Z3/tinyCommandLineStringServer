#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void){
  char* SERVER_PORT="19008";
  int MSG_LEN=1024; 

  char buf[MSG_LEN];//Char buf to be cast
  struct addrinfo hints,*p,*res;
  int status;
  int sockfd;
  char svrIPstr[INET_ADDRSTRLEN];
  
  memset(&hints,0,sizeof hints);

  hints.ai_family=AF_INET;
  hints.ai_socktype=SOCK_STREAM;

  status=getaddrinfo("localhost",SERVER_PORT,&hints,&res);
  if(status!=0){
    printf("getaddrinfo Error: %s\n",gai_strerror(status));
    return 1;
  } 
  sockfd=socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if(sockfd==-1){
    printf("socket Error:\n");
    return 1;
  }
  int constatus=connect(sockfd, res->ai_addr,res->ai_addrlen);
  if(constatus==-1){
    printf("connect Error: %s\n",strerror(errno));
    return 1;
  }
  inet_ntop(res->ai_family,&(((struct sockaddr_in *)res->ai_addr)->sin_addr),svrIPstr, sizeof svrIPstr);
  printf("Connected to server on %s\n", svrIPstr);
   
  int recstatus=recv(sockfd,buf,MSG_LEN,0);
  if(recstatus==-1){
    printf("recv Error: %s\n", strerror(errno));
    return 1;
  }
  printf("Message recieved\n");
  
  
  printf("Recieved message: %s\n", buf);
 
  close(sockfd);
  freeaddrinfo(res);
  return 0;
}
