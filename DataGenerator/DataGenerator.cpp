//
// Created by Bogdan Vitelaru on 07.04.2026.
//

#include "DataGenerator.h"
#include <iostream>
#include <fstream>
#include <random>
#include <sys/socket.h>
#include <netdb.h>
#include <thread>

class DataGenerator::Impl {
public:
    using rng_result_type = std::mt19937_64::result_type;
    using u_dist_t = std::uniform_int_distribution<rng_result_type>;

    Impl() {
        std::random_device dev;
        rng_.seed(dev());

        // Generate random index into initialization vector for market instruments
        random_symbol_dist_ = u_dist_t{0, possible_symbols_.size() - 1};

        // Generate volumes between 1 -> UINT16_MAX
        random_trade_vol_dist_ = u_dist_t{1, UINT16_MAX};

        // Will help generate price fluctuations between [-10, 10]%
        random_price_dist_ = u_dist_t{0, 20};

        // Start timestamp for date and time generation
        start_time_ = std::chrono::system_clock::now();

        // Uniform distribution to generate a random number of milliseconds between [0, 100]
        random_timestamp_dist_ = u_dist_t{0, 100};
    }

    MarketEvent generate_market_event() {
        auto index = random_symbol_dist_(rng_);
        MarketEvent market_event;
        strncpy(market_event.symbol,this->possible_symbols_[index].first.c_str(), possible_symbols_[index].first.size() + 1);
        market_event.timestamp = generate_event_timestamp();
        market_event.volume = random_trade_vol_dist_(rng_);
        market_event.price = generate_price(index);
        market_event.type = random_price_dist_(rng_) % 2 == 0 ? MarketEventType::Ask : MarketEventType::Bid;
        return market_event;
    }

private:
    time_point generate_event_timestamp() {
        auto random_milliseconds = random_timestamp_dist_(rng_);
        return start_time_ += std::chrono::milliseconds{random_milliseconds};
    }

    float generate_price(rng_result_type index) {
        auto random_fluctuation = static_cast<int>(random_price_dist_(rng_)) - 10;
        possible_symbols_[index].second *= (1 + random_fluctuation / 100.0f);

        return possible_symbols_[index].second;
    }

    std::vector<std::pair<std::string, float>> possible_symbols_ {
            {"AAPL", 100} , {"MSFT", 400}, {"TSMC", 12}, {"NVDA", 81.1},
            {"GOOGL", 40}, {"META", 14.4}, {"AMD", 40.1}
    };

    std::mt19937_64 rng_;
    u_dist_t random_symbol_dist_;
    u_dist_t random_trade_vol_dist_;
    u_dist_t random_price_dist_;
    u_dist_t random_timestamp_dist_;

    time_point start_time_;
};

//
//
// DataGenerator methods
DataGenerator::DataGenerator() {
    pimpl_ = std::make_unique<Impl>();
}

DataGenerator::~DataGenerator() = default;

MarketEvent DataGenerator::create_market_event() {
    return pimpl_->generate_market_event();
}

void DataGenerator::start() {
    using namespace std::literals::chrono_literals;

    // Set up socket
    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, "3499", &hints, &servinfo)) != 0) {
        std::cout << "getaddrinfo() error: " << gai_strerror(status) << "\n";
        exit(1);
    }

    int socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (socket_fd == -1) {
        std::cout << "socket() error. \n";
        exit(1);
    }

    status = connect(socket_fd, servinfo->ai_addr, servinfo->ai_addrlen);
    if (status == -1) {
        std::cout << "connect() error. \n";
        exit(1);
    }

    time_point start_timepoint = std::chrono::system_clock::now();
    size_t count = 0;

    while (std::chrono::system_clock::now() - start_timepoint <= 1s ) {
        auto market_event = this->pimpl_->generate_market_event();
        auto bytes_sent = send(socket_fd, &market_event, sizeof(market_event), 0);

        count++;
        if (bytes_sent != sizeof(market_event)) {
            std::cout << "send() didn't sent all the content of msg. \n";
            std::cout<< "Market events sent until now: " << count << ". Total bytes until now sent: "
            << count * 32 << "\n";
            exit(1);
        }
    }

    std::cout<< "Total market events sent in 1s: " << count
    << ". Total MB sent in 1s: " << count * 32 / (1024.0 * 1024.0) << "\n";

    shutdown(socket_fd, 2);
    freeaddrinfo(servinfo);
}
