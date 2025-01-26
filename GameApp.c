#include "GameApp.h"

server_t server;

int connectServer(int argc, char **argv)
{
  server.ConnectSocket = INVALID_SOCKET;
  server.result = NULL;

  // Validate the parameters
  if (argc != 2) {
    printf("Usage: %s <server-name>\n", argv[0]);
    return 0;
  }

  // Initialize Winsock
  if ((server.iResult = WSAStartup(MAKEWORD(2, 2), &server.wsaData)) != 0) {
    printf("WSAStartup failed with error: %d\n", server.iResult);
    return 0;
  }

  ZeroMemory(&server.hints, sizeof(server.hints));
  server.hints.ai_family = AF_UNSPEC;
  server.hints.ai_socktype = SOCK_STREAM;
  server.hints.ai_protocol = IPPROTO_TCP;

  // Resolve the server address and port
  if ((server.iResult = getaddrinfo(argv[1], DEFAULT_PORT, &server.hints, &server.result)) != 0) {
    printf("getaddrinfo failed with error: %d\n", server.iResult);
    WSACleanup();
    freeaddrinfo(server.result);
    return 0;
  }

    // Create a socket for connecting to the server
  server.ConnectSocket = socket(server.result->ai_family, server.result->ai_socktype, server.result->ai_protocol);
  if (server.ConnectSocket == INVALID_SOCKET) {
    printf("Socket creation failed with error: %d\n", WSAGetLastError());
    WSACleanup();
    freeaddrinfo(server.result);
    return 0;
  }

  // Connect to server
  server.iResult = connect(server.ConnectSocket, server.result->ai_addr, (int)server.result->ai_addrlen);
  if (server.iResult == SOCKET_ERROR) {
    printf("Unable to connect to server.\n");
    closesocket(server.ConnectSocket);
    server.ConnectSocket = INVALID_SOCKET;
    WSACleanup();
    freeaddrinfo(server.result);
    return 0;
  }

  freeaddrinfo(server.result);

  if (server.ConnectSocket == INVALID_SOCKET) {
    printf("Failed to connect to server.\n");
    WSACleanup();
    return 0;
  }

  enum Request request = echores;
  enum Request response;

  // Send test packet
  server.iResult = send(server.ConnectSocket, (char *)(request), sizeof(request), 0);
  if (server.iResult == SOCKET_ERROR) {
    printf("Send failed with error: %d\n", WSAGetLastError());
    closesocket(server.ConnectSocket);
    freeaddrinfo(server.result);
    WSACleanup();
    return 0;
  }


  // Receive 
  server.iResult = recv(server.ConnectSocket, (char *)&response, sizeof(response), 0);
  if (server.iResult <= 0) {
    printf("Connection closed by server or error occurred.\n");
    closesocket(server.ConnectSocket);
    freeaddrinfo(server.result);
    WSACleanup();
    return 0;
  }

  return 1;
}

int __cdecl main(int argc, char **argv) 
{
  int isConnected = 0;
  printf("Connecting to server..\n");
  do  
  {
    isConnected = connectServer(argc, argv);
  } while (isConnected);
  printf("Connected to server.\n");

}

void gameLoop() {

}