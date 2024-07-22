#include "system.h"
#include <linux_parser.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// x TODO: Return the system's CPU//
vector<Processor>& System::Cpu() {
  vector<vector<string>> cpu = LinuxParser::CpuUtilization();
  vector<Processor> cpu_temp;
  cpu_temp.reserve(LinuxParser::cpuN);
  for(const auto& core : cpu) {
    Processor processor(core);
    cpu_temp.emplace_back(processor);
  }
  cpu_ = cpu_temp;
  cpu_temp.clear();
  return cpu_;
}

// x TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  vector<Process> temp_processes;
  temp_processes.reserve(pids.size());
  for(auto pid : pids) {
    Process process(pid);
    temp_processes.emplace_back(process);
  }
  processes_.swap(temp_processes);
  temp_processes.clear();
  std::sort(processes_.begin(), processes_.end());
  return processes_;
}

// x TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// x TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// x TODO: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// X TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// X TODO: Return the total number of processes on the system
int System::TotalProcesses() { return processes_.size(); }

// X TODO: Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }