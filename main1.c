#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 21
#define MAX_BUFFER_SIZE 1024

int main() {
  int serverSocket, clientSocket;
  struct sockaddr_in serverAddress, clientAddress;
  char buffer[MAX_BUFFER_SIZE];

  // Create server socket
  serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket < 0) {
    perror("Error creating server socket");
    exit(1);
  }

  // Set server address
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(PORT);

  // Bind server socket to the specified address and port
  if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
    perror("Error binding server socket");
    exit(1);
  }

  // Listen for incoming connections
  if (listen(serverSocket, 5) < 0) {
    perror("Error listening for connections");
    exit(1);
  }

  printf("FTP server started on port %d\n", PORT);

  while (1) {
    socklen_t clientAddressLength = sizeof(clientAddress);

    // Accept incoming connection
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
      perror("Error accepting connection");
      exit(1);
    }

    printf("New client connected\n");

    // Authenticate client
    send(clientSocket, "220 Welcome to the FTP server\n", strlen("220 Welcome to the FTP server\n"), 0);
    recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
    if (strcmp(buffer, "USER admin\n") == 0) {
      send(clientSocket, "331 Password required for admin\n", strlen("331 Password required for admin\n"), 0);
      recv(clientSocket, buffer, MAX_BUFFER_SIZE, 0);
      if (strcmp(buffer, "PASS 1234\n") == 0) {
        send(clientSocket, "230 User logged in\n", strlen("230 User logged in\n"), 0);
        // Handle FTP commands from client
        // TODO: Implement FTP command handling
      } else {
        send(clientSocket, "530 Login incorrect\n", strlen("530 Login incorrect\n"), 0);
      }
    } else {
      send(clientSocket, "530 Login incorrect\n", strlen("530 Login incorrect\n"), 0);
    }

    // Close client socket
    close(clientSocket);
    printf("Client disconnected\n");
  }

  // Close server socket
  close(serverSocket);

  return 0;
}