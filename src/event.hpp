#ifndef __EVENT_H__
#define __EVENT_H__

#include "controller.hpp"

typedef enum eventType { ARRIVE, DEPARTURE } eventType;
/**
 * @brief Class Event
 *
 * The Event class represents the event that happens inside the digital
 * transmission system or computer network of the Simulator in terms of
 * clients requesting to establish connections (thus needing resources) known as
 * an "arrival" or closing the connection thereafter (thus freeing resources)
 * known as a "departure".
 *
 * Note that, as the Simulator uses Event Based Simulation, this class'
 * implementation is a key-piece for the simulator's execution.
 *
 * The Event class contains 3 attributes: an EventType variable which describes
 * the kind of Event taking two possible values: ARRIVE or DEPARTURE, the Id
 * (identifier) of the Connection regarding the current Event represented as a
 * long long, and the time at which the current Event has occurred represented
 * as a double.
 *
 * The Event class contains two main methods for setting it's variables' values,
 * both being exclusively constructors: one (void constructor) for using default
 * initialized values and other for receiving their values as arguments. Each
 * attribute has it's own getter.
 */
class Event {
 public:
  /**
   * @brief Constructs a new Event object with default values for it's
   * attributes: (Event Type: ARRIVE, Id Connection: -1, Time: -1)
   */
  Event(void);
  /**
   * @brief Constructs a new Event object setting it's attributes using the
   * values given through it's arguments: (Event Type: type, Id Connection:
   * idConnection, Time: time)
   *
   * @param type (eventType): the type of current Event object, whether it's
   * ARRIVE or DEPARTURE.
   *
   * @param time (double): the actual time at which the current Event has
   * occured.
   *
   * @param idConnection (long long): Connection Identifier (Id).
   */
  Event(eventType type, double time, long long idConnection);

  /**
   * @brief Delete the Event object.
   */

  ~Event();

  /**
   * @brief Gets the type attribute of the Event object. This represents the
   * kind of Event of the current Event object.
   *
   * @return (eventType): the type of Event (ARRIVE or DEPARTURE).
   */

  eventType getType();

  /**
   * @brief Gets the time attribute of the Event object. This represents the
   * time at which the current Event object has occurred.
   *
   * @return (double): the actual time at which the Event has occurred.
   */

  double getTime();

  /**
   * @brief Gets the idConnection attribute of the Event object. This represents
   * the Identifier (Id) of the Connection concerning the current Event.
   *
   * @return (long long): the identifier of the Connection related to the Event.
   */

  long long getIdConnection();

 private:
  eventType type;
  double time;
  long long idConnection;
};

#endif