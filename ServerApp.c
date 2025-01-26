#include "ServerApp.h"

server_t server;

void *handle_client(void *client_socket) {
  SOCKET playerSocket = *(SOCKET *)client_socket;
  char recvBuffer[1024];
  int recvResult;

  printf("Player handler thread started.\n");

  // Receive data from client
  while ((recvResult = recv(playerSocket, recvBuffer, sizeof(recvBuffer), 0)) > 0) {
    
  }

  if (recvResult == 0) {
    printf("Client disconnected.\n");
  } else {
    printf("recv failed with error: %d\n", WSAGetLastError());
  }

  closesocket(playerSocket);
  return NULL;
}

int setupServer (int argc, char **argv) {

}

int __cdecl main(int argc, char **argv) 
{
  server.ListenSocket = INVALID_SOCKET;
  server.result = NULL;
  enum Request request;
  enum Response response;
  int data, iResult;

  ZeroMemory(&server.hints, sizeof(server.hints));
  server.hints.ai_socktype = SOCK_STREAM;
  server.hints.ai_protocol = IPPROTO_TCP;
  server.hints.ai_flags = AI_PASSIVE;


  // Initialize Winsock
  if ((iResult = WSAStartup(MAKEWORD(2, 2), &server.wsaData)) != 0)
  {
    printf("WSAStartup failed with error: %d\n", iResult);
    goto cleanup_wsa;
  }

  // Resolve server address and port
  if ((iResult = getaddrinfo(NULL, DEFAULT_PORT, &server.hints, &server.result)) != 0)
  {
    printf("getaddrinfo failed with error: %d\n", iResult);
    goto cleanup_wsa;
  }

  // Create socket for resolving connections
  server.ListenSocket = socket(server.result->ai_family, server.result->ai_socktype, server.result->ai_protocol);
  if (server.ListenSocket == INVALID_SOCKET)
  {
    printf("Socket creation failed with error: %ld\n", WSAGetLastError());
    goto cleanup_addrinfo;
  }

  // Bind socket to port
  if (bind(server.ListenSocket, server.result->ai_addr, (int)server.result->ai_addrlen) == SOCKET_ERROR)
  {
    printf("Bind failed with error: %d\n", WSAGetLastError());
    goto cleanup_socket;
  }

  // Listen for incoming connections
  if (listen(server.ListenSocket, SOMAXCONN) == SOCKET_ERROR)
  {
    printf("Listen failed with error: %d\n", WSAGetLastError());
    goto cleanup_socket;
  }
  
  printf("Waiting for client connections...\n");

  while(1) {
    server.ClientSocket = accept(server.ListenSocket, NULL, NULL);
    if (server.ClientSocket == INVALID_SOCKET)
    {
      printf("Accept failed with error: %d\n", WSAGetLastError());
      continue;
    }

    printf("Client connected.\n");

    pthread_t playerThread;
    if (pthread_create(&playerThread, NULL, handle_client, &server.ClientSocket) != 0)
    {
      printf("Failed to create client thread.\n");
      pthread_detach(playerThread);
    }
  }
  

cleanup_client:
  closesocket(server.ClientSocket);
cleanup_socket:
  closesocket(server.ListenSocket);
cleanup_addrinfo:
  freeaddrinfo(server.result);
cleanup_wsa:
  WSACleanup();
}