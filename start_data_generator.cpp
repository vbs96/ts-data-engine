//
// Created by Bogdan Vitelaru on 07.04.2026.
//

#include "DataGenerator/DataGenerator.h"
#include <iostream>
#include <mach/mach.h>

static auto toMB = [](uint64_t bytes) {
    return bytes / (1024.0 * 1024.0);
};

void print_memory_diag() {
    task_basic_info t_info;
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
    DataGenerator dg;
    dg.start();

    return 0;
}