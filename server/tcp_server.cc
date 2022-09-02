#include "./tcp_server.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Poco/Task.h"
#include "Poco/Util/ServerApplication.h"

using Poco::Task;
using Poco::Util::Application;

TCPServer::TCPServer() : Task("TCPServer") {

}

void TCPServer::runTask() {
    Application &app = Application::instance();
    Poco::Logger &logger = app.logger();
    Poco::Util::LayeredConfiguration &config = app.config();

    if (config.getBool("application.runAsDaemon", false)) {
        logger.information("Running as daemon");
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "socket error" << std::endl;
    }

    int optVal = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optVal, sizeof(int));

    struct sockaddr_in serv_addr, cli_addr;
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;;
    serv_addr.sin_port = htons(std::stoi(config.getString("server-port")));
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "bind error" << std::endl;
    }

    listen(sockfd, 5);

    while (!isCancelled()) {
        socklen_t clilen = sizeof(cli_addr);
        int incomingSock = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (incomingSock < 0) {
            std::cerr << "accept error" << std::endl;
        }
        std::cout << "server: got connection from = " << inet_ntoa(cli_addr.sin_addr) << " and port = "
                  << ntohs(cli_addr.sin_port) << std::endl;

        write(incomingSock, "You are connected!", std::stoi(config.getString("msg-length")));

        unsigned int buffer_length = std::stoi(config.getString("max-buffer"));
        std::string buffer(buffer_length, 0);
        if (read(incomingSock, &buffer[0], buffer_length - 1) < 0) {
            std::cerr << "read error" << std::endl;
        }
        std::cout << "Got the message: " << buffer << std::endl;

        close(incomingSock);
        cancel();
    }

    close(sockfd);
}
