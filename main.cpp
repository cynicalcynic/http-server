#include <iostream>
#include <regex>
#include <boost/program_options.hpp>
#include <arpa/inet.h>
#include <signal.h>
#include "Server.h"
using namespace std;
namespace po = boost::program_options;

int main(int ac, char* av[]) {

    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, [](int signum){printf("Server killed\n"); exit(0);});

    try {

        po::options_description desc("HTTP Server\nUsage");
        desc.add_options()
                ("help", "show this help message")
                ("root,r", po::value<std::string>()->default_value("."), "set root path")
                ("port,p", po::value<int>()->default_value(8080), "set port")
                ("address,a", po::value<std::string>()->default_value("0.0.0.0"), "set address")
                ;

        po::positional_options_description p;
        p.add("path", 1);

        po::variables_map vm;
        po::store(po::command_line_parser(ac, av).
                options(desc).positional(p).run(), vm);
        po::notify(vm);


        if (vm.count("help")) {
            cout << desc << "\n";
            return 0;
        }

        std::string address = vm["address"].as<std::string >();
        int port = vm["port"].as<int >();
        std::string root = vm["root"].as<std::string >();

        Server server;
        server.start(inet_addr(address.c_str()), port, root);

    }
    catch(exception& e) {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...) {
        cerr << "Exception of unknown type!\n";
    }
    return 0;
}