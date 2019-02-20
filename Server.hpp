#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

class Server {
    public:
        Server();
        int createSocket();
        int bindSocket();
        int listenToConnections();
        int acceptConnection();
        int getClientSocket();
        int getServerSocket();
        int getPort();
        std::string getHostName();
        void closeClientSocket();
        void closeServerSocket();
        int readClient(std::string &request);
        int sendClient(std::string &response);
        void displayHostInfo();
    private:
        socklen_t len;
        int server_socket;
        int client_socket;
        sockaddr_in address;
        int incrementor;
        int sendResponseSize(int size);
        int readResponseSize(uint32_t &len);
};