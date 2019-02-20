#include "Server.hpp"

Server::Server() {
    this->len = sizeof(address);
}

int Server::createSocket() {
    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(this->server_socket == -1) {
        return -1;
    }

    printf("Socket created\n");

    return this->server_socket;
}

int Server::bindSocket() {
    // bind the socket to an IP/port - what port am i on?
    this->address.sin_family = AF_INET;
    this->address.sin_port = htons(0);
    inet_pton(AF_INET, "0.0.0.0", &this->address.sin_addr); // 127.0.0.1 localhost
    // bind listening socket to an address
    int ret = bind(this->server_socket, (sockaddr*)&this->address, sizeof(this->address));
    if(ret == -1) {
        return -1;
    }

    printf("Socket binded\n");
    return ret;
}

int Server::getPort() {
    int ret = getsockname(this->server_socket, (sockaddr*)&this->address, &this->len);

    if(ret == -1) {
        fprintf(stderr, "Error: could not get port");
        return -1;
    }

    return ntohs(this->address.sin_port);
}

std::string  Server::getHostName() {
    char hostbuffer[256];
    int hostname;
    hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
    if (hostname == -1) { 
        fprintf(stderr, "Error: could not get hostname");
        exit(0);
    } 


    return std::string(hostbuffer);
}

int Server::listenToConnections() {
    int ret = listen(this->server_socket, SOMAXCONN);

    if(ret ==  -1) {
        fprintf(stderr, "Error: could not listen to client");
        return -1;
    }
    return ret;
}

int Server::acceptConnection() {
    sockaddr_in client;
    socklen_t client_size = sizeof(client);
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    this->client_socket = accept(this->server_socket, (sockaddr*)&client, &client_size);

    if(this->client_socket == -1) {
        fprintf(stderr, "Error: connecting to client");
        return -1;
    }
    
    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int ret = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if(ret) {
        printf("%s connected on %s\n", host, svc);
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        printf("%s connected on %d\n", host, ntohs(client.sin_port));
    }

    return this->client_socket;
}

void Server::closeClientSocket() {
    // close socket
    close(this->client_socket);
}

void Server::closeServerSocket() {
    // close socket
    close(this->server_socket);
}

int Server::getClientSocket() {
    return this->client_socket;
}

int Server::readResponseSize(uint32_t &len) {
    int ret = recv(client_socket, &len, sizeof(uint32_t), 0);

    return ret;
}

int Server::readClient(std::string &request) {
    uint32_t len;
    int ret = readResponseSize(len);

    if(ret == -1) {
        std::cerr << "error reading  (size) from client" << std::endl;
        return -1;
    }

    if(ret == 0) {
        return 0;
    }

    int size = ntohl(len);
    char *buf;
    buf = new char[size];
    memset(buf, '\0', size);

    ret = recv(client_socket, buf, size + 1, 0);

    if(ret == -1) {
        std::cerr << "Error reading from client" << std::endl;
        return -1;
    }

    std::string s(buf);
    request = s;


    delete [] buf;

    return ret;
}

int Server::sendResponseSize(int size) {
    uint32_t len = htonl(size);
    int ret = send(client_socket, &len, sizeof(uint32_t), 0);
    if(ret == -1) {
        return -1;
    }

    return ret;
}

int Server::sendClient(std::string &response) {
    int ret = sendResponseSize(response.length());
    if(ret == -1) {
        std::cerr << "error sending (packet size) to client" << std::endl;
        return -1;
    }
  
    ret = send(client_socket, response.c_str(), response.length(), 0);
    if(ret == -1) {
        std::cerr << "error sending to client" << std::endl;
        return -1;
    }
   
    return ret;
}

void Server::displayHostInfo() {
    std::string hostname = getHostName();
    int port = getPort();

    std::cout << "Hostname: " << hostname.c_str() << std::endl;
    std::cout << "Port: " << port << std::endl;
}