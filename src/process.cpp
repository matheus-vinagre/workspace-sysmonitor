#include "process.h"

#include <linux_parser.h>
#include <unistd.h>

#include <cctype>
#include <sstream>
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

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization

float calculateCpu(long& a, long& b) {

  long clockTicks = sysconf(_SC_CLK_TCK);

  if(b == 0) {
    return 0;
  }
  else {
    return ((a / clockTicks) / b) / 100;
  }
}

float Process::CpuUtilization() {

  vector<long> datas = LinuxParser::ActiveJiffies(pid_);
  long totaltime = datas[0] + datas[1] + datas[2] + datas[3];
  long uptime = LinuxParser::UpTime(pid_);
  //long starttime = datas[4];
  //long deltaP = Sub(totaltime, utime_);
  //long deltaT = Sub(uptime, dtime_);
  //utime_= totaltime;
  //dtime_ = uptime;

 // long clockTicks = sysconf(_SC_CLK_TCK);

  long d_totaltime = Sub(LinuxParser::last_totaltime_, totaltime);

  long d_uptime = Sub(LinuxParser::last_uptime_, uptime );

 // double seconds = d_uptime / clockTicks;

  float cpuUsage = calculateCpu(d_totaltime, d_uptime);
  LinuxParser::last_totaltime_ = totaltime;
  LinuxParser::last_uptime_ = uptime;

  //float cpuP = (deltaP / clockTicks) / deltaT;

  return cpuUsage;

}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() {return LinuxParser::Ram(pid_);}

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(LinuxParser::Uid(pid_)); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& other) const {
  float a = std::stof(LinuxParser::Ram(pid_));
  float b = std::stof(LinuxParser::Ram(other.pid_));

  return a > b;
}