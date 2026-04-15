//
// Created by Bogdan Vitelaru on 07.04.2026.
//

#ifndef TS_DATA_ENGINE_DATAGENERATOR_H
#define TS_DATA_ENGINE_DATAGENERATOR_H

#include "../Contracts/MarketEvent.h"
#include <memory>

class DataGenerator {
public:
    DataGenerator();
    ~DataGenerator();

    MarketEvent create_market_event();

private:
    class Impl;
    std::unique_ptr<Impl> pimpl_;
};


#endif //TS_DATA_ENGINE_DATAGENERATOR_H