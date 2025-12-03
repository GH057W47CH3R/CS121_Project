#include "app_state.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>


//Loads all records from a binary (.bin) file
void AppState::load_from_file(const fs::path &p) {
  //Opens the file for reading in binary
  std::ifstream in(p, std::ios::binary);
  //If the file doesn't exist, this creates an empty file  
  if (!in) {
        std::ofstream out(p, std::ios::binary);
        //Reset internal state to empty
        records_state_ = RecordArray();
        return;
    }

    uint32_t count = 0;
    in.read(reinterpret_cast<char*>(&count), sizeof(count));
    in.read(reinterpret_cast<char*>(&next_id_), sizeof(next_id_));

    //RecordArray function will deserialize data if it exists
    records_state_.deserialize(in, count);
}

//Save all records to a binary (.bin) file
void AppState::save_to_file(const fs::path &p) {
    //Open file for writing(this will overwrite existing file)
    std::ofstream out(p, std::ios::binary);
    //Excetion for if the file fails to open
    if (!out) {
        throw std::runtime_error("Failed to open file for writing: " + p.string());
    }

    uint32_t count = records_state_.size_;
    out.write(reinterpret_cast<char*>(&count), sizeof(count));
    out.write(reinterpret_cast<char*>(&next_id_), sizeof(next_id_));

    //Serialize the RecordArray to file
    records_state_.serialize(out);
    std::cout << "Records saved to file: " << p << "\n";
}
//Adds a new record to the RecordArray
void AppState::add_record_to_state(Record rec){
  records_state_.add_record(rec);
}
//Deletes a record from the RecordArray
void AppState::delete_record_from_state(std::uint32_t index){
  records_state_.delete_record(index);
}

