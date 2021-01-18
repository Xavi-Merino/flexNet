#ifndef __EVENT_H__
#define __EVENT_H__

// #include "simulator.hpp"
#include "controller.hpp"

typedef enum eventType { ARRIVE, DEPARTURE } eventType;
class Event {
 public:
  Event(void);
  Event(eventType type, double time, int idConnection);

  ~Event();

  eventType getType();

  double getTime();

  long long getIdConnection();

 private:
  eventType type;
  double time;
  long long idConnection;
};

#endif