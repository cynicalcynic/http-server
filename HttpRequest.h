//
// Created by lisek on 28.10.2019.
//

#ifndef HTTP_SEVER_HTTPREQUEST_H
#define HTTP_SEVER_HTTPREQUEST_H

#include <unordered_map>
#include <netinet/in.h>

//typedef std::unordered_map<std::string, std::string> Headers;

class HttpRequest {
public:

    HttpRequest(const sockaddr_in &remoteHost, const std::string &method, std::string body,
                const std::string uri, const std::unordered_map<std::string, std::string> &headers);

    virtual ~HttpRequest();

    const sockaddr_in &getRemoteHost() const {
        return remoteHost;
    }

    const std::string &getMethod() const {
        return method;
    }

    const std::string &getUri() const {
        return uri;
    }

    const std::unordered_map<std::string, std::string> &getAllHeaders() const {
        return headers;
    }

    bool hasHeader(const std::string &header) const {
        return headers.find(header) != headers.end();
    }

    std::string getHeader(const std::string &header) {
        if(headers.find(header) != headers.end())
            return headers[header];
        else return "";
    }

    std::string getBody() const {
        return body;
    }

    std::string getPath();

private:
    sockaddr_in remoteHost;
    std::string method;
    std::string uri;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    std::unordered_map<std::string, std::string> params;
};


#endif //HTTP_SEVER_HTTPREQUEST_H
