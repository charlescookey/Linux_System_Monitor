#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
  std::string line ,key;
  float value;
  float memTotal{0};
  float memFree{0};
  std::ifstream memstream(kProcDirectory + kMeminfoFilename);
  if (memstream.is_open()) {
    while (std::getline(memstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = value;
        }
        if (key == "MemFree:") {
          memFree = value;
          break;
        }
      }
    }
  }
  return 1 - (memFree / memTotal);
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long upTime{0};
  long idleTime{0};
  std::string line;
  std::ifstream upTstream(kProcDirectory + kUptimeFilename);
  if (upTstream.is_open()) {
    std::getline(upTstream, line);
    std::istringstream linestream(line);
    linestream>> upTime >> idleTime;
  }
  return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
float LinuxParser::ActiveJiffies(int pid) {
  vector<std::string> list{ };
  std::string id = std::to_string(pid);
  std::string line,temp;
  float cpu_usage{0};
  float utime, stime, cutime, cstime,total_time,starttime;
  std::ifstream Activestream(kProcDirectory + id + kStatFilename);
  if (Activestream.is_open()) {
    
    std::getline(Activestream, line);
    std::istringstream linestream(line);
    while(linestream>>temp){
      list.push_back(temp);
    };
    
    if(list.size()<16 )return 0;
    if(list[13] == "" || list[14] == "")return 0;
    if(list[15] == "" || list[16] == "")return 0;
    
    utime = std::stol(list[13]);
    stime  = std::stol(list[14]);

    cutime = std::stol(list[15]);
    cstime  = std::stol(list[16]);

  
    total_time = utime + stime  + cutime + cstime;
    cpu_usage = total_time / sysconf(_SC_CLK_TCK);

    starttime = UpTime() - UpTime(pid);
    if(starttime > 0)
    cpu_usage  /= starttime;
  };
  return cpu_usage;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
   vector<string> answer;
  std::string line,cpu;
  std::ifstream upTstream(kProcDirectory + kStatFilename);
  if (upTstream.is_open()) {
    std::getline(upTstream, line);
    std::istringstream linestream(line);
    linestream>>cpu;
    if(cpu == "cpu"){
      while(linestream >> cpu){
        answer.push_back(cpu);
      };
    };
  }
  return answer;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
   std::string line,key;
  int answer{0};
  std::ifstream Tstream(kProcDirectory + kStatFilename);
  if (Tstream.is_open()) {
    while (std::getline(Tstream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> answer;
        break;
      }
    }
  }
  return answer;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line,key;
  int answer{0};
  std::ifstream Tstream(kProcDirectory + kStatFilename);
  if (Tstream.is_open()) {
    while (std::getline(Tstream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> answer;
        break;
      }
    }
  }
  return answer;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  std::string id = std::to_string(pid);
  std::string line;
  std::ifstream Activestream(kProcDirectory + id + kCmdlineFilename);
  if (Activestream.is_open()) {
    std::getline(Activestream, line);
 }
 return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  std::string line ,key,value;
  std::string id = std::to_string(pid);
  std::ifstream Ramstream(kProcDirectory +id + kStatusFilename);
  if (Ramstream.is_open()) {
    while (std::getline(Ramstream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize:") {
        linestream>>value;
        break;
      }
    }
  }
  if(value == "")return "";
  long valued = std::stol(value);
  if(valued>0)valued/=1024;
  value = std::to_string(valued);
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  std::string line ,key,value;
  std::string id = std::to_string(pid);
  std::ifstream Ramstream(kProcDirectory +id + kStatusFilename);
  if (Ramstream.is_open()) {
    while (std::getline(Ramstream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid:") {
        linestream>>value;
        break;
      }
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string key;
  string value;
  string uid = Uid(pid);
  char x;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >>value) {
        if (value == uid) {
          return key;
        }
      }
    }
  }
  return key;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  vector<std::string> list{ };
  std::string id = std::to_string(pid);
  std::string line,temp;
  long answer{0};
  std::ifstream Activestream(kProcDirectory + id + kStatFilename);
  if (Activestream.is_open()) {
    std::getline(Activestream, line);
    std::istringstream linestream(line);
    while(linestream>>temp){
      list.push_back(temp);
    };
    if(list[21]!="")
    answer = std::stol(list[21]);
  };
  int x = sysconf(_SC_CLK_TCK);
  if(x!=0)
  answer/=x;
  return answer;
}