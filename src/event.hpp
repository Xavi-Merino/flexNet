#ifndef __EVENT_H__
#define __EVENT_H__

// #include "simulator.hpp"
#include "controller.hpp"

class Event {
 public:
  Event(int eventType, double time, int idConnection);

  ~Event();

  int getEventType();

  double getTime();

  long long getIdConnection();

 private:
  int eventType;
  double time;
  long long idConnection;
};

#endif