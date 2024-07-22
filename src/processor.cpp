#include "processor.h"
#include <string>
#include <vector>
#include <filesystem>
#include <linux_parser.h>
using std::stoull;
using std::string;
using std::to_string;
using std::vector;

Processor::Processor(const std::vector<std::string>& data)
    : user_(stoull(data[0])),
      nice_(stoull(data[1])),
      system_(stoull(data[2])),
      idle_(stoull(data[3])),
      iowait_(stoull(data[4])),
      irq_(stoull(data[5])),
      softirq_(stoull(data[6])),
      steal_(stoull(data[7])),
      guest_(stoull(data[8])),
      guest_nice_(stoull(data[9])),
      cpunumber_(stoull(data[10]))
      {}

template <typename Type>
  Type Sub(Type a, Type b) { return (a > b ) ? ( a - b ): 0; }


// x TODO: Return the aggregate CPU utilization
float Processor::Utilization() {

  unsigned long long usertime = user_ - guest_;
  unsigned long long nicetime = nice_ - guest_nice_;
  unsigned long long idleAlltime = idle_ + iowait_;
  unsigned long long systemAlltime = system_ + irq_ + softirq_;
  unsigned long long int virtalltime = guest_ + guest_nice_;
  unsigned long long totaltime =
      usertime + nicetime + systemAlltime + idleAlltime + steal_ + virtalltime;

  vector<PrevProcessor> prevProcessor = LinuxParser::prevProcessor;

  prevProcessor[cpunumber_].set_user_period(Sub(usertime, prevProcessor[cpunumber_].user_temp()));
  prevProcessor[cpunumber_].set_nice_period(Sub(nicetime, prevProcessor[cpunumber_].nice_temp()));
  prevProcessor[cpunumber_].set_system_period(Sub(system_, prevProcessor[cpunumber_].system_temp()));
  prevProcessor[cpunumber_].set_system_alltime_period(Sub(systemAlltime, prevProcessor[cpunumber_].system_alltime_temp()));
  prevProcessor[cpunumber_].set_idle_alltime_period(Sub(idleAlltime, prevProcessor[cpunumber_].idle_alltime_temp()));
  prevProcessor[cpunumber_].set_idle_period(Sub(idle_, prevProcessor[cpunumber_].idle_temp()));
  prevProcessor[cpunumber_].set_iowait_period(Sub(iowait_, prevProcessor[cpunumber_].iowait_temp()));
  prevProcessor[cpunumber_].set_irq_period(Sub(irq_, prevProcessor[cpunumber_].irq_temp()));
  prevProcessor[cpunumber_].set_softirq_period(Sub(softirq_, prevProcessor[cpunumber_].softirq_temp()));
  prevProcessor[cpunumber_].set_steal_period(Sub(steal_, prevProcessor[cpunumber_].steal_temp()));
  prevProcessor[cpunumber_].set_guest_period(Sub(virtalltime, prevProcessor[cpunumber_].guest_temp()));
  prevProcessor[cpunumber_].set_total_period(Sub(totaltime, prevProcessor[cpunumber_].total_temp()));

  prevProcessor[cpunumber_].set_user_temp(usertime);
  prevProcessor[cpunumber_].set_nice_temp(nicetime);
  prevProcessor[cpunumber_].set_system_temp(system_);
  prevProcessor[cpunumber_].set_system_alltime_temp(systemAlltime);
  prevProcessor[cpunumber_].set_idle_alltime_temp(idleAlltime);
  prevProcessor[cpunumber_].set_idle_temp(idle_);
  prevProcessor[cpunumber_].set_iowait_temp(iowait_);
  prevProcessor[cpunumber_].set_irq_temp(irq_);
  prevProcessor[cpunumber_].set_softirq_temp(softirq_);
  prevProcessor[cpunumber_].set_steal_temp(steal_);
  prevProcessor[cpunumber_].set_guest_temp(virtalltime);
  prevProcessor[cpunumber_].set_total_temp(totaltime);

  float utilization = (prevProcessor[cpunumber_].total_period() - prevProcessor[cpunumber_].idle_alltime_period())
                  / 1000.0;
  LinuxParser::prevProcessor[cpunumber_] = prevProcessor[cpunumber_];
  return utilization;
}


//PrevProcessor Getter/Setter
unsigned long long& PrevProcessor::user_temp() { return userTemp_; }
void PrevProcessor::set_user_temp(unsigned long long user_temp) {
  userTemp_ = user_temp;
}
unsigned long long& PrevProcessor::nice_temp() { return niceTemp_; }
void PrevProcessor::set_nice_temp(unsigned long long nice_temp) {
  niceTemp_ = nice_temp;
}
unsigned long long& PrevProcessor::system_temp() { return systemTemp_; }
void PrevProcessor::set_system_temp(unsigned long long system_temp) {
  systemTemp_ = system_temp;
}
unsigned long long& PrevProcessor::idle_temp() { return idleTemp_; }
void PrevProcessor::set_idle_temp(unsigned long long idle_temp) {
  idleTemp_ = idle_temp;
}
unsigned long long& PrevProcessor::iowait_temp() { return iowaitTemp_; }
void PrevProcessor::set_iowait_temp(unsigned long long iowait_temp) {
  iowaitTemp_ = iowait_temp;
}
unsigned long long& PrevProcessor::irq_temp() { return irqTemp_; }
void PrevProcessor::set_irq_temp(unsigned long long irq_temp) {
  irqTemp_ = irq_temp;
}
unsigned long long& PrevProcessor::softirq_temp() { return softirqTemp_; }
void PrevProcessor::set_softirq_temp(unsigned long long softirq_temp) {
  softirqTemp_ = softirq_temp;
}
unsigned long long& PrevProcessor::steal_temp() { return stealTemp_; }
void PrevProcessor::set_steal_temp(unsigned long long steal_temp) {
  stealTemp_ = steal_temp;
}
unsigned long long& PrevProcessor::guest_temp() { return guestTemp_; }
void PrevProcessor::set_guest_temp(unsigned long long guest_temp) {
  guestTemp_ = guest_temp;
}
unsigned long long& PrevProcessor::guest_nice_temp() { return guest_niceTemp_; }
void PrevProcessor::set_guest_nice_temp(unsigned long long guest_nice_temp) {
  guest_niceTemp_ = guest_nice_temp;
}
unsigned long long& PrevProcessor::system_alltime_temp() {
  return systemAlltimeTemp_;
}
void PrevProcessor::set_system_alltime_temp(
    unsigned long long system_alltime_temp) {
  systemAlltimeTemp_ = system_alltime_temp;
}
unsigned long long& PrevProcessor::idle_alltime_temp() {
  return idleAlltimeTemp_;
}
void PrevProcessor::set_idle_alltime_temp(
    unsigned long long idle_alltime_temp) {
  idleAlltimeTemp_ = idle_alltime_temp;
}
unsigned long long& PrevProcessor::total_period() { return totalPeriod_; }
void PrevProcessor::set_total_period(unsigned long long total_period) {
  totalPeriod_ = total_period;
}
unsigned long long& PrevProcessor::user_period() { return userPeriod_; }
void PrevProcessor::set_user_period(unsigned long long user_period) {
  userPeriod_ = user_period;
}
unsigned long long& PrevProcessor::nice_period() { return nicePeriod_; }
void PrevProcessor::set_nice_period(unsigned long long nice_period) {
  nicePeriod_ = nice_period;
}
unsigned long long& PrevProcessor::system_period() { return systemPeriod_; }
void PrevProcessor::set_system_period(unsigned long long system_period) {
  systemPeriod_ = system_period;
}
unsigned long long& PrevProcessor::idle_period() { return idlePeriod_; }
void PrevProcessor::set_idle_period(unsigned long long idle_period) {
  idlePeriod_ = idle_period;
}
unsigned long long& PrevProcessor::iowait_period() { return iowaitPeriod_; }
void PrevProcessor::set_iowait_period(unsigned long long iowait_period) {
  iowaitPeriod_ = iowait_period;
}
unsigned long long& PrevProcessor::irq_period() { return irqPeriod_; }
void PrevProcessor::set_irq_period(unsigned long long irq_period) {
  irqPeriod_ = irq_period;
}
unsigned long long& PrevProcessor::softirq_period() { return softirqPeriod_; }
void PrevProcessor::set_softirq_period(unsigned long long softirq_period) {
  softirqPeriod_ = softirq_period;
}
unsigned long long& PrevProcessor::steal_period() { return stealPeriod_; }
void PrevProcessor::set_steal_period(unsigned long long steal_period) {
  stealPeriod_ = steal_period;
}
unsigned long long& PrevProcessor::guest_period() { return guestPeriod_; }
void PrevProcessor::set_guest_period(unsigned long long guest_period) {
  guestPeriod_ = guest_period;
}
unsigned long long& PrevProcessor::guest_nice_period() {
  return guest_nicePeriod_;
}
void PrevProcessor::set_guest_nice_period(
    unsigned long long guest_nice_period) {
  guest_nicePeriod_ = guest_nice_period;
}
unsigned long long& PrevProcessor::system_alltime_period() {
  return systemAlltimePeriod_;
}
void PrevProcessor::set_system_alltime_period(
    unsigned long long system_alltime_period) {
  systemAlltimePeriod_ = system_alltime_period;
}
unsigned long long& PrevProcessor::idle_alltime_period() {
  return idleAlltimePeriod_;
}
void PrevProcessor::set_idle_alltime_period(
    unsigned long long idle_alltime_period) {
  idleAlltimePeriod_ = idle_alltime_period;
}
unsigned long long& PrevProcessor::total_temp() { return totalTemp_; }
void PrevProcessor::set_total_temp(unsigned long long total_temp) {
  totalTemp_ = total_temp;
}

