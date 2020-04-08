// 3.1.12
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#define PORT 8080
#define NUM_CLIENTS 2
#define SERVER 0
#define CLIENT 1
#define SEND 0
#define RECV 1

void* client_sender(){
  printf("Client %i online.\n",s);

  char msg[256];
  struct sockaddr_in serv_addr;
  socklen_t addr_len = sizeof(serv_addr);
  int s;
  
  if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) { 
      perror("Client: Socket creation error \n"); 
      pthread_exit(EXIT_FAILURE);
    }

  //set dst socket info
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT); 
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
    printf("Client: Invalid address/ Address not supported\n"); 
    pthread_exit(EXIT_FAILURE);
  }

  sprintf(msg, "Hello from client!");
  printf("Client %i is sending: '%s'\n",s,msg);
  if(sendto(s, msg, strlen(msg) , 0,(struct sockaddr*)&serv_addr,addr_len)<0) {
    printf("Client %i error in sendto.\n",s);
    perror("*");
    return EXIT_FAILURE;
  }

  printf("Client %i done!\n",s);
  pthread_exit(EXIT_SUCCESS);
}

void* client_maker(){ 
  printf("client_maker started.\n");

  //make a client n times
  pthread_t c[NUM_CLIENTS];
  
  for(int i = 0; i < NUM_CLIENTS; i++){
    //can't make the socket here, it causes a race condition, not sure why
    //makes the client sender use the same socket
    printf("Making a client to send some data.\n");
    pthread_create(&c[i],NULL,client_sender,NULL);
  }
  for(int i = 0; i < NUM_CLIENTS; i++){
    pthread_join(c[i]);
  }

  printf("All clients have successfully exited!\n");
  pthread_exit(EXIT_SUCCESS);
}


void *server_listener() {
  int sd, sock, msg_rscvd;
  struct sockaddr_in addr;
  int addrlen = sizeof(addr);
  int buf_size = 1024;
  char buf[buf_size];
  struct sockaddr_in from;
  char from_addr[INET_ADDRSTRLEN];

  printf("server_listener started.\n");  
  
  // Creating socket file descriptor
  if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) { 
    perror("Server: Error in socket"); 
    exit(EXIT_FAILURE);
  }
  
  addr.sin_family = AF_INET; 
  addr.sin_addr.s_addr = INADDR_ANY; //INADDR_LOOPBACK doesn't work??
  addr.sin_port = htons(PORT);

  //binding socket
  if (bind(sd, (struct sockaddr *) &addr, addrlen)<0) { 
    perror("Server: Error in bind"); 
    exit(EXIT_FAILURE); 
  }

  //wait for incoming connection
  while(1) {
    printf("listening...\n");
    memset((void*)buf, 0x00, buf_size);
    if(recvfrom(sd, (void*)buf, buf_size,0,(struct sockaddr*)&from,&addrlen)<0) {
      perror("Error in recvfrom.");
      return EXIT_FAILURE;
    }
    inet_ntop(AF_INET, &from.sin_addr,from_addr, INET_ADDRSTRLEN);
    short port = ntohs(from.sin_port);
    printf("Received message from %s:%i: '%s'\n",from_addr,port,buf);
  }
  
  pthread_exit(EXIT_SUCCESS);
}


int main(int argc, char const *argv[]) {
  pthread_t s,c;
  pthread_create(&s,NULL, server_listener,NULL);
  pthread_create(&c,NULL, client_maker,NULL);
  pthread_join(s,NULL);
  pthread_join(c,NULL);
  return EXIT_SUCCESS;
}
