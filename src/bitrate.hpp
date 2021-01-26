#ifndef __BITRATE_H__
#define __BITRATE_H__

#include <string>
#include <vector>

class BitRate {
 public:
  BitRate(double bitRate);
  ~BitRate();

  void addModulation(std::string modulation, int slots, double reach);

  std::string getModulation(int pos);

  int getNumberOfSlots(int pos);

  double getReach(int pos);

 private:
  double bitRate;
  std::string bitRateStr;
  std::vector<std::string> modulation;
  std::vector<int> slots;
  std::vector<double> reach;
};

#endif