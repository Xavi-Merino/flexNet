#ifndef __REQUEST_H__
#define __REQUEST_H__

#include <string>
#include <vector>

class Request {
 public:
  Request(int id, float bitRate);

 private:
  int id;
  float bitRate;
  std::vector<std::string> modulationFormat;
  std::vector<int> fsu;

  double clock;
};

#endif