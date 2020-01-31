//
// Created by lisek on 28.10.2019.
//

#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <fstream>
#include "HttpResponse.h"
#include "gzip/compress.hpp"
#include "utils.h"

HttpResponse::HttpResponse(int sock) {
    this->isHead = false;
    this->socket = sock;
    this->statusCode = 200;
    this->isRanged = false;
//    this->contentType = "text/plain; charset=utf-8";
}

HttpResponse &HttpResponse::set(std::string key, std::string value) {
    this->headers[key] = value;
    return *this;
}

HttpResponse &HttpResponse::type(const std::string &type) {
    this->headers["Content-Type"] = type;
    return *this;
}

HttpResponse &HttpResponse::status(unsigned short code) {
    this->statusCode = code;
    return *this;
}

char * HttpResponse::assembleHeaders() {
    char headerString[1024] = {};

    char timeString[1000];
    time_t now = time(nullptr);
    struct tm tm = *gmtime(&now);
    strftime(timeString, sizeof timeString, "%a, %d %b %Y %H:%M:%S %Z", &tm);

    this->headers["Server"] = SERVER_STRING;
    this->headers["Date"] = timeString;
    this->headers["Connection"] = "close";

    for(auto const &it : this->headers) {
        char buf[512];
        sprintf(buf, "%s: %s\r\n", it.first.c_str(), it.second.c_str());
        strcat(headerString, buf);
    }

    strcat(headerString, "\r\n");

    return strdup(headerString);
}

void HttpResponse::send(const std::string &body, bool gzip) {
    std::string compressed, ranged;
    if(this->isRanged && !body.empty()){
        if(this->to > (int)body.size() || this->to <= this->from){
            this->status(416).end();
            return;
        }
        this->status(206);
        size_t size = this->from + this->to + 1;
        ranged = body.substr(this->from, size);
    }
    else if(gzip && !body.empty()){
        compressed = gzip::compress(body.data(), body.size());
        this->set("Content-Encoding", "gzip");
        this->set("Content-Length", std::to_string(compressed.size()));
    }
    else{
        this->set("Content-Length", std::to_string(body.size()));
    }

    const char* headersString = assembleHeaders();
    char statusLine[64];
    sprintf(statusLine, "HTTP/1.1 %d %s\r\n", this->statusCode, utils::getStatusCodeString(this->statusCode).c_str());

    write(this->socket, statusLine, strlen(statusLine));
    write(this->socket, headersString, strlen(headersString));
    delete[] headersString;
    if(!body.empty() && !this->isHead){
        if(this->isRanged){
            write(this->socket, ranged.data(), ranged.size());
        }
        else if(gzip){
            write(this->socket, compressed.data(), compressed.size());
        }
        else {
            write(this->socket, body.data(), body.size());
        }
    }
}

//void HttpResponse::sendFile(const std::string &path) {
//
//    char statusLine[64];
//    sprintf(statusLine, "HTTP/1.1 %d %s\r\n", this->statusCode, utils::getStatusCodeString(this->statusCode).c_str());
//    this->set("Transfer-Encoding", "chunked");
//    this->set("Content-Type", utils::getMimeType(path.substr(path.find_last_of('.') + 1)) + "; charset=utf-8");
//    const char* headers = assembleHeaders();
////    printf("%s\n", headers);
//    write(this->socket, statusLine, strlen(statusLine));
//    write(this->socket, headers, strlen(headers));
//
//
//    int file = open(path.c_str(), O_RDONLY);
//    char buf[1024];
//    int bytes = 0;
//    while((bytes = read(file, buf, 1024)) > 0){
//        char hex[10];
//        sprintf(hex, "%x\r\n", bytes);
//        write(this->socket, hex, strlen(hex));
//        write(this->socket, buf, bytes);
//        write(this->socket, "\r\n", 2);
//    }
//    close(file);
////    final chunk
//    sprintf(buf, "0\r\n\r\n");
//    write(this->socket, buf, 5);
//
//}

void HttpResponse::sendFile(const std::string &path, bool gzip) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buf = new char[size];
    if (file.read(buf, size))
    {
        buf[size] = 0;
        this->set("Content-Type", utils::getMimeType(path.substr(path.find_last_of('.') + 1)));
        send(buf, gzip);
    }
    else{
        status(500).end();
    }
    delete[] buf;

}

void HttpResponse::end(){
    send("");
}

HttpResponse &HttpResponse::range(unsigned int from, unsigned int to) {
    this->isRanged = true;
    this->from = from;
    this->to = to;
    return *this;
}

HttpResponse &HttpResponse::head(){
    this->isHead = true;
    return *this;
}

#include "HttpResponse.h"
