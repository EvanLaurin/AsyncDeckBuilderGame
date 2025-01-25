int __cdecl main(int argc, char **argv)


  WSADATA wsaData;
  SOCKET ListenSocket = INVALID_SOCKET, ClientSocket = INVALID_SOCKET;
  struct addrinfo *result = NULL, hints;
  enum Request request;
  enum Response response;
  int data, iResult;

  // Initialize Winsock
  if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
  {
    printf("WSAStartup failed with error: %d\n", iResult);
    goto cleanup;
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // Resolve server address and port
  if ((iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result)) != 0)
  {
    printf("getaddrinfo failed with error: %d\n", iResult);
    goto cleanup_wsa;
  }

  // Create socket for resolving connections
  ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (ListenSocket == INVALID_SOCKET)
  {
    printf("Socket creation failed with error: %ld\n", WSAGetLastError());
    goto cleanup_addrinfo;
  }

  // Bind socket to port
  if (bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
  {
    printf("Bind failed with error: %d\n", WSAGetLastError());
    goto cleanup_socket;
  }

  // Listen for incoming connections
  if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
  {
    printf("Listen failed with error: %d\n", WSAGetLastError());
    goto cleanup_socket;
  }
  
  printf("Waiting for client connections...\n");

  // Create new socket for communicating with client
  ClientSocket = accept(ListenSocket, NULL, NULL);
  if (ClientSocket == INVALID_SOCKET)
  {
    printf("Accept failed with error: %d\n", WSAGetLastError());
    goto cleanup_socket;
  }
  printf("Client connected.\n");
  

cleanup_client:
  closesocket(ClientSocket);
cleanup_socket:
  closesocket(ListenSocket);
cleanup_addrinfo:
  freeaddrinfo(result);
cleanup_wsa:
  WSACleanup();
}