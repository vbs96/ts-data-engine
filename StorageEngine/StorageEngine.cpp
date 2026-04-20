
#include "StorageEngine.h"
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netdb.h>

std::vector<MarketEvent> StorageEngine::query(const std::string& symbol, time_point start_time, time_point end_time) const{
        std::vector<MarketEvent> to_return;
        for (const auto& it: this->data_) {
                if (start_time <= it.timestamp && it.timestamp <= end_time && it.symbol == symbol)
                        to_return.push_back(it);
        }
        return std::move(to_return);
}

void StorageEngine::start() {
        // Set up socket
        int status;
        struct addrinfo hints;
        struct addrinfo *servinfo;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if ((status = getaddrinfo(NULL, "3499", &hints, &servinfo)) != 0) {
                std::cout<< "getaddrinfo() error: " << gai_strerror(status) << "\n";
                exit(1);
        }

        int socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
        if (socket_fd == -1) {
                std::cout << "socket() error. \n";
                exit(1);
        }

        status = bind(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen);
        if (status == -1) {
                std::cout << "bind() error. \n";
                exit(1);
        }

        int backlog = 5;
        status = listen(socket_fd, backlog);
        if (status == -1) {
                std::cout << "listen() error. \n";
                exit(1);
        }

        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof(their_addr);
        int new_socket_fd = accept(socket_fd, reinterpret_cast<sockaddr*>(&their_addr), &addr_size);
        if (new_socket_fd == -1) {
                std::cout << "listen() error. \n";
                exit(1);
        }
        shutdown(socket_fd, 2);

        size_t market_events_count = 0;
        auto start_time = std::chrono::system_clock::now();

        unsigned char buff[4096];
        MarketEvent market_event;
        while (true) {
                size_t len = recv(new_socket_fd, buff, sizeof(buff), 0);
                if (len == 0)
                        break;
                if (len == -1) {
                        std::cout << "recv() returned -1.\n";
                        std::cout << "Total market events received until now: " << market_events_count
                                        << ". Total bytes received until now: " << market_events_count * 32 << "\n";
                        exit(1);
                }

                for (auto i = 0; i + sizeof(MarketEvent) <= len; i += sizeof(MarketEvent)) {
                        ++market_events_count;
                        memcpy(&market_event, buff + i, sizeof(MarketEvent));
                        this->insert(market_event);
                }
        }

        std::cout << "Total market events received: " << market_events_count
        << ". Total MB received: " << market_events_count * 32 / (1024.0 * 1024.0)
        << ". Total time needed: " << std::chrono::duration<float, std::milli>{std::chrono::system_clock::now() - start_time}
        << "\n";

        shutdown(new_socket_fd, 2);
        freeaddrinfo(servinfo);
}