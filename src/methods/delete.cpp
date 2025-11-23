#include <iostream>
#include "core/record_array.hpp"

void delete_record(RecordArray& arr, std::uint32_t index) {
    if (index == 0 || index > arr.count) {
        std::cerr << "Invalid index\n";
        return;
    }

    // Allocate new array with one less slot
    Record* newArr = new Record[arr.count - 1];

    // Copy all records except the one to delete
    for (std::uint32_t i = 0, j = 0; i < arr.count; i++) {
        if (i == index - 1) continue; // skip the one to delete
        newArr[j++] = arr.data[i];
    }

    // Delete old array
    delete[] arr.data;

    // Update RecordArray
    arr.data = newArr;
    arr.count--;
}