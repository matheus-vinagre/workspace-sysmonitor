#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Parse everything I need from proc/stat
// Instead of accessing the file multiple times.
void LinuxParser::ProcStatParsin() {
  vector<string> cpuData(10);
  string s_totalProcesses, s_runningProcesses;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line;
  int cpuCount = 0;
  while (std::getline(filestream, line)) {
    if (line.substr(0, 3) == "cpu") {
      cpuCount++;
      std::istringstream linestream(line);
      linestream.ignore(256, ' '); // Skip the "cpu" part
      linestream >> cpuData[kUser_] >> cpuData[kNice_] >> cpuData[kSystem_]
                 >> cpuData[kIdle_] >> cpuData[kIOwait_] >> cpuData[kIRQ_]
                 >> cpuData[kSoftIRQ_] >> cpuData[kSteal_] >> cpuData[kGuest_]
                 >> cpuData[kGuestNice_];
      cpuUtilization.push_back(cpuData);
    }
    if (line.substr(0, 9) == "processes") {
      std::istringstream linestream(line);
      linestream.ignore(256, ' ');
      linestream >> s_totalProcesses;
      totalProcesses = std::stoi(s_totalProcesses);
    }
    if (line.substr(0, 13) == "procs_running") {
      std::istringstream linestream(line);
      linestream.ignore(256, ' ');
      linestream >> s_runningProcesses;
      runningProcesses = std::stoi(s_runningProcesses);
    }
  }
  cpuN = cpuCount;
}

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
/*vector<int> LinuxParser::Pids() {
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
*/
// New code filesystem
bool is_digits(string& str)
{
  return all_of(str.begin(), str.end(), ::isdigit);
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  std::filesystem::path dirpath = kProcDirectory;
  for(const auto& entry : std::filesystem::directory_iterator(dirpath)) {
    if(entry.is_directory()) {
      string dirName = entry.path().filename();
      if(is_digits(dirName)){
        int pid = std::stoi(dirName);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

//                        TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  enum class Choice{A, T, F, B, C, H, R};
    std::map<string, Choice> stringToEnumMap
        = { {"bugKill", Choice::A},
            {"MemTotal:", Choice::T},
            {"MemFree:", Choice::F},
            {"Buffers:", Choice::B},
            {"Cached:", Choice::C},
            {"Shmem:", Choice::H},
            {"SReclaimable:", Choice::R}};
  unsigned long long int memTotal, memFree, memBuffer, memCached, sHmem, sReclaimable ;
  string key, sValue, kB;
  string line;
  Choice choice;
  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> sValue >> kB) {
        choice = stringToEnumMap[key];
        switch (choice) {
        case Choice::T:
          memTotal = stof(sValue);
          break;
        case Choice::F:
          memFree = stof(sValue);
          break;
        case Choice::B:
          memBuffer = stof(sValue);
          break;
        case Choice::C:
          memCached = stof(sValue);
          break;
        case Choice::H:
            sHmem = stof(sValue);
        break;
        case Choice::R:
            sReclaimable = stof(sValue);
        break;
        default:
          break;
        }
      }
    }
  }
  unsigned long long int memUsed = memTotal - memFree;
  unsigned long long int memCached_T = memCached + sReclaimable - sHmem;
  unsigned long long int memUtilization = memUsed - (memBuffer + memCached_T);
  float per = (100 * memUtilization) /memTotal;
  return per/100;
}

//                         TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kUptimeFilename);
  long upTime;
  if (filestream.is_open()) {
    string line, s_upTime, s_idleTime;
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> s_upTime >> s_idleTime;
    upTime = std::stol(s_upTime);
  }
  return upTime;
}

//                         TODO: Read and return the number of jiffies for the system
// Not Used! I use ProcStatParsin() instead.
// And caculate the rest inside Processor class.
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
vector<long> LinuxParser::ActiveJiffies(int pid) {
  string spid = to_string(pid);
  long utime, stime, cutime, cstime, starttime;
  std::ifstream filestream(LinuxParser::kProcDirectory + spid +
                           LinuxParser::kStatFilename);
  string line;
  string data;
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    for(int i=0; i<22;++i ) {
      linestream >> data;
      switch (i) {
        case 13:
          utime = std::stol(data);
        break;
        case 14:
          stime = std::stol(data);
        break;
        case 15:
          cutime = std::stol(data);
        break;
        case 16:
          cstime = std::stol(data);
        break;
        case 21:
          starttime = std::stol(data);
        break;
        default:
          break;
      }
    }
  }
  vector<long> datas({utime, stime, cutime, cstime, starttime});

  return datas;
}

//                         TODO: Read and return the number of active jiffies for the system
// Not Used! I use ProcStatParsin() instead.
// And caculate the rest inside Processor class.
long LinuxParser::ActiveJiffies() { return 0; }

//                         TODO: Read and return the number of idle jiffies for the system
// Not Used! I use ProcStatParsin() instead.
// And caculate the rest inside Processor class.
long LinuxParser::IdleJiffies() { return 0; }

//                         TODO: Read and return CPU utilization
vector<vector<string>> LinuxParser::CpuUtilization() {
  vector<vector<string>> cpuUtilizationTemp = cpuUtilization;
  cpuUtilization.clear();
  return cpuUtilizationTemp;
}

//                         TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return totalProcesses;}

//                        TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return runningProcesses;}

//                    TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string spid = to_string(pid);
  std::ifstream filestream(LinuxParser::kProcDirectory + spid +
                           LinuxParser::kCmdlineFilename);
  string line;
  std::getline(filestream, line);
  return line;
}

//TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string spid = to_string(pid);
  string ram, kb;
  std::ifstream filestream(LinuxParser::kProcDirectory + spid +
                           LinuxParser::kStatusFilename);
  string line;
  while (std::getline(filestream, line)) {
    if (line.substr(0, 6) == "VmRSS:") { // Used VmRSS instead of VmSize
      std::istringstream linestream(line.substr(7));
      linestream >> ram >> kb;
      long double kram = std::stoull(ram);
      long double mram = kram /(1024);
      std::ostringstream oss;
      oss << std::fixed << std::setprecision(2) << mram;
      string sram = oss.str();
      return (sram);
    }
  }
  return  "0";
}

//                    TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string spid = to_string(pid);
  string uid, uid2 ,uid3, uid4;
  std::ifstream filestream(LinuxParser::kProcDirectory + spid +
                           LinuxParser::kStatusFilename);
  string line;
    while (std::getline(filestream, line)) {
      if (line.substr(0, 4) == "Uid:") {
        std::istringstream linestream(line.substr(5));
        linestream >> uid >> uid2 >> uid3 >> uid4;
    }
  }
  return uid;
}

//                      TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(const string& uid) {
  string user, psswd, key, gid, gecos, dir, shell;
  std::ifstream filestream(kPasswordPath);
  string line;
  while (std::getline(filestream, line)) {
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while (linestream >> user >> psswd >> key >> gid >> gecos >> dir >> shell) {
      if (key == uid) {
        return user;
      }
    }
  }
  return "error";
}

//TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function

long LinuxParser::UpTime(int pid) {
  string spid = to_string(pid);
  string data;
  std::ifstream filestream(LinuxParser::kProcDirectory + spid +
                           LinuxParser::kStatFilename);
  string line;
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    for(int i=0; i<22;++i ) {
      linestream >> data;
    }
  }
  long p_uptime = std::stol(data) / sysconf(_SC_CLK_TCK);
  long s_uptime = UpTime();
  return s_uptime - p_uptime;
}
