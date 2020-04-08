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
#define IPV4LEN 16
#define HOSTNAME "midgard"
#define LOOKUP "localhost"

//changes hostname temporarily
int change_hostname(char *newhn) {
  int buflen = 256;
  //gethostname
  char hn[buflen];
  if(gethostname(hn,buflen)<0) { perror("Error in gethostname."); }
  char prevhn[strlen(hn)];
  strcpy(prevhn,hn);
  printf("gethostname: %s\n",hn);

  //sethostname
  printf("changing host name to %s...\n",newhn);
  if(sethostname(newhn,strlen(newhn))<0) { perror("Error in sethostname."); }
  if(gethostname(hn,buflen)<0) { perror("Error in gethostname."); }
  printf("gethostname now: %s\n",hn);
  printf("Changing back to %s...\n",prevhn);
  if(sethostname(prevhn,strlen(prevhn))<0) { perror("Error in sethostname."); }
  if(gethostname(hn,buflen)<0) { perror("Error in gethostname."); }
  printf("gethostname now: %s\n",hn);

  return EXIT_SUCCESS;
}

//runs getaddrinfo for argument and reutrns the IP addresses in a char*
int get_info(char* lookup) {  
  struct addrinfo hints, *infoptr;
  int buflen = 256;
  char buf[buflen];
  int err;
  struct addrinfo *temp;

  printf("memset\n");
  memset(&hints, 0, sizeof(struct addrinfo));
  printf("memset\n");
  hints.ai_family = AF_INET;
  if(err = getaddrinfo(lookup, NULL, &hints, &infoptr)) {
    fprintf(stderr,"gai_strerror: %s\n",gai_strerror(err));
    perror("Error in getaddrinfo");
    return EXIT_FAILURE;
  }
  printf("getaddrinfo success.\n");
  printf("infoptr: %p\n",infoptr);
  for(temp = infoptr; temp != NULL; temp = temp->ai_next) {
    getnameinfo(temp->ai_addr, temp->ai_addrlen, buf, buflen, NULL, 0, NI_NUMERICHOST);
    printf("getaddrinfo-getnameinfo: %s\n",buf);
  }

  return EXIT_SUCCESS;
}

// This is a wrapper to handle receiving and sending text messages.
//  it incorporates print statements to announce what the sender and receiver is
//  doing to make debugging easier.
int sr_wrapper(char *msg, char cs, char sr, int s) {
  char *name = (cs==SERVER) ? "Server" : "Client";
    int buf_size = 1024;
    char buf[buf_size];

  if(sr==SEND){
    printf("%s %i is sending: '%s'\n", name,s,msg);
    if(send(s, msg, strlen(msg) , 0)<0) {
      printf("%s %i error in send.\n",name,s);
      perror("*");
      return EXIT_FAILURE;
    }
  }
  else if (sr==RECV) {
    if(recv(s, (void*)buf, buf_size,0)<0) {
      printf("%s %i error in recv.\n",name,s);
      perror("*");
      return EXIT_FAILURE;
    }
    printf("%s %i received message: '%s'\n",name,s,buf);
  }
  else {
    fprintf(stderr,"Error in wrapper.");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}



void* client_sender(void *sock){
  int s = *((int*)(sock));
  char msg[256];
  printf("Client %i online.\n",s);

  sprintf(msg,"This is send() from client!");
  sr_wrapper(msg,CLIENT,SEND,s);
  //sleep(1);
  sprintf(msg, "Change hostname to: %s", HOSTNAME);
  sr_wrapper(msg,CLIENT,SEND,s);
  sprintf(msg, "Looking up hostname: %s", LOOKUP);
  sr_wrapper(msg,CLIENT,SEND,s);

  printf("Client %i done!\n",s);
  pthread_exit(EXIT_SUCCESS);
}

void* client_maker(){
  printf("client_maker started.\n");
 
  int sock;
  struct sockaddr_in serv_addr;
  //make a client n times
  pthread_t c[NUM_CLIENTS];
   
  for(int i = 0; i < NUM_CLIENTS; i++){
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
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

    while(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
      printf("Client: Connection Failed, trying again...\n");
      sleep(1);
    }
    printf("Connection Success!\n");
    printf("Making a client to send some data.\n");
    pthread_create(&c[i],NULL,client_sender,(void*)&sock);
  }
  for(int i = 0; i < NUM_CLIENTS; i++){
    printf("waiting for clients to exit...\n"); //without this replit crashes??
    pthread_join(c[i]);
  }
  printf("All clients have successfully exited!\n");
  pthread_exit(EXIT_SUCCESS);
}

void *server_sender(void *sock){
  int s = *((int*)(sock));

  printf("Server %i online.\n",s);
  sr_wrapper(NULL, SERVER, RECV, s); //intro
  sr_wrapper(NULL, SERVER, RECV, s); //hostname
  change_hostname(HOSTNAME);
  sr_wrapper(NULL, SERVER, RECV, s); //get_info
  get_info(LOOKUP);
  
  printf("Server %i done!\n",s);
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
  //these options aren't really needed but just for POC
  // SO_REUSEPORT would really only be useful if I had multiple
  // server threads binded to the same port
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
  while(1) {
    if ((sock = accept(sd, (struct sockaddr *)&addr, (socklen_t*)&addrlen))<0){ 
      perror("Server: Error in accept"); 
      exit(EXIT_FAILURE); 
    }
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
