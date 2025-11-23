#include <iostream>
#include <fstream>
#include <filesystem>
#include "core/record_array.hpp"

namespace fs = std::filesystem;

void save_records(const fs::path& p, const RecordArray& arr) {
    std::ofstream out(p);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << p << "\n";
        return;
    }

    //write the number of records as the first line
    out << arr.count << "\n";

    //write each record name on its own line
    for (std::uint32_t i = 0; i < arr.count; i++) {
        out << arr.data[i].name << "\n";
    }

    out.close();
    std::cout << "Records saved to file: " << p << "\n";
}