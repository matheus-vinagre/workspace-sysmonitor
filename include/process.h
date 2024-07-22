#ifndef PROCESS_H
#define PROCESS_H
#include <string>

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  long& last_totaltime();
  void set_last_totaltime(long last_totaltime);
  long& last_uptime();
  void set_last_uptime(long last_uptime);
  explicit Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long UpTime();                           // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 // x TODO: Declare any necessary private members
private:
  int pid_;
  long last_totaltime_ = 0;
  long last_uptime_ = 0;
};

#endif