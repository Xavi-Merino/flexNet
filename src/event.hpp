#ifndef __EVENT_H__
#define __EVENT_H__

// #include "simulator.hpp"
#include "controller.hpp"

/**
 * @brief   Class event
 *
 * This class contains the event that happens in the network of nodes
 */

typedef enum eventType { ARRIVE, DEPARTURE } eventType;
class Event {
 public:
  /**
   * @brief    Constructor of empyy event
   */
  Event(void);
  /**
   * @brief    Constructor of the event with the data types: eventType, double,
   * long long
   */
  Event(eventType type, double time, long long idConnection);

  /**
   * @brief    Delete the Event object
   */

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