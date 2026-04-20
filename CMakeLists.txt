cmake_minimum_required(VERSION 3.13)
project(ts-data-engine)
set(CMAKE_CXX_STANDARD 20)

set(DATA_GENERATOR_HEADER_FILES
        DataGenerator/DataGenerator.h
        Contracts/MarketEvent.h)

set(STORAGE_ENGINE_HEADER_FILES
        StorageEngine/StorageEngine.h
        Contracts/MarketEvent.h)

add_executable(data-generator ${DATA_GENERATOR_HEADER_FILES}
        start_data_generator.cpp
        DataGenerator/DataGenerator.cpp)

add_executable(storage-engine ${STORAGE_ENGINE_HEADER_FILES}
        start_storage.cpp
        StorageEngine/StorageEngine.cpp)