#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "defines.h"

#define BUFSIZE 64

int parse_cmd(const unsigned char *buf, char **resp, int *out_len);


void error(char *msg) {
  perror(msg);
  exit(1);
}


int main() {
  int server_fd; /* parent socket */
  int portno; /* port to listen on */
  int clientlen; /* byte size of client's address */
  struct sockaddr_in serveraddr; /* server's addr */
  struct sockaddr_in clientaddr; /* client addr */
  struct hostent *hostp; /* client host info */
  unsigned char buf[BUFSIZE] = {'\0'}; /* message buffer */
  char *hostaddrp; /* dotted decimal host addr string */
  int optval; /* flag value for setsockopt */
  int n; /* message byte size */
  char *value = NULL;

  portno = 8000;

  server_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) 
    error("ERROR opening socket");

  optval = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, 
	     (const void *)&optval , sizeof(int));

  bzero((char *) &serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);
  
  //Bind server_fd with address
  if (bind(server_fd, (struct sockaddr *) &serveraddr, 
	   sizeof(serveraddr)) < 0) 
    error("ERROR on binding");

  printf("Server started\n");
  if (listen(server_fd, 5) < 0) /* allow 5 requests to queue up */ 
    error("ERROR on listen");

  signal(SIGPIPE, SIG_IGN); //Ignore closing of remote socket and handle it through error code
  clientlen = sizeof(clientaddr);
  struct response resp = {
    .header = 0xaa,
    .len_code = 0,
    .footer = 0x55,
  };
  while (1) {
    printf("Waiting for connection\n");
    int client_fd = accept(server_fd, (struct sockaddr *) &clientaddr, &clientlen);
    if (client_fd < 0){ 
      error("ERROR on accept");
    }
    /* 
     * gethostbyaddr: determine who sent the message 
     */
    hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			  sizeof(clientaddr.sin_addr.s_addr), AF_INET);
    if (hostp == NULL)
      error("ERROR on gethostbyaddr");
    hostaddrp = inet_ntoa(clientaddr.sin_addr);
    if (hostaddrp == NULL)
      error("ERROR on inet_ntoa\n");
    printf("server established connection with %s (%s) on client %d\n", 
	   hostp->h_name, hostaddrp, client_fd);
   while(1){ 
      n = recv(client_fd, buf, 5, 0); //len 5: Header 1 byte, len 4 bytes
      unsigned int *rcv_len = (unsigned int*)&buf[1];
      n = recv(client_fd, &buf[5], *rcv_len + 1, 0); //Including Header
      int ret = 0, n = 0;
      ret = parse_cmd(buf, &value, &resp.len_code);
      if(ret < 0){
        resp.len_code = -1;
      }
      n += send(client_fd, (const void*)&resp, sizeof(resp), 0);
      if(n < 0){
        if(value != NULL) free(value);
        close(client_fd);
        printf("ERROR writing to socket: %d\n", n);      
        break;
      }
      if(resp.len_code > 1){
        n = 0;
        n += send(client_fd, (const void*)&resp.header, 1, MSG_NOSIGNAL);
        n += send(client_fd, value, resp.len_code, MSG_NOSIGNAL);
        n += send(client_fd, (const void*)&resp.footer, 1, MSG_NOSIGNAL);
      }
      if(n < 0){
        if(value != NULL) free(value);
        close(client_fd);
        printf("ERROR writing to socket: %d\n", n);      
        break;
      }
      printf("Response sent successfully\n");
    }
  }
}