//
// Created by lisek on 28.10.2019.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <thread>
#include <regex>
#include <algorithm>
#include <sys/stat.h>
#include <fstream>
#include <time.h>
#include <boost/algorithm/string/predicate.hpp>
#include "Server.h"
#include "utils.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <boost/filesystem.hpp>
#include <arpa/inet.h>

#define MAX_CONNECTIONS 24
#define MAX_LINE_SIZE 8 * 1024
#define RECEIVE_TIMEOUT 10
#define BMAG  "\u001b[45m"
#define BBLUE "\u001b[44;1m"
#define BRED "\u001b[41m"
#define BGREEN "\u001b[42m"
#define KYELLOW "\u001b[33m"
#define RESET "\x1B[0m"


Server::Server() {
    this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->sock == -1) {
        perror("Fatal error");
        exit(1);
    }
    const int one = 1;
    setsockopt(this->sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
}

void Server::getHandler(HttpRequest &req, HttpResponse &res) {

    bool gzip = false;
    if (req.hasHeader("Accept-Encoding")) {
        auto desiredEncodings = utils::split(req.getHeader("Accept-Encoding"), ", ");
        for (const auto &entry : desiredEncodings) {
            if (std::regex_match(entry, std::regex("gzip(;q=((0\\.[1-9])|(1\\.0)))?"))) {
//                gzip = true;
                break;
            }
        }
    }

    namespace fs = boost::filesystem;
    fs::path requestPath = fs::weakly_canonical(fs::path(req.getPath().c_str()));
    fs::path resourcePath = fs::path(this->rootPath) / requestPath;

    if (fs::exists(resourcePath.parent_path() / fs::path(".htpasswd"))) {
        std::string directoryCredentials;
        std::ifstream credFile((resourcePath.parent_path() / fs::path(".htpasswd")).string());
        credFile >> directoryCredentials;
        credFile.close();
        if (req.hasHeader("Authorization")) {
            std::string credentials = utils::base64_decode(utils::split(req.getHeader("Authorization"), "Basic ")[1]);
            if (credentials != directoryCredentials) {
                res.status(403).type("text/html").send(utils::generateStatusHTML(403), gzip);
                return;
            }
        } else {
            res.set("WWW-Authenticate", "Basic");
            res.status(401).type("text/html").send(utils::generateStatusHTML(401), gzip);
            return;
        }
    }

    if (!fs::exists(resourcePath)) {
        res.status(404).type("text/html").send(utils::generateStatusHTML(404), gzip);
        return;
    }

    resourcePath = fs::weakly_canonical(resourcePath);

    if (fs::is_regular_file(resourcePath)) {

        time_t lastModified = fs::last_write_time(resourcePath);
        if (req.hasHeader("If-Modified-Since")) {
            struct tm t{};
            strptime(req.getHeader("If-Modified-Since").c_str(), "%a, %d %b %Y %T GMT", &t);
            time_t condition = mktime(&t);
            if (lastModified > condition) {
                res.sendFile(resourcePath.c_str());
            } else {
                res.status(304).end();
            }
        }
        if (req.hasHeader("If-Unmodified-Since")) {
            struct tm t{};
            strptime(req.getHeader("If-Unmodified-Since").c_str(), "%a, %d %b %Y %T GMT", &t);
            time_t condition = mktime(&t);
            if (lastModified < condition) {
                res.sendFile(resourcePath.c_str());
            } else {
                res.status(412).end();
            }
        }

        res.sendFile(resourcePath.c_str(), gzip);

    } else if (fs::is_directory(resourcePath)) {
        std::vector<fs::directory_entry> items;
        fs::directory_iterator it{resourcePath};
        while (it != fs::directory_iterator{}) {
            items.emplace_back(*it++);
        }

        std::string html;
        char buf[512];
        sprintf(buf,
                "<!DOCTYPE html><html><meta charset=\"UTF-8\"><h1>Index of %s</h1><body><table><tbody><thead><td>size</td><td>name</td></thead>",
                req.getUri().c_str());
        html += buf;
        std::sort(items.begin(), items.end(), [](fs::directory_entry &a, fs::directory_entry &b) -> bool {
            return a.path().filename().string() < b.path().filename().string();
//            if((fs::is_regular_file(a.path())?fs::file_size(a.path()):0) < (fs::is_regular_file(b.path())?fs::file_size(b.path()):0));
        });
        if (requestPath != "/") {
            html += "<tr><td></td><td><a href=\"";
            html += requestPath.parent_path().string();
            html += "\">../</a></td></tr>";
        }
        for (auto &item : items) {
            if (boost::ends_with(item.path().filename().string(), ".html")) {
                res.type("text/html").sendFile(item.path().string(), gzip);
                return;
            }
            if (item.path().filename() == ".htpasswd") continue;

            bool isDirectory = fs::is_directory(item.status());
            bool isRegularFile = fs::is_regular_file(item.status());
            if (!isDirectory && !isRegularFile) continue;

            std::string filename = item.path().filename().string() + (isDirectory ? "/" : "");
            std::string href = fs::weakly_canonical(requestPath / item.path().filename()).string();
            std::string fileSize = !isDirectory ? std::to_string(fs::file_size(item)) : "";

            sprintf(buf, "<tr><td>%s</td><td><a href=\"%s\">%s</a></td></tr>", fileSize.c_str(), href.c_str(),
                    filename.c_str());
            html += buf;
        }
        html += "</tbody></table><p>terrible-server running @ localhost:";
        html += std::to_string(port);
        html += "</p></body></html>";
        res.type("text/html").send(html, gzip);
    }
}

void Server::postHandler(HttpRequest &req, HttpResponse &res) {
    namespace fs = boost::filesystem;
    fs::path requestPath = fs::weakly_canonical(fs::path(req.getPath().c_str()));
    fs::path resourcePath = fs::path(this->rootPath) / requestPath;

    if (!fs::exists(resourcePath.parent_path())) {
        res.status(409).send(utils::generateStatusHTML(409));
        return;
    }

    std::ofstream file(resourcePath.string(), std::ios::binary);
    file.write(req.getBody().c_str(), req.getBody().size());
    file.close();
    res.status(201).send(req.getBody());
}

void Server::deleteHandler(HttpRequest &req, HttpResponse &res) {
    namespace fs = boost::filesystem;
    fs::path requestPath = fs::weakly_canonical(fs::path(req.getPath().c_str()));
    fs::path resourcePath = fs::path(this->rootPath) / requestPath;

    fs::remove(resourcePath);
    res.status(204).send(req.getBody());
}

void Server::handleConnection(int socket, sockaddr_in client, socklen_t len) {

    struct timeval timeout{};
    timeout.tv_sec = RECEIVE_TIMEOUT;
    timeout.tv_usec = 0;
    setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout));

    char buf[MAX_LINE_SIZE];

    //read and parse first line
    int lineLength = utils::getline(socket, buf, MAX_LINE_SIZE);
    if (lineLength < 0) {
        HttpResponse res(socket);
        int code = errno == EWOULDBLOCK ? 408 : 500;
        res.status(code).send(utils::generateStatusHTML(code));
        shutdown(socket, SHUT_RDWR);
        return;
    }

    if (!std::regex_match(buf, std::regex(
            "((GET)|(POST)|(PUT)|(PATCH)|(DELETE)|(HEAD)|(OPTIONS)|(TRACE)) .* HTTP/1\\.(1|0)"))) {
        HttpResponse res(socket);
        res.status(400).type("text/html").send(utils::generateStatusHTML(400));
        return;
    }
    std::string method, uri, protocol;
    auto parts = utils::split(buf, " ");
    method = parts[0];
    uri = parts[1];
    protocol = parts[2];

    if (method != "GET" && method != "HEAD" && method != "POST" && method != "DELETE") {
        HttpResponse res(socket);
        res.status(501).type("text/html").send(utils::generateStatusHTML(501));
        return;
    }


    //read and parse headers
    std::unordered_map<std::string, std::string> headers;
    while ((lineLength = utils::getline(socket, buf, MAX_LINE_SIZE) > 0)) {
        //if a header line is malformed return a 400
        if (!std::regex_match(buf, std::regex("([\\w-]+): (.*)"))) {
            HttpResponse res(socket);
            res.status(400).type("text/html").send(utils::generateStatusHTML(400));
            return;
        }
        auto x = utils::split(buf, ": ");
        headers[x[0]] = x[1];
    }

    if (lineLength < 0) {
        HttpResponse res(socket);
        int code = errno == EWOULDBLOCK ? 408 : 500;
        res.status(code).send(utils::generateStatusHTML(code));
        shutdown(socket, SHUT_RDWR);
        return;
    }

    if (headers.find("Expect") != headers.end() && headers["Expect"] == "100-continue") {
        write(socket, "HTTP/1.1 100 Continue\r\n\r\n", 25);
    }

    //A host header for HTTP 1.1 clients is mandatory
    if(headers.find("Host") == headers.end() && protocol != "HTTP 1.0"){
        HttpResponse res(socket);
        res.status(400).type("text/html").send(utils::generateStatusHTML(400));
        return;
    }

    std::string body;
    int contentLength = 0;
    if (method == "POST") {
        if (headers.find("Transfer-Encoding") != headers.end() && headers["Transfer-Encoding"] == "chunked") {
            char buf[MAX_LINE_SIZE];

            while (true) {
                utils::getline(socket, buf, MAX_LINE_SIZE);
                unsigned int chunkSize;
                std::stringstream s;
                s << buf;
                s >> std::hex >> chunkSize;
                if (chunkSize == 0) break;
                recv(socket, buf, chunkSize, MSG_WAITALL);
                body += buf;
                utils::getline(socket, buf, MAX_LINE_SIZE);
            }

        } else if (headers.find("Content-Length") == headers.end()) {
            HttpResponse res(socket);
            res.status(411).type("text/html").send(utils::generateStatusHTML(411));
            return;
        } else {
            contentLength = std::stoi(headers["Content-Length"]);
            if (contentLength > 0) {
                char *raw = new char[contentLength];

                int bytesLeft = contentLength;
                while (bytesLeft > 0) {
                    int received = recv(socket, raw + (contentLength - bytesLeft), bytesLeft, 0);

                    if (received < 0) {
                        HttpResponse res(socket);
                        int code = errno == EWOULDBLOCK ? 408 : 500;
                        res.status(code).send(utils::generateStatusHTML(code));
                        shutdown(socket, SHUT_RDWR);
                        return;
                    }

                    bytesLeft -= received;
                }

//                int received = recv(socket, raw, contentLength, MSG_WAITALL);
//                if(received == -1){
//                    shutdown(socket, SHUT_RDWR);
//                    return;
//                }
                body = std::string(raw, contentLength);
//                printf("%d %s\n", received, body.c_str());
            }
        }
    }

    HttpRequest req(client, method, body, uri, headers);
    HttpResponse res(socket);
    if (headers.find("Range") != headers.end()) {
        auto aux = utils::split(headers["Range"], "bytes=")[1];
        auto range = utils::split(aux, "-");
        unsigned int from = atoi(range[0].c_str());
        unsigned int to = atoi(range[1].c_str());
        printf("%d %d\n", from, to);
        res.range(from, to);
    }
    if (method == "GET") {
        this->getHandler(req, res);
    } else if (method == "HEAD") {
        this->getHandler(req, res.head());
    } else if (method == "POST") {
        this->postHandler(req, res);
    } else if (method == "DELETE") {
        this->deleteHandler(req, res);
    }
    auto userAgent = headers.find("User-Agent") != headers.end() ? headers["User-Agent"] : "";

    const char *color =
            method == "GET" ? BBLUE : method == "POST" ? BGREEN : method == "DELETE" ? BRED : method == "HEAD" ? BMAG
                                                                                                               : "";

    printf("%s%s" RESET " %s " KYELLOW "%s " RESET "%s\n", color, method.c_str(), uri.c_str(), inet_ntoa(client.sin_addr),userAgent.c_str());
    shutdown(socket, SHUT_RDWR);
}

void Server::start(const unsigned long address, const unsigned short port, const std::string &path) {
    this->rootPath = path;
    this->port = port;
    if (!boost::filesystem::exists(path)) {
        printf("Path does not exist\n");
        exit(0);
    }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = address;
    addr.sin_port = htons(port);

    int err = bind(this->sock, (sockaddr *) &addr, sizeof(addr));
    if (err == -1) {
        perror("Fatal error");
        exit(1);
    }

    err = listen(this->sock, MAX_CONNECTIONS);
    if (err == -1) {
        perror("Fatal error");
        exit(1);
    }
    printf("Listening on port " KYELLOW "%d" RESET "\nServing " KYELLOW "%s\n" RESET, port,
           boost::filesystem::absolute(path).c_str());

    while (true) {
        sockaddr_in client;
        socklen_t len = sizeof(client);
        int newSocket = accept(this->sock, (sockaddr *) &client, &len);
        if (newSocket == -1) {
            perror("Fatal error");
        }
        std::thread t([=] { handleConnection(newSocket, client, len); });
        t.detach();
    }
}

