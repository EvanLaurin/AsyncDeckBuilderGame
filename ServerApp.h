#ifndef CLIENT_H
#define CLIENT_H

#include <winsock2.h>
#include <pthread.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define DEFAULT_PORT "27015"
#define MAX_CLIENTS 10  

// Forward declarations
typedef struct player player_t;
typedef struct card card_t;

// Request and Response Enums
enum Request {
  echoreq  // Example: Echo Request (TODO: Add more request types)
};

enum Response {
  echores  // Example: Echo Response (TODO: Add more response types)
};

// Card Structure
struct card {
  int id;
  int cost;
  int attack;
  int health;
  int type;
  void (*effect)(player_t *player, player_t *opponent, card_t *card);
  card_t *next;
};

// Player Structure
struct player {
  SOCKET ClientSocket;
  int id;
  int maxHealth;
  card_t **hand;  
};

// Server Structure
typedef struct Server {
  WSADATA wsaData;
  SOCKET ListenSocket;
  SOCKET ClientSocket;
  struct addrinfo *result, hints;
  int iResult;
} server_t;

// Packet Structure
typedef struct SendPacket {
  enum Request request;
  int data;
} send_packet_t;

#endif  // CLIENT_H