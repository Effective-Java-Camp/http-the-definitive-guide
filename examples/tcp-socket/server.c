#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1460
#define CLIENT_SIZE 1

void error_handling(char *msg);

int main(int argc, char *argv[]) {
  
  int server;               // Server socket
  int client;               // Client socket
  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size;
  
  char buff[BUFFER_SIZE];   // Buffer
  FILE * readFile;
  int file_size = 0;
  int recv_size = 0;
  int cur_size = 0;
  
  int index = 0;
  
  // Usage
  if (argc!=2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }
  
  // Make socket
  server = socket(PF_INET, SOCK_STREAM, 0); // IPv4, TCP
  if (server == -1)
    error_handling("socket() Error");
  
  // Init
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));
  
  // Bind
  if (bind(server, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
    error_handling("bind() Error");
  
  // Listen
  if (listen(server, CLIENT_SIZE) == -1)
    error_handling("listen() Error");
  printf("Server start!\n");
  
  // Accept
  clnt_addr_size = sizeof(clnt_addr);
  client = accept(server, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
  if (client == -1)
    error_handling("accept() Error");
  else
    printf("Connected Client!\n");
  
  // Open target
  readFile = fopen("source.mp4", "rb");
  if (readFile == NULL) {
    fputs("File Error", stderr);
    exit(1);
  }
  
  // Send target size to client
  fseek(readFile, 0, SEEK_END);
  file_size = ftell(readFile);
  fseek(readFile, 0, SEEK_SET);
  printf("Total File Size : %d\n", file_size);
  
  write(client, (void *)&file_size, BUFFER_SIZE);
  printf("Sending File Size to Client!\n");
  
  FILE * fp = fopen("source.mp4", "rb");
  // Send target data to client
  while(!feof(fp)) {
    recv_size = fread(buff, 1, BUFFER_SIZE, fp);
    cur_size += recv_size;
    send(client, buff, recv_size, 0);
    printf("[%d] : %d / %d\n", ++index, cur_size, file_size);
  }
  
  // Close
  fclose(readFile);
  close(client);
  close(server);
  printf("Done! : %d / %d\n", cur_size, file_size);
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}