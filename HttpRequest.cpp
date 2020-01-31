//
// Created by lisek on 28.10.2019.
//


#include "HttpRequest.h"
#include "urilite.h"
#include "utils.h"

HttpRequest::HttpRequest(const sockaddr_in &remoteHost, const std::string &method, std::string body,
                         const std::string uri, const std::unordered_map<std::string, std::string> &headers)
        : remoteHost(remoteHost), method(method), uri(uri), headers(headers), body(body)
{

}

std::string HttpRequest::getPath(){
    std::string path = utils::split(urilite::uri::decode(this->uri), "?")[0];

    if(path == "/") return path;
    while(path.back() == '/') path.pop_back();
    return path;
}


HttpRequest::~HttpRequest() {

}
