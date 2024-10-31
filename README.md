# Linux-Client-Server
This project is a basic C++ client-server application, demonstrating socket communication. The server listens on a specified port for incoming client connections, and clients can connect, send messages periodically, and log interactions to a file.

### Features
- Server: Accepts multiple client connections and logs messages from clients.
- Client: Sends a specified message periodically to the server with a user-defined interval.
- Multi-threaded: The server handles each client connection in a separate thread.
- File Logging: Logs all received client messages to log.txt on the server side.

### Usage
Compile the project using g++ or any C++ compiler that supports C++11 or later.

### Server
Run the server on a specified port:
```bash
./app <port> server
```

### Example:
```bash
./app 3000 server
```

### Client
Run the client with port, name, and message interval:
```bash
./app <port> client <name> <period>
```

Example:
```bash
./app 3000 client "name" 5
```

In this example, the client will connect to the server on port 3000, use "ClientName" as its identifier, and send a message every 5 seconds.

### Code Structure
- Socket class: Abstract base class managing basic socket setup and configuration.
- TCPServer class: Handles incoming connections and logs client messages.
- TCPClient class: Sends periodic messages to the server with a custom name and interval.

### Requirements
- A Unix-like OS for socket programming (Linux or macOS).
- C++11 or later.

### License
This project is licensed under the MIT License.
