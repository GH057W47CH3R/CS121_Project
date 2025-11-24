#include <iostream>
#include "core/record_array.hpp"

void RecordArray::delete_record(std::uint32_t index) {
    if (index >= size) {
        std::cerr << "Invalid index\n";
        return;
    }
    
    data[index] = data[size - 1] ;   
    size--;
}
