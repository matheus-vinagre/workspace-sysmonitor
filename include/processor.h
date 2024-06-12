#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  explicit Processor(const std::vector<std::string>& data);

  float Utilization();  // TODO: See src/processor.cpp
  float StringToFloat();

    // TODO: Declare any necessary private members
private:
  unsigned long long user_, nice_, system_, idle_, iowait_, irq_,
                    softirq_, steal_, guest_, guest_nice_;
};
class PrevProcessor {
 public:
  PrevProcessor() :
          userTemp_(0),
          niceTemp_(0),
          systemTemp_(0),
          idleTemp_(0),
          iowaitTemp_(0),
          irqTemp_(0),
          softirqTemp_(0),
          stealTemp_(0),
          guestTemp_(0),
          guest_niceTemp_(0),
          systemAlltimeTemp_(0),
          idleAlltimeTemp_(0),
          totalPeriod_(0),
          userPeriod_(0),
          nicePeriod_(0),
          systemPeriod_(0),
          idlePeriod_(0),
          iowaitPeriod_(0),
          irqPeriod_(0),
          softirqPeriod_(0),
          stealPeriod_(0),
          guestPeriod_(0),
          guest_nicePeriod_(0),
          systemAlltimePeriod_(0),
          idleAlltimePeriod_(0),
          totalTemp_(0)
  {}

  unsigned long long& user_temp();
  void set_user_temp(unsigned long long user_temp);
  unsigned long long& nice_temp();
  void set_nice_temp(unsigned long long nice_temp);
  unsigned long long& system_temp();
  void set_system_temp(unsigned long long system_temp);
  unsigned long long& idle_temp();
  void set_idle_temp(unsigned long long idle_temp);
  unsigned long long& iowait_temp();
  void set_iowait_temp(unsigned long long iowait_temp);
  unsigned long long& irq_temp();
  void set_irq_temp(unsigned long long irq_temp);
  unsigned long long& softirq_temp();
  void set_softirq_temp(unsigned long long softirq_temp);
  unsigned long long& steal_temp();
  void set_steal_temp(unsigned long long steal_temp);
  unsigned long long& guest_temp();
  void set_guest_temp(unsigned long long guest_temp);
  unsigned long long& guest_nice_temp();
  void set_guest_nice_temp(unsigned long long guest_nice_temp);
  unsigned long long& system_alltime_temp();
  void set_system_alltime_temp(unsigned long long system_alltime_temp);
  unsigned long long& idle_alltime_temp();
  void set_idle_alltime_temp(unsigned long long idle_alltime_temp);
  unsigned long long& total_period();
  void set_total_period(unsigned long long total_period);
  unsigned long long& user_period();
  void set_user_period(unsigned long long user_period);
  unsigned long long& nice_period();
  void set_nice_period(unsigned long long nice_period);
  unsigned long long& system_period();
  void set_system_period(unsigned long long system_period);
  unsigned long long& idle_period();
  void set_idle_period(unsigned long long idle_period);
  unsigned long long& iowait_period();
  void set_iowait_period(unsigned long long iowait_period);
  unsigned long long& irq_period();
  void set_irq_period(unsigned long long irq_period);
  unsigned long long& softirq_period();
  void set_softirq_period(unsigned long long softirq_period);
  unsigned long long& steal_period();
  void set_steal_period(unsigned long long steal_period);
  unsigned long long& guest_period();
  void set_guest_period(unsigned long long guest_period);
  unsigned long long& guest_nice_period();
  void set_guest_nice_period(unsigned long long guest_nice_period);
  unsigned long long& system_alltime_period();
  void set_system_alltime_period(unsigned long long system_alltime_period);
  unsigned long long& idle_alltime_period();
  void set_idle_alltime_period(unsigned long long idle_alltime_period);
  unsigned long long& total_temp();
  void set_total_temp(unsigned long long total_temp);

 private:
  unsigned long long userTemp_, niceTemp_, systemTemp_, idleTemp_,
      iowaitTemp_, irqTemp_, softirqTemp_, stealTemp_, guestTemp_,
      guest_niceTemp_, systemAlltimeTemp_, idleAlltimeTemp_, totalPeriod_;
  unsigned long long userPeriod_, nicePeriod_, systemPeriod_, idlePeriod_,
      iowaitPeriod_, irqPeriod_, softirqPeriod_, stealPeriod_, guestPeriod_,
      guest_nicePeriod_, systemAlltimePeriod_, idleAlltimePeriod_, totalTemp_;
};
#endif