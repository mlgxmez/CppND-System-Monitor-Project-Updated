#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    ReadStats();
    int PrevIdle = prev_stats[3] + prev_stats[4];
    int Idle = curr_stats[3] + curr_stats[4];

    int PrevNonIdle = prev_stats[0] + prev_stats[1] + prev_stats[2] + prev_stats[5] + prev_stats[6] + prev_stats[7];
    int NonIdle = curr_stats[0] + curr_stats[1] + curr_stats[2] + curr_stats[5] + curr_stats[6] + curr_stats[7]; // OK
    int PrevTotal = PrevIdle + PrevNonIdle;
    int Total = Idle + NonIdle;

    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;
    InsertPrevStats();
    return (totald - idled)/totald; 
    }

void Processor::InsertPrevStats() {
    prev_stats.clear();
    for (auto t: curr_stats){
        prev_stats.push_back(t);
    }
    curr_stats.clear();
}

void Processor::ReadStats() {
    std::string line, word;
    std::ifstream statfile(LinuxParser::kProcDirectory+LinuxParser::kStatFilename);
    if(statfile.is_open()) {
        std::getline(statfile, line);
        std::istringstream mystream(line);
        while(mystream >> word){
            if (word != "cpu"){
                curr_stats.push_back(std::stoi(word));
            }
        }
    }
}