//
// Created by lisek on 28.10.2019.
//

#ifndef HTTP_SEVER_HTTPRESPONSE_H
#define HTTP_SEVER_HTTPRESPONSE_H

#include <string>
#include <unordered_map>

#define SERVER_STRING "terrible-server"

class HttpResponse {
public:
    HttpResponse(int sock);

    void send(const std::string &body, bool gzip = false);
    void sendFile(const std::string &path, bool gzip = false);
    void end();
    HttpResponse& head();
    HttpResponse& set(std::string key, std::string value);
    HttpResponse& type(const std::string &type);
    HttpResponse& status(unsigned short code);
    HttpResponse& range(unsigned int from, unsigned int to);


private:
    std::unordered_map<std::string, std::string> headers;
    char *assembleHeaders();

    int socket;
    unsigned short statusCode;
    bool isRanged, isHead;
    int from, to;
};


#endif //HTTP_SEVER_HTTPRESPONSE_H
