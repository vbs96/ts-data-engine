//
// Created by Bogdan Vitelaru on 07.04.2026.
//

#include "DataGenerator/DataGenerator.h"
#include "StorageEngine/StorageEngine.h"

#include <iostream>
#include <mach/mach.h>

auto toMB = [](uint64_t bytes) {
    return bytes / (1024.0 * 1024.0);
};

void print_memory_diag() {
    struct task_basic_info t_info;
    mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;
    if (KERN_SUCCESS != task_info(mach_task_self(),
                                  TASK_BASIC_INFO,
                                  (task_info_t)&t_info,
                                  &t_info_count))
    {
        return;
    }
    std::cout << "Memory used MB: " << toMB(t_info.resident_size) << "\n";
}

int main() {
    using namespace std::chrono;

    constexpr size_t mem_container_size = 2000000;
    StorageEngine tse {mem_container_size};

    DataGenerator dg;
    auto start = std::chrono::steady_clock::now();
    uint count = 0;
    uint64_t mem_used = 0;

    while (std::chrono::steady_clock::now() - start < std::chrono::seconds(1)) {
        auto transaction = dg.create_market_event();
        mem_used+=sizeof(transaction);
        tse.insert(std::move(transaction));
        count++;
    }

    std::cout<<"Ingestion phase took: "<<
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start)
    << "Generated: " << count << " market events"
    << "\n";

    // Test query() implementation
    auto start_query_date = system_clock::now();
    auto end_query_date = system_clock::now() + days{1};
    auto query_value = tse.query("AAPL",start_query_date, end_query_date);

    auto total_query_time = duration_cast<std::chrono::milliseconds>(system_clock::now() - start_query_date);
    std::cout<< "Total query time: " << total_query_time
    << "; per event: " << duration_cast<nanoseconds>(total_query_time)/query_value.size()
    << "; elements retrieved: " << query_value.size()
    <<"\n";

    // Memory diagnostics
    print_memory_diag();
    std::cout<< "MemUsed variable: " << toMB(mem_used) << "\n";
    return 0;
}