#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

class Client {
    public:
        Client();
        int startClient(int port, std::string hostname);
        int closeClient();
        int sendServer(std::string request);
        int readServer(std::string &request);
        int randomQuestion();

    private:
        int socket_fd;
        int readSize(uint32_t &len);

};