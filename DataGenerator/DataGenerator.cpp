//
// Created by Bogdan Vitelaru on 07.04.2026.
//

#include "DataGenerator.h"
#include <random>

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
        return MarketEvent{
            possible_symbols_[index].first,
            generate_event_timestamp(),
            random_trade_vol_dist_(rng_),
            generate_price(index),
            random_price_dist_(rng_) % 2 == 0 ? MarketEventType::Ask : MarketEventType::Bid
        };
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
