#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory>

enum SocketErrorStatus {
    SOCKET_ERROR = -1,
    BIND_ERROR = -2,
    LISTEN_ERROR = -3,
    ACCEPT_ERROR = -4,
    READ_ERROR = -5,
    WRITE_ERROR = -6
};

enum SocketStatus {
    DISCONNECTED,
    CONNECTED
};

std::mutex mtx;

class Socket {
protected:
    SocketStatus status;
    int socket_fd;
    unsigned port;

public:
    Socket(const unsigned &port, const unsigned &protocol)
        : status(DISCONNECTED), socket_fd(-1), port(port) {
        struct sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        socket_fd = socket(AF_INET, protocol, 0);
        if (socket_fd == -1) {
            throw SOCKET_ERROR;
        }

        if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            throw BIND_ERROR;
            close(socket_fd);
        }
    }

    virtual void start() = 0;

    virtual ~Socket() {
        if (socket_fd != -1) {
            close(socket_fd);
        }
    }
};

class TCPServer : public Socket {
public:
    TCPServer(const unsigned port) : Socket(port, SOCK_STREAM) {}

    void start() override {
        if (listen(socket_fd, 3) < 0) {
            perror("listen failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "Server listening on port " << port << std::endl;

        while (true) {
            int new_socket;
            struct sockaddr_in client_address{};
            socklen_t addrlen = sizeof(client_address);

            if ((new_socket = accept(socket_fd, (struct sockaddr *)&client_address, &addrlen)) < 0) {
                perror("accept failed");
                continue;
            }

            std::thread(&TCPServer::handleClient, this, new_socket).detach();
        }
    }

private:
    std::mutex file_mutex;

    void handleClient(int client_socket) {
        char buffer[1024] = {0};
        int bytes_read = read(client_socket, buffer, sizeof(buffer));

        if (bytes_read > 0) {
            std::lock_guard<std::mutex> lock(file_mutex);
            std::ofstream log_file("log.txt", std::ios_base::app);
            if (log_file.is_open()) {
                log_file << buffer << std::endl;
            }
        }
        close(client_socket);
    }
};

class TCPClient : public Socket {
    std::string client_name;
    int period;

public:
    TCPClient(const unsigned port, const std::string& name, int period) 
        : Socket(port, SOCK_STREAM), client_name(name), period(period) {}

    void start() override {
        struct sockaddr_in server_address{};
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);

        if (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
            perror("connect failed");
            exit(EXIT_FAILURE);
        }

        while (true) {
            std::string message = "Hello from " + client_name;
            send(socket_fd, message.c_str(), message.size(), 0);
            std::cout << "Message sent to server: " << message << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(period));
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage for server: " << argv[0] << " <port> server\n";
        std::cerr << "Usage for client: " << argv[0] << " <port> client <name> <period>\n";
        return 1;
    }

    int port = std::stoi(argv[1]);
    std::string mode = argv[2];

    try {
        if (mode == "server") {
            std::unique_ptr<Socket> server = std::make_unique<TCPServer>(port);
            server->start();
        } else if (mode == "client") {
            if (argc != 5) {
                std::cerr << "Usage for client: " << argv[0] << " <port> client <name> <period>\n";
                return 1;
            }
            std::string name = argv[3];
            int period = std::stoi(argv[4]);
            std::unique_ptr<Socket> client = std::make_unique<TCPClient>(port, name, period);
            client->start();
        } else {
            std::cerr << "Invalid mode. Use 'server' or 'client'.\n";
            return 1;
        }
    } catch (SocketErrorStatus error) {
        std::cerr << "Socket error occurred: " << error << std::endl;
        return -1;
    }

    return 0;
}
