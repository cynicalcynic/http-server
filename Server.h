//
// Created by lisek on 28.10.2019.
//

#ifndef HTTP_SEVER_SERVER_H
#define HTTP_SEVER_SERVER_H


#include <unordered_map>
#include <functional>
#include "HttpResponse.h"
#include "HttpRequest.h"

class Server {
public:
    Server();
    void start(unsigned long address, unsigned short port,  const std::string &path);

private:
    void handleConnection(int socket, sockaddr_in client, socklen_t len);
    void getHandler(HttpRequest &req, HttpResponse &res);

    int sock;
    std::string rootPath;
    unsigned short port;
    void postHandler(HttpRequest &req, HttpResponse &res);

    void deleteHandler(HttpRequest &req, HttpResponse &res);
};


#endif //HTTP_SEVER_SERVER_H
