//
// Created by Bogdan Vitelaru on 15.04.2026.
//

#ifndef TS_DATA_ENGINE_TRANSACTION_H
#define TS_DATA_ENGINE_TRANSACTION_H

#include <chrono>

using time_point = std::chrono::time_point<std::chrono::system_clock>;

enum class MarketEventType {
    Bid,
    Ask,
    Trade
};

struct MarketEvent {
    std::string symbol;
    time_point timestamp;
    size_t volume;
    float price;
    MarketEventType type;
};

#endif //TS_DATA_ENGINE_TRANSACTION_H