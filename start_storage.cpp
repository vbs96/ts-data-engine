//
// Created by Bogdan Vitelaru on 19.04.2026.
//

#include "StorageEngine/StorageEngine.h"

int main() {
    constexpr size_t mem_container_size = 2000000;
    StorageEngine tse {mem_container_size};
    tse.start();

    return 0;
}