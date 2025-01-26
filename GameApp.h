#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#ifndef client_h
#define client_h

#define DEFAULT_PORT "27015"
#define DEFAULT_BUFLEN 512

enum Request {
  //TODO: Add request types
  echoreq
};

enum Response {
  //TODO: Add response types
  echores
};

typedef struct Server {
  WSADATA wsaData;
  SOCKET ConnectSocket;
  struct addrinfo *result, hints;
  int iResult;
} server_t;

typedef struct SendPacket {
  enum Request request;
  int data;
} send_packet_t;

#endif