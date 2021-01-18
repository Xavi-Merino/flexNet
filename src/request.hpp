#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <stdexcept>
#include <string>
#include <vector>

class Request {
 public:
  Request(int id, float bitRate);

  void addFormat(std::string modulationFormat, int numberOfSlots);

  int getNumberOfSlots(int pos);
  int getNumberOfSlots(std::string modulationFormat);

 private:
  int id;
  float bitRate;
  std::vector<std::string> modulationFormat;
  std::vector<int> fsu;
  int numberOfFormats;
};

#endif