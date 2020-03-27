// 3.1.12
//#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#define PORT 8080
#define NUM_CLIENTS 1
#define SERVER 0
#define CLIENT 1
#define SEND 0
#define RECV 1
#define IP4LEN 16
#define BUFLEN 256

//runs gethostname, getaddrinfo, sethostname and prints the results
int get_info() {  
  int buf_len = IP4LEN;
  char hn[IP4LEN];
  char ip[IP4LEN];
  gethostname(hn,IP4LEN);
  inet_ntop(AF_INET,hn,ip,IP4LEN);
  printf("gethostname: %s\n",ip);
  struct addrinfo hints, *infoptr;
  hints.ai_family = AF_INET;
  if(getaddrinfo("www.google.com", NULL, &hints, &infoptr)) {
    perror("get_info: getaddrinfo failed.");
    return EXIT_FAILURE;
  }
  struct addrinfo *temp;
  char buf[BUFLEN];
  for(temp = infoptr; temp != NULL; temp = temp->ai_next) {
    getnameinfo(temp->ai_addr, temp->ai_addrlen, buf, BUFLEN, NULL, 0, NI_NUMERICHOST);
    printf("getaddrinfo-getnameinfo: %s",buf);
    }

  return EXIT_SUCCESS;
}

// This is a wrapper to handle receiving and sending text messages.
//  it incorporates print statements to announce what the sender and receiver is
//  doing to making debugging easier.
int sr_wrapper(char *msg, char cs, char sr, int s) {
  char *name = (cs==SERVER) ? "Server" : "Client";
  if(sr==SEND){
    printf("%s %i is sending: '%s'\n", name,s,msg);
    if(send(s, msg, strlen(msg) , 0)<0) {
      printf("%s %i error in recv.\n",name,s);
      pthread_exit(EXIT_FAILURE);
    }
  }
  else if (sr==RECV) {
    printf("%s %i received '%s'\n",name,s,msg);
    int buf_size = 1024;
    char buf[buf_size];
    if(recv(s, (void*)buf, buf_size,0)<0) {
      printf("%s %i error in recv.\n",name,s);
      pthread_exit(EXIT_FAILURE);
    }
    printf("%s %i received message: '%s'\n",name,s,buf);
  }
  else {
    perror("Error in announce.");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void* client_sender(void *sock){
  int s = *((int*)(sock));
  printf("Client %i online.\n",s);
  char *msg = "This is send() from client!";
  sr_wrapper(msg,CLIENT,SEND,s);
  pthread_exit(EXIT_SUCCESS);
}

void* client_maker(){
  printf("client_maker started.\n");
  int sock;
  struct sockaddr_in serv_addr;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
    perror("Client: Socket creation error \n"); 
    pthread_exit(EXIT_FAILURE);
  }
  //set dst socket info
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT); 
  if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) { 
    printf("Client: Invalid address/ Address not supported\n"); 
    pthread_exit(EXIT_SUCCESS);
  }
  
  //make a client n times
  for(int i = 0; i < NUM_CLIENTS; i++){
    while(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
      printf("Client: Connection Failed, trying again...\n");
      sleep(1);
    }
    printf("Connection Success!\n");
    printf("Making a client to send some data.\n");
    pthread_t c;
    pthread_create(&c,NULL,client_sender,(void*)&sock);
    pthread_join(c);
  }
  pthread_exit(EXIT_SUCCESS);
}

void *server_sender(void *sock){
  int s = *((int*)(sock));
  printf("Server %i online.\n",s);
  sr_wrapper(NULL, SERVER, RECV, s);
  char ip[IP4LEN];
  get_info();
  pthread_exit(EXIT_SUCCESS);
}


void *server_listener() {
	int sd, sock, msg_rscvd;
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	
  printf("server_listener started.\n");

	// Creating socket file descriptor
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
    perror("Server: Error in socket"); 
    exit(EXIT_FAILURE);
	}

  //setting sock opts
  int opt = 1;
  int optlen;
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
		perror("Server: Error in setsockopt"); 
		exit(EXIT_FAILURE); 
	}
  //make sure sock opts were set appropriately
  int buf_size, buf_size_size;
  if (getsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, &optlen)) { 
    perror("Server: Error in getsockopt"); 
		exit(EXIT_FAILURE);
  }
  if(opt) {
    printf("Server: SO_REUSEADDR | SO_REUSEPORT successfully set.\n");
  }
  else {
    perror("Server: sockopts not set appropriately."); 
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

  if (listen(sd, 1) < 0) { 
		perror("Server: Error in listen"); 
		exit(EXIT_FAILURE); 
	} 

  //wait for incoming connection
  while(1){
    if ((sock = accept(sd, (struct sockaddr *)&addr, (socklen_t*)&addrlen))<0){ 
      perror("Server: Error in accept"); 
      exit(EXIT_FAILURE); 
    }
    //start receiver thread here
    pthread_t* s;
    pthread_create(&s,NULL, server_sender,(void*)&sock);
    printf("Making a server thread to send things.\n");
    pthread_detach(s);
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