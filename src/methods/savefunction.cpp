#include <iostream>
#include "core/record_array.hpp"

void RecordArray::save_records(const fs::path& p) {
    std::ofstream out(p);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << p << "\n";
        return;
    }

    out << size << "\n";

    for (std::uint32_t i = 0; i < size; i++) {
        out << data[i].name << "\n";
    }

    out.close(); 
    std::cout << "Records saved to file: " << p << "\n";
}
