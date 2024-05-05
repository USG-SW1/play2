#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Read the request from the client
    bytes_read = read(client_socket, buffer, BUFFER_SIZE - 1);
    if (bytes_read < 0) {
        perror("Error reading from socket");
        return;
    }

    // Add null terminator to the end of the request
    buffer[bytes_read] = '\0';

    // Check if it's a GET request for the index page
    if (strstr(buffer, "GET / HTTP/1.1") != NULL) {
        // Send the HTTP response headers
        const char* response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html\r\n\r\n"
                               "<html><body>"
                               "<h1>Hello World!</h1>"
                               "<form action=\"/upload\" method=\"post\" enctype=\"multipart/form-data\">"
                               "<input type=\"file\" name=\"file\">"
                               "<input type=\"submit\" value=\"Upload\">"
                               "</form>"
                               "</body></html>";
        write(client_socket, response, strlen(response));
    } else if (strstr(buffer, "POST /upload HTTP/1.1") != NULL) {
        // Handle file upload
        // TODO: Implement file upload handling logic here
    } else {
        // Send a 404 Not Found response for any other requests
        const char* response = "HTTP/1.1 404 Not Found\r\n"
                               "Content-Type: text/plain\r\n\r\n"
                               "404 Not Found";
        write(client_socket, response, strlen(response));
    }

    // Close the client socket
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length;

    // Create a socket for the server
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up the server address
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(80);

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Error binding socket");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Error listening for connections");
        exit(1);
    }

    printf("Server listening on port 80...\n");

    while (1) {
        // Accept a client connection
        client_address_length = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length);
        if (client_socket < 0) {
            perror("Error accepting connection");
            exit(1);
        }

        // Handle the client request
        handle_request(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}