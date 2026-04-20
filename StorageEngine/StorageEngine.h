//
// Created by Bogdan Vitelaru on 07.04.2026.
//

#ifndef TS_DATA_ENGINE_TIMESERIESSTORAGE_H
#define TS_DATA_ENGINE_TIMESERIESSTORAGE_H

#include "../Contracts/MarketEvent.h"
#include <vector>
#include <chrono>

class StorageEngine {
public:
    StorageEngine(size_t reservationSize) {
        data_.reserve(reservationSize);
    }

    void insert(MarketEvent market_data) {
        data_.emplace_back(std::move(market_data));
    }

    void insert(std::vector<MarketEvent> market_data) {
        data_.insert(std::end(data_), std::begin(market_data), std::end(market_data));
    }

    // Return all transactions on "symbol" from start to end time points.
    std::vector<MarketEvent> query(const std::string& symbol, time_point start_time, time_point end_time) const;

    void start();

private:
    std::vector<MarketEvent> data_;
};

#endif //TS_DATA_ENGINE_TIMESERIESSTORAGE_H