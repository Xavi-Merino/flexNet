#include "event.hpp"
Event::Event(void) {
  this->type = ARRIVE;
  this->time = -1;
  this->idConnection = -1;
}

Event::Event(eventType type, double time, int idConnection) {
  this->type = type;
  this->time = time;
  this->idConnection = idConnection;
}

Event::~Event() {}

eventType Event::getType() { return this->type; }

double Event::getTime() { return this->time; }

long long Event::getIdConnection() { return this->idConnection; }
