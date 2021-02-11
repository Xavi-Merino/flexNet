#ifndef __EVENT_H__
#define __EVENT_H__

// #include "simulator.hpp"
#include "controller.hpp"

typedef enum eventType { ARRIVE, DEPARTURE } eventType;
/**
 * @brief Class event
 *
 * This class contains the event that happens in the network of nodes
 */
class Event {
 public:
  /**
   * @brief Constructor of empyy event
   */
  Event(void);
  /**
   * @brief Constructor of the event with the data types: eventType, double,
   * long long
   *
   * @param type type of event, it can be an ARRIVE or DEPARTURE
   *
   * @param time event execution time
   *
   * @param idConnection connection identifier
   */
  Event(eventType type, double time, long long idConnection);

  /**
   * @brief Delete the Event object
   */

  ~Event();

  /**
   * @brief Getter of type of the private param event
   *
   * @return enum ARRIVE or DEPARTURE
   */

  eventType getType();

  /**
   * @brief Getter of private param time
   *
   * @return Param time, it is type is double
   */

  double getTime();

  /**
   * @brief Getter of private param idConnection
   *
   * @return id of the connection, it is type is long long
   */

  long long getIdConnection();

 private:
  eventType type;
  double time;
  long long idConnection;
};

#endif