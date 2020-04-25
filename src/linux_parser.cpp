#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stoi;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  // Memory utilization = MemTotal - MemFree
  string line;
  string key, value;
  float memtotal = 0.0, memfree = 0.0, buffers = 0.0, cached = 0.0, slab = 0.0;
  std::ifstream memfstream(kProcDirectory+kMeminfoFilename);
  if(memfstream.is_open()){
    while(std::getline(memfstream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      stream >> key >> value;
      if (key == "MemTotal") { memtotal = stof(value); }
      if (key == "MemFree") { memfree = stof(value); }
      if (key == "Buffers") { buffers = stof(value); }
      if (key == "Cached") { cached = stof(value); }
      if (key == "Slab") { slab = stof(value); }
      if (memtotal != 0 && memfree != 0 && buffers != 0 && cached != 0 && slab != 0){ break;}
    };
    return (memtotal - memfree - buffers - cached - slab)/memtotal;
  }
  return 0.0;
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long uptime, idletime;
  std::ifstream uptimeStream(kProcDirectory+kUptimeFilename);
  if(uptimeStream.is_open()){
    std::getline(uptimeStream, line);
    std::istringstream stream(line);
    stream >> uptime >> idletime;
    return uptime;
  }
  return 0; 
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  // Sum all values of cpu
  string line, word;
  int jiffies = 0;
  std::ifstream jfstream(kProcDirectory+kStatFilename);
  if(jfstream.is_open()) {
    std::getline(jfstream, line);
    std::istringstream linestream(line);
    int count = 1;
    while(linestream >> word){
      if (count !=1){
        jiffies += stoi(word);
      }
      count++;
    }
    return jiffies;
  }
  return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
  string word;
  string line;
  int jiffies = 0;
  std::ifstream jfstream(kProcDirectory+to_string(pid)+kStatFilename);
  if(jfstream.is_open()) {
    std::getline(jfstream, line);
    std::istringstream linestream(line);
    int count = 1;
    while(linestream >> word) {
      switch(count) {
        case 14:
          jiffies += stoi(word);
          break;
        case 15:
          jiffies += stoi(word);
          break;
        case 16:
          jiffies += stoi(word);
          break;
        case 17:
          jiffies += stoi(word);
          break;
      }
      count++;
    }
    return jiffies;
  }
  return jiffies; 
  }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line;
  string k0, k1, k2, k3;
  int jiffies = 0;
  std::ifstream ajfstream(kProcDirectory+kStatFilename);
  if(ajfstream.is_open()) {
    std::getline(ajfstream, line);
    std::istringstream linestream(line);
    linestream >> k0 >> k1 >> k2 >> k3;
    jiffies = jiffies + stoi(k1) + stoi(k2) + stoi(k3);
  }
  return jiffies; 
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  string k0, k1, k2, k3, k4;
  int jiffies = 0;
  std::ifstream ajfstream(kProcDirectory+kStatFilename);
  if(ajfstream.is_open()) {
    std::getline(ajfstream, line);
    std::istringstream linestream(line);
    linestream >> k0 >> k1 >> k2 >> k3 >> k4;
    jiffies = stoi(k4);
  }
  return jiffies;  
  }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key, first_value;
  std::ifstream processStream(kProcDirectory+kStatFilename);
  if(processStream.is_open()){
    while(std::getline(processStream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> first_value;
      if (key == "processes"){
        return stoi(first_value);
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key, first_value;
  std::ifstream processStream(kProcDirectory+kStatFilename);
  if(processStream.is_open()){
    while(std::getline(processStream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> first_value;
      if (key == "procs_running"){
        return stoi(first_value);
      }
    }
  }
  return 0;
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { 
  string line;
  std::ifstream cmdfstream(kProcDirectory+to_string(pid)+kCmdlineFilename);
  if(cmdfstream.is_open()){
    std::getline(cmdfstream, line);
    return line;
  }
  return line; 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { 
  string line;
  string key, value;
  std::ifstream statusStream(kProcDirectory+to_string(pid)+kStatusFilename);
  if(statusStream.is_open()){
    while (std::getline(statusStream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream mystream;
      mystream >> key >> value;
      if (key == "VmSize"){

        value = to_string(stof(value)*0.001);
      }
    }
  }
  return value; 
  }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory+'/'+to_string(pid)+kStatusFilename);
  if(filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      while(stream >> key >> value){
        if(key == "Uid"){
          return value;
        }
      }
    }
  }
  return value; 
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { 
  string line;
  string k0, k1, k2;
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if(filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      while(stream >> k0 >> k1 >> k2) {
        if (k2 == uid) {
          return k0;
        }
      }
    }
  }
  return k0; 
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { 
  // Get the 22th field of the file
  string word;
  string line;
  long seconds = 0;
  std::ifstream statfile(kProcDirectory+'/'+to_string(pid)+kStatFilename);
  if(statfile.is_open()) {
    std::getline(statfile, line);
    std::istringstream stream(line);
    int word_ix(0);
    while(stream >> word){
      if (word_ix == 21){
        break;
      }
    }
    seconds = stof(word)/sysconf(_SC_CLK_TCK);
  }
  return seconds; 
  }