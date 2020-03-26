// 3.1.12
//#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
//#include <string.h> 
#define PORT 8080

int server() {
	int sd, new_socket, msg_rscvd;
	struct sockaddr_in addr;
	int addrlen = sizeof(addr);
	//char *hello = "Hello from server";
	
	// Creating socket file descriptor
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
    perror("Error in socket"); 
    exit(EXIT_FAILURE);
	}

  //setting sock opts
  int opt = 1;
  int optlen;
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) { 
		perror("Error in setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
  //make sure sock opts were set appropriately
  int buf_size, buf_size_size;
  if (getsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, &optlen)) { 
    perror("Error in getsockopt"); 
		exit(EXIT_FAILURE);
  }
  if(opt) {
    printf("SO_REUSEADDR | SO_REUSEPORT successfully set.\n");
  }
  else {
    perror("sockopts not set appropriately."); 
		exit(EXIT_FAILURE);
  }

	addr.sin_family = AF_INET; 
	addr.sin_addr.s_addr = INADDR_ANY; //INADDR_LOOPBACK doesn't work??
	addr.sin_port = htons(PORT);

  //binding socket
	if (bind(sd, (struct sockaddr *) &addr, addrlen)<0) { 
		perror("Error in bind"); 
		exit(EXIT_FAILURE); 
	}

  if (listen(sd, 1) < 0) { 
		perror("Error in listen"); 
		exit(EXIT_FAILURE); 
	} 

  //wait for incoming connection
  while(1){
    if ((new_socket = accept(sd, (struct sockaddr *)&addr, (socklen_t*)&addrlen))<0){ 
      perror("Error in accept"); 
      exit(EXIT_FAILURE); 
    }
    //start receiver thread here
  }
	
	return EXIT_SUCCESS;
}


int main(int argc, char const *argv[]) { 
  server();
  
}
