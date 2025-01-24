# AsyncDeckBuilderGame

## Overview
This project is a work in progrees console-based asynchronous multiplayer game implemented in C. Loosly inspired by games like The Bazaar. The game allows multiple players to participate by making moves independently, without requiring real-time interaction.

## Implementation Details
### Server-Side:
- **TCP-based networking** for handling player connections.
- **Thread pool** for concurrent processing of player moves.
- **Task queue** to process moves efficiently.
- **Game state storage** in a file or database for persistence.

### Client-Side:
- Players send moves via TCP sockets.
- Receives updates from the server asynchronously.

## How to Run
1. **Compile the server and client:**
   ```sh
   gcc server.c -o server -lpthread
   gcc client.c -o client
   ```
2. **Start the server:**
   ```sh
   ./server
   ```
3. **Start clients and play:**
   ```sh
   ./client
   ```

## License
This project is licensed under the MIT License.