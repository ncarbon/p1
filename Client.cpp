#include "Client.hpp"

Client::Client() {

}

int Client::startClient(int port, std::string hostname) {
    struct hostent *ent = gethostbyname(hostname.c_str());

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(socket_fd == -1) {
        fprintf(stderr, "Error: socket could not be created");
        return -1;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    struct in_addr ip_addr = *(struct in_addr *)(ent->h_addr);
    inet_pton(AF_INET, inet_ntoa(ip_addr), &address.sin_addr); // 127.0.0.1 localhost
    socklen_t len = sizeof(address);


	int res = connect(socket_fd, (sockaddr*)&address, sizeof(address));

	if(res < 0) {
		fprintf(stderr, "Error: failed to connect to server\n");
        return -1;
	}

    return socket_fd;

}

int Client::closeClient() {
    return close(socket_fd);
}

int Client::sendServer(std::string request) {
    uint32_t len = htonl(request.length());
    int res = send(socket_fd, &len, sizeof(uint32_t), 0);
    if(res == -1) {
        std::cerr << "error sending (packet size) to client" << std::endl;
        return -1;
    }

    res = send(socket_fd, request.c_str(), request.length(), 0);
    if(res == -1) {
        std::cerr << "error sending to server" << std::endl;
        return -1;
    }

    return res;

}

int Client::readSize(uint32_t &len) {
    int res = recv(socket_fd, &len, sizeof(uint32_t), 0);

    return res;
}

int Client::readServer(std::string &response) {
    uint32_t len;
    int res = readSize(len);
    if(res == -1) {
        std::cerr << "error reading  (size) from server" << std::endl;
        return -1;
    }

    int size = ntohl(len);
    char *buf;
    buf = new char[size];
    memset(buf, '\0', size);
    res = recv(socket_fd, buf, size+1, 0);

    if(res == -1) {
        std::cerr << "error reading from server" << std::endl;
        return -1;
    }
    std::string s(buf);
    response = s;

    delete [] buf;

    return res;
}

int Client::randomQuestion() {
    std::string response = "";
    int ret = sendServer("r");
    if(ret == -1) { std::cerr << "error sending to server" << std::endl;return -1;}

    ret = readServer(response);
    if(ret == -1) { std::cerr << "error reading from server" << std::endl;return -1;}
    
    std::cout << response << std::endl;

    std::string answer;
    std::cin >> answer;
    ret = sendServer(answer);
    if(ret == -1) { std::cerr << "error reading from server" << std::endl;return -1;};


    ret = readServer(response);
    if(ret == -1) { std::cerr << "error reading from server" << std::endl;return -1;};

    std::cout << response << std::endl;

    return ret;
}
