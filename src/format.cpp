#include <string>
#include <iomanip>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) {
    int hours = (int)seconds /3600;
    int minutes = (int)(seconds - hours*3600)/60;
    int rem_seconds = (int)(seconds - hours*3600 - minutes*60);
    auto time_padding = [](string time_str) {return time_str.length() == 1? '0'+time_str : time_str;};
    string time_formatted(std::to_string(hours)+':'+time_padding(std::to_string(minutes))+':'+time_padding(std::to_string(rem_seconds)));
    return time_formatted; 
    }