
/*
 * A wrapper around the TimerOne library that allows for setting up multiple timers using one hardware timer.
 * Also allows for timers with delays longer than 2^16 ticks
*/

#ifndef TIMER_ONE_MULTI_H
  #define TIMER_ONE_MULTI_H

#include "Arduino.h"
#include<TimerOne.h>

#define MAX_PERIOD 8388608

class TimerEvent
{
  friend class TimerOneMulti;
private:
  TimerEvent(unsigned long period, void (*callback) (void*), bool periodic, void* arg);
  unsigned long delta;
  unsigned long period;
  unsigned long timeRemainingAfterTick;
  bool periodic;
  bool cancelled;
  void (*callback) (void*);
  void* arg;  
  TimerEvent* next;
  
  void add(TimerEvent* event);
  bool cancel(TimerEvent* event);
};

class TimerOneMulti
{
public:
  volatile static bool timerFirstShot;
private:
  TimerEvent* events;
  static TimerOneMulti* singleton;
  
public:
  static TimerOneMulti* getTimerController();
  
  TimerEvent* addEvent(unsigned long period, void (*callback) (void*), bool periodic = false, void* arg = NULL);
  
  bool cancelEvent(TimerEvent* event);
  
  /*
   This is really the tick function. The actual tick function has to be static, so it calls this one with the singleton.
   This one has access to the private members.
   This shouldn't really be used as a public method, it should only be called by tick()
  */
  void advanceTimer();
  

private:
  TimerOneMulti();
  
  //Note, this one assumes interrupts are already dissabled
  void addEvent(TimerEvent* event);
  
  static void tick();
  
};
  
#endif

