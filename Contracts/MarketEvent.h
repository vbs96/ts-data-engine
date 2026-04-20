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
    static constexpr short SYMBOL_MAX_SIZE = 8;
    char symbol[SYMBOL_MAX_SIZE];
    time_point timestamp;
    size_t volume;
    float price;
    MarketEventType type;

    std::string get_type_as_string() const{
        if (type == MarketEventType::Bid)
            return "Bid";
        if (type == MarketEventType::Ask)
            return "Ask";
        return "Trade";
    }

    std::string to_string() const{
        std::ostringstream oss;
        oss << "Symbol: " << symbol << " Timestamp: " << std::format("{:%Y%m%d%H%M}", timestamp)
            << " Volume: " << volume << " Price: " << price << " MarketEventType: " << get_type_as_string();
        return oss.str();
    }
};

#endif //TS_DATA_ENGINE_TRANSACTION_H