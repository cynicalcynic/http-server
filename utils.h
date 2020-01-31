//
// Created by lisek on 30.10.2019.
//

#ifndef HTTP_SEVER_UTILS_H
#define HTTP_SEVER_UTILS_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <vector>
#include <sstream>

class utils{
public:
    static int getline(int sock, char *buf, int n) {
        char c;
        int i = 0;
        int received;
        while (i < n - 1) {
            received = recv(sock, &c, 1, 0);
            if(received < 0) return received;
            if (c == '\r') {
                received = recv(sock, &c, 1, 0);
                if(received < 0) return received;
                if (c == '\n') {
                    buf[i] = '\0';
                    break;
                } else
                    buf[i++] = '\r';
            } else
                buf[i++] = c;
        }
        return i;
    }

    static std::vector<std::string> split(const std::string &str, const std::string delim = " ") {
        std::vector<std::string> cont;
        std::size_t current, previous = 0;
        current = str.find(delim);
        while (current != std::string::npos) {
            cont.push_back(str.substr(previous, current - previous));
            previous = current + delim.size();
            current = str.find(delim, previous);
        }
        cont.push_back(str.substr(previous, current - previous));
        return cont;
    }

    static std::string generateStatusHTML(int code){
        std::stringstream html;
        html << "<!DOCTYPE html><html><body><h1>" << code << " " << utils::getStatusCodeString(code) << "</h1></body></html>";
        return html.str();
    }

    /*
   base64.cpp and base64.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/
    static inline bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {

        const std::string base64_chars =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";

        std::string ret;
        int i = 0;
        int j = 0;
        unsigned char char_array_3[3];
        unsigned char char_array_4[4];

        while (in_len--) {
            char_array_3[i++] = *(bytes_to_encode++);
            if (i == 3) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for(i = 0; (i <4) ; i++)
                    ret += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i)
        {
            for(j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++)
                ret += base64_chars[char_array_4[j]];

            while((i++ < 3))
                ret += '=';

        }

        return ret;

    }
    static std::string base64_decode(std::string const& encoded_string) {

        const std::string base64_chars =
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz"
                "0123456789+/";

        int in_len = encoded_string.size();
        int i = 0;
        int j = 0;
        int in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        std::string ret;

        while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
            char_array_4[i++] = encoded_string[in_]; in_++;
            if (i ==4) {
                for (i = 0; i <4; i++)
                    char_array_4[i] = base64_chars.find(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    ret += char_array_3[i];
                i = 0;
            }
        }

        if (i) {
            for (j = i; j <4; j++)
                char_array_4[j] = 0;

            for (j = 0; j <4; j++)
                char_array_4[j] = base64_chars.find(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
        }

        return ret;
    }

    static std::string getStatusCodeString(int code) {
        switch (code) {
            case 100:
                return "Continue";
            case 101:
                return "Switching Protocols";
            case 102:
                return "Processing";
            case 200:
                return "OK";
            case 201:
                return "Created";
            case 202:
                return "Accepted";
            case 203:
                return "Non-authoritative Information";
            case 204:
                return "No Content";
            case 205:
                return "Reset Content";
            case 206:
                return "Partial Content";
            case 207:
                return "Multi-Status";
            case 208:
                return "Already Reported";
            case 226:
                return "IM Used";
            case 300:
                return "Multiple Choices";
            case 301:
                return "Moved Permanently";
            case 302:
                return "Found";
            case 303:
                return "See Other";
            case 304:
                return "Not Modified";
            case 305:
                return "Use Proxy";
            case 307:
                return "Temporary Redirect";
            case 308:
                return "Permanent Redirect";
            case 400:
                return "Bad Request";
            case 401:
                return "Unauthorized";
            case 402:
                return "Payment Required";
            case 403:
                return "Forbidden";
            case 404:
                return "Not Found";
            case 405:
                return "Method Not Allowed";
            case 406:
                return "Not Acceptable";
            case 407:
                return "Proxy Authentication Required";
            case 408:
                return "Request Timeout";
            case 409:
                return "Conflict";
            case 410:
                return "Gone";
            case 411:
                return "Length Required";
            case 412:
                return "Precondition Failed";
            case 413:
                return "Payload Too Large";
            case 414:
                return "Request-URI Too Long";
            case 415:
                return "Unsupported Media Type";
            case 416:
                return "Requested Range Not Satisfiable";
            case 417:
                return "Expectation Failed";
            case 418:
                return "I\'m a teapot";
            case 421:
                return "Misdirected Request";
            case 422:
                return "Unprocessable Entity";
            case 423:
                return "Locked";
            case 424:
                return "Failed Dependency";
            case 426:
                return "Upgrade Required";
            case 428:
                return "Precondition Required";
            case 429:
                return "Too Many Requests";
            case 431:
                return "Request Header Fields Too Large";
            case 444:
                return "Connection Closed Without Response";
            case 451:
                return "Unavailable For Legal Reasons";
            case 499:
                return "Client Closed Request";
            case 500:
                return "Internal Server Error";
            case 501:
                return "Not Implemented";
            case 502:
                return "Bad Gateway";
            case 503:
                return "Service Unavailable";
            case 504:
                return "Gateway Timeout";
            case 505:
                return "HTTP Version Not Supported";
            case 506:
                return "Variant Also Negotiates";
            case 507:
                return "Insufficient Storage";
            case 508:
                return "Loop Detected";
            case 510:
                return "Not Extended";
            case 511:
                return "Network Authentication Required";
            case 599:
                return "Network Connect Timeout Error";
            default:
                return "OK";
        }
    }

    static std::string getMimeType(const std::string &ext){
        if(ext == "a") return "application/octet-stream";
        else if(ext == "ai") return "application/postscript";
        else if(ext == "aif") return "audio/x-aiff";
        else if(ext == "aifc") return "audio/x-aiff";
        else if(ext == "aiff") return "audio/x-aiff";
        else if(ext == "au") return "audio/basic";
        else if(ext == "avi") return "video/x-msvideo";
        else if(ext == "bat") return "text/plain";
        else if(ext == "bin") return "application/octet-stream";
        else if(ext == "bmp") return "image/x-ms-bmp";
        else if(ext == "c") return "text/plain";
        else if(ext == "cdf") return "application/x-cdf";
        else if(ext == "csh") return "application/x-csh";
        else if(ext == "csv") return "text/csv";
        else if(ext == "css") return "text/css";
        else if(ext == "dll") return "application/octet-stream";
        else if(ext == "doc") return "application/msword";
        else if(ext == "dot") return "application/msword";
        else if(ext == "dvi") return "application/x-dvi";
        else if(ext == "eml") return "message/rfc822";
        else if(ext == "eps") return "application/postscript";
        else if(ext == "etx") return "text/x-setext";
        else if(ext == "exe") return "application/octet-stream";
        else if(ext == "gif") return "image/gif";
        else if(ext == "gtar") return "application/x-gtar";
        else if(ext == "h") return "text/plain";
        else if(ext == "hdf") return "application/x-hdf";
        else if(ext == "htm") return "text/html";
        else if(ext == "html") return "text/html";
        else if(ext == "jpe") return "image/jpeg";
        else if(ext == "jpeg") return "image/jpeg";
        else if(ext == "jpg") return "image/jpeg";
        else if(ext == "js") return "application/javascript";
        else if(ext == "ksh") return "text/plain";
        else if(ext == "latex") return "application/x-latex";
        else if(ext == "m1v") return "video/mpeg";
        else if(ext == "man") return "application/x-troff-man";
        else if(ext == "me") return "application/x-troff-me";
        else if(ext == "mht") return "message/rfc822";
        else if(ext == "mhtml") return "message/rfc822";
        else if(ext == "mif") return "application/x-mif";
        else if(ext == "mov") return "video/quicktime";
        else if(ext == "movie") return "video/x-sgi-movie";
        else if(ext == "mp2") return "audio/mpeg";
        else if(ext == "mp3") return "audio/mpeg";
        else if(ext == "mp4") return "video/mp4";
        else if(ext == "mpa") return "video/mpeg";
        else if(ext == "mpe") return "video/mpeg";
        else if(ext == "mpeg") return "video/mpeg";
        else if(ext == "mpg") return "video/mpeg";
        else if(ext == "ms") return "application/x-troff-ms";
        else if(ext == "nc") return "application/x-netcdf";
        else if(ext == "nws") return "message/rfc822";
        else if(ext == "o") return "application/octet-stream";
        else if(ext == "obj") return "application/octet-stream";
        else if(ext == "oda") return "application/oda";
        else if(ext == "pbm") return "image/x-portable-bitmap";
        else if(ext == "pdf") return "application/pdf";
        else if(ext == "pfx") return "application/x-pkcs12";
        else if(ext == "pgm") return "image/x-portable-graymap";
        else if(ext == "php") return "text/plain";
        else if(ext == "png") return "image/png";
        else if(ext == "pnm") return "image/x-portable-anymap";
        else if(ext == "pot") return "application/vnd.ms-powerpoint";
        else if(ext == "ppa") return "application/vnd.ms-powerpoint";
        else if(ext == "ppm") return "image/x-portable-pixmap";
        else if(ext == "pps") return "application/vnd.ms-powerpoint";
        else if(ext == "ppt") return "application/vnd.ms-powerpoint";
        else if(ext == "pptx") return "application/vnd.ms-powerpoint";
        else if(ext == "ps") return "application/postscript";
        else if(ext == "pwz") return "application/vnd.ms-powerpoint";
        else if(ext == "py") return "text/x-python";
        else if(ext == "pyc") return "application/x-python-code";
        else if(ext == "pyo") return "application/x-python-code";
        else if(ext == "qt") return "video/quicktime";
        else if(ext == "ra") return "audio/x-pn-realaudio";
        else if(ext == "ram") return "application/x-pn-realaudio";
        else if(ext == "ras") return "image/x-cmu-raster";
        else if(ext == "rdf") return "application/xml";
        else if(ext == "rgb") return "image/x-rgb";
        else if(ext == "roff") return "application/x-troff";
        else if(ext == "rtx") return "text/richtext";
        else if(ext == "sgm") return "text/x-sgml";
        else if(ext == "sgml") return "text/x-sgml";
        else if(ext == "sh") return "application/x-sh";
        else if(ext == "shar") return "application/x-shar";
        else if(ext == "snd") return "audio/basic";
        else if(ext == "so") return "application/octet-stream";
        else if(ext == "src") return "application/x-wais-source";
        else if(ext == "swf") return "application/x-shockwave-flash";
        else if(ext == "t") return "application/x-troff";
        else if(ext == "tar") return "application/x-tar";
        else if(ext == "tcl") return "application/x-tcl";
        else if(ext == "tex") return "application/x-tex";
        else if(ext == "texi") return "application/x-texinfo";
        else if(ext == "texinfo") return "application/x-texinfo";
        else if(ext == "tif") return "image/tiff";
        else if(ext == "tiff") return "image/tiff";
        else if(ext == "tr") return "application/x-troff";
        else if(ext == "tsv") return "text/tab-separated-values";
        else if(ext == "txt") return "text/plain";
        else if(ext == "ustar") return "application/x-ustar";
        else if(ext == "vcf") return "text/x-vcard";
        else if(ext == "wav") return "audio/x-wav";
        else if(ext == "wiz") return "application/msword";
        else if(ext == "wsdl") return "application/xml";
        else if(ext == "xbm") return "image/x-xbitmap";
        else if(ext == "xlb") return "application/vnd.ms-excel";
        else if(ext == "xls") return "application/vnd.ms-excel";
        else if(ext == "xlsx") return "application/vnd.ms-excel";
        else if(ext == "xml") return "text/xml";
        else if(ext == "xpdl") return "application/xml";
        else if(ext == "xpm") return "image/x-xpixmap";
        else if(ext == "xsl") return "application/xml";
        else if(ext == "xwd") return "image/x-xwindowdump";
        else if(ext == "zip") return "application/zip";
        else return "application/octet-stream";
    }

};

#endif //HTTP_SEVER_UTILS_H
