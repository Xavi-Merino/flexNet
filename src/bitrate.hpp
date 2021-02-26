#ifndef __BITRATE_H__
#define __BITRATE_H__

#include <string>
#include <vector>

#include "json.hpp"

/**
 * @brief Class BitRate
 *
 * This class contains the variables: bitRate, modulation, slots and reach
 *
 */

class BitRate {
 public:
  /**
   * @brief Construct a new bitRate objet. It receives a double which represents
   * the bitrate and returns the object with the set bitrate
   *
   * @param bitRate bit rate selected. Is a variable tipe double
   *
   * @return object BitRate
   */

  BitRate(double bitRate);

  /**
   * @brief Delete the objet BitRate
   */

  ~BitRate();

  /**
   * @brief Set modulation and add this to an array
   *
   * @param modulation string representing the type of modulation chosen
   * @param slot int, number of slot to set
   * @param reach double, which represents the max distance to set
   */

  void addModulation(std::string modulation, int slots, double reach);

  /**
   * @brief Get the modulation from a vector
   *
   * @param pos int which represent the position in the array
   *
   * @return string which represent the modulation
   */

  std::string getModulation(int pos);

  /**
   * @brief Get the number of slot from a vector
   *
   * @param pos int which represent the position in the array
   *
   * @return string which represent the number of slots
   */

  int getNumberOfSlots(int pos);

  /**
   * @brief Get the reach from a vector
   *
   * @param pos int which represent the position in the array
   *
   * @return int which represent the reach
   */

  double getReach(int pos);

  /**
   * @brief Read a json file and extract the corresponding information, to fill
   * a vector type bitrate.
   *
   * @param fileName location of json file, example: "../bitrate/bitrate.json"
   *
   * @param vect vector pointer type bitrate, which enters empty and is filled
   * with the json information.
   */

  static std::vector<BitRate> readBitRateFile(std::string fileName);

 private:
  double bitRate;
  std::string bitRateStr;
  std::vector<std::string> modulation;
  std::vector<int> slots;
  std::vector<double> reach;
};

#endif