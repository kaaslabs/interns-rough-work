# Group Chat Application

A multi-client group chat application in C using epoll and multi-threading for efficient I/O multiplexing and message broadcasting.

## Features

- Server uses **epoll** for efficient I/O multiplexing
- **Multi-threaded** message broadcasting to prevent blocking
- Support for up to 100 concurrent clients
- Real-time text-based group chat
- User join/leave notifications
- Display active users with `/users` command
- Clean disconnect with `/quit` command

## Architecture

### Server (server.c)
- Uses `epoll` to monitor multiple client sockets simultaneously
- Non-blocking I/O for all client connections
- Spawns separate threads for broadcasting messages to avoid blocking the main event loop
- Thread-safe client management with mutex locks
- Edge-triggered epoll for better performance

### Client (client.c)
- Two-threaded design:
  - One thread for receiving messages from the server
  - One thread for sending user input to the server
- Non-blocking communication with the server

## Compilation

### Compile Server
```bash
gcc -o server server.c -lpthread
```

### Compile Client
```bash
gcc -o client client.c -lpthread
```

## Usage

### Start the Server
```bash
./server <port>
```

Example:
```bash
./server 8080
```

The server will start listening on `0.0.0.0:<port>` (all network interfaces).

### Connect Clients
```bash
./client <server-IP> <port>
```

Example (connecting to localhost):
```bash
./client 127.0.0.1 8080
```

Example (connecting to remote server):
```bash
./client 192.168.1.100 8080
```

When prompted, enter your name (2-31 characters).

## Commands

- `/users` - Display all active users in the chat
- `/quit` - Exit the chat
- Type any message and press Enter to send it to all users

## Example Session

### Terminal 1 (Server)
```
$ ./server 8080
=== GROUP CHAT SERVER (EPOLL + MULTI-THREADING) ===
Server started on 0.0.0.0:8080
Waiting for connections...
Client Alice connected from 127.0.0.1:54321
<<< Alice has joined the chat >>>
Client Bob connected from 127.0.0.1:54322
<<< Bob has joined the chat >>>
[Alice]: Hello everyone!
[Bob]: Hi Alice!
<<< Alice has left the chat >>>
```

### Terminal 2 (Client 1 - Alice)
```
$ ./client 127.0.0.1 8080
Enter your name: Alice
=== WELCOME TO THE GROUP CHAT ===
Type /users to see active users
Type /quit to exit
<<< Alice has joined the chat >>>
<<< Active users: Alice >>>
> <<< Bob has joined the chat >>>
Hello everyone!
> [Bob]: Hi Alice!
/quit
Goodbye!
```

### Terminal 3 (Client 2 - Bob)
```
$ ./client 127.0.0.1 8080
Enter your name: Bob
=== WELCOME TO THE GROUP CHAT ===
Type /users to see active users
Type /quit to exit
<<< Bob has joined the chat >>>
<<< Active users: Alice, Bob >>>
> [Alice]: Hello everyone!
Hi Alice!
> <<< Alice has left the chat >>>
```

## Technical Details

### Server Implementation
- **epoll**: Monitors all client file descriptors for read events
- **Edge-triggered mode**: More efficient event notification
- **Non-blocking sockets**: Prevents server from blocking on slow clients
- **Thread pool for broadcasting**: Each broadcast spawns a new thread that detaches after completion
- **Mutex protection**: Ensures thread-safe access to the client list

### Client Implementation
- **Dual threads**: Separates input/output for smooth user experience
- **Signal handling**: Graceful shutdown on Ctrl+C

## Limitations

- Maximum 100 concurrent clients (configurable via MAX_CLIENTS)
- Message size limited to 2048 bytes (configurable via BUFFER_SIZE)
- Name length limited to 32 characters (configurable via NAME_LEN)
- No message persistence (messages are not stored)
- No authentication or encryption

## Notes

- The server binds to `0.0.0.0` by default, making it accessible from any network interface
- For local testing, use `127.0.0.1` as the client IP
- Ensure the port is not blocked by a firewall
- The application uses TCP sockets for reliable message delivery
