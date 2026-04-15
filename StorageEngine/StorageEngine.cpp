
#include "StorageEngine.h"

std::vector<MarketEvent> StorageEngine::query(const std::string& symbol, time_point start_time, time_point end_time) const{
        std::vector<MarketEvent> to_return;
        for (const auto& it: this->data_) {
                if (start_time <= it.timestamp && it.timestamp <= end_time && it.symbol == symbol)
                        to_return.push_back(it);
        }
        return std::move(to_return);
}