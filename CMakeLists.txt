cmake_minimum_required(VERSION 3.13)
project(ts-data-engine)
set(CMAKE_CXX_STANDARD 20)

set(HEADER_FILES
        DataGenerator/DataGenerator.h
        StorageEngine/StorageEngine.h
        Contracts/MarketEvent.h)

add_executable(data-engine ${HEADER_FILES}
        main.cpp
        DataGenerator/DataGenerator.cpp
        StorageEngine/StorageEngine.cpp)