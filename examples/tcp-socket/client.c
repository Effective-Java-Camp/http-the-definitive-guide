#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1460
void error_handling(char *msg);

int main(int argc, char *argv[]) {
  
  int sock;
  struct sockaddr_in serv_addr;
  
  char hsv[] = "Hello Server";
  
  char buff[BUFFER_SIZE];
  FILE * writeFile;
  int file_size = 0;
  int packet_size = 0;
  int recv_size = 0;;
  int cur_size = 0;
  
  int index = 0;
  
  // Usage
  if (argc!=3) {
    printf("Usage : %s <IP> <PORT>\n", argv[0]);
    exit(1);
  }
  
  // Make socket
  sock = socket(PF_INET, SOCK_STREAM, 0); // IPv4, TCP
  if (sock == -1)
    error_handling("socket() Error");
  
  // Init
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));
  
  // Connect server
  if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
    error_handling("connect() error");
  else
    puts("Connected...");
  
  // Send 'Hello Server' to server
  write(sock, hsv, strlen(hsv));
  
  // Receive a size of file from server
  do {
    packet_size = read(sock, &file_size, BUFFER_SIZE);
  } while (packet_size < 0);
  
  printf("Packet received.\n");
  printf("Packet size: %d\n", packet_size);
  printf("File size : %d\n", file_size);
  
  // Open target
  writeFile = fopen("received.mp4", "wb");
  
  // Read and Write
  while (recv_size = recv(sock, buff, BUFFER_SIZE, 0) != 0) {
    fwrite(buff, 1, recv_size, writeFile);
    cur_size += recv_size;
  }
   
  // Close
  fclose(writeFile);
  close(sock);
  
  // Compare size
  if (file_size == cur_size)
    printf("File Successfully Received! : %d / %d\n", cur_size, file_size);
  else
    printf("Failed Received... : %d / %d\n", cur_size, file_size);
  
  return 0;
}

void error_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}