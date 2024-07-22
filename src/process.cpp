#include "process.h"

#include <linux_parser.h>
#include <unistd.h>
#include <cctype>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

template <typename Type>
Type Sub(Type a, Type b) {
  return (a > b) ? (a - b) : 0;
}

Process::Process(int pid) : pid_(pid) {}

long& Process::last_totaltime() { return last_totaltime_; }
void Process::set_last_totaltime(long last_totaltime) {
  last_totaltime_ = last_totaltime;
}
long& Process::last_uptime() { return last_uptime_; }
void Process::set_last_uptime(long last_uptime) { last_uptime_ = last_uptime; }

// x TODO: Return this process's ID
int Process::Pid() { return pid_; }

// x TODO: Return this process's CPU utilization

double calculateCpu(const long& delta_totaltime, const long& delta_uptime) {
  long clockTicks = sysconf(_SC_CLK_TCK);
  if(delta_uptime == 0) {
    return 0;
  }
  double dd_delta_totaltime = delta_totaltime * 10.0;
  double cpuPercent = ((dd_delta_totaltime  / clockTicks) / delta_uptime);
  return cpuPercent;
}
vector<long> Prev(int pid, long totaltime, long uptime) {
  if(LinuxParser::prevProcesses.empty()) {
    Process prevprocess(pid);
    prevprocess.set_last_totaltime(totaltime);
    prevprocess.set_last_uptime(uptime);
    LinuxParser::prevProcesses.push_back(prevprocess);
    vector<long> prev({0 ,0});
    return prev;
  }
  for(auto prev : LinuxParser::prevProcesses ) {
    if(prev.Pid() == pid) {
      long last_totaltime = prev.last_totaltime();
      long last_uptime = prev.last_uptime();
      prev.set_last_totaltime(totaltime);
      prev.set_last_uptime(uptime);
      vector<long> prevProcess({last_totaltime, last_uptime});
      return prevProcess;
    }
  }
  Process prevprocess(pid);
  prevprocess.set_last_totaltime(totaltime);
  prevprocess.set_last_uptime(uptime);
  LinuxParser::prevProcesses.push_back(prevprocess);
  vector<long> prev({0 ,0});
  return prev;
}

float Process::CpuUtilization() {

  vector<long> datas = LinuxParser::ActiveJiffies(pid_);
  int pid = pid_;
  long totaltime = datas[0] + datas[1] + datas[2] + datas[3];
  long uptime = LinuxParser::UpTime(pid_);
  vector<long> prevProcess = Prev(pid,totaltime, uptime);
  long d_totaltime = Sub(totaltime, prevProcess[0]);
  long d_uptime = Sub(uptime,prevProcess[1]);
  double cpuUsage = calculateCpu(d_totaltime, d_uptime);
  return cpuUsage;
}
// x TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// x TODO: Return this process's memory utilization
string Process::Ram() {return LinuxParser::Ram(pid_);}

// x TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(LinuxParser::Uid(pid_)); }

// x TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// x TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& other) const {
  float a = std::stof(LinuxParser::Ram(pid_));
  float b = std::stof(LinuxParser::Ram(other.pid_));
  return a > b;
}