#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

class Processor {
 public:
    Processor(){
        for(int i=0; i<10;i++) {prev_stats.push_back(0); }
        ReadStats();
    }
    float Utilization();  // TODO: See src/processor.cpp
    void InsertPrevStats();
    void ReadStats();

  // TODO: Declare any necessary private members
 private:
    std::vector<int> prev_stats;
    std::vector<int> curr_stats;
};

#endif