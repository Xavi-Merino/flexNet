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
   * @param slots int, number of slot to set
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
   * @brief Get the Bitrate string. This bitrate is a number in string
   * format. By example, 10 (that generally means 10 Gbps).
   *
   * @return std::string The bitrate of this object in string format.
   */
  std::string getBitRateStr();

  /**
   * @brief Get the Bitrate. This bitrate is a number of type double. By
   * example, 10.0 (that generally means 10 Gbps).
   *
   * @return double The bitrate of this object.
   */
  double getBitRate();

  /**
   * @brief Read a json file and extract the corresponding information, to fill
   * a vector type bitrate. The json format is shown in the next code:
   *
   * \code{.json}
       {
            "10": [
                {
                    "BPSK": {
                        "slots": 1,
                        "reach": 5520
                    }
                }
            ],
            "40": [
                {
                    "BPSK": {
                        "slots": 4,
                        "reach": 5520
                    }
                },
                {
                    "QPSK": {
                        "slots": 2,
                        "reach": 5520
                    }
                }
            ],
            "100": [
                {
                    "BPSK": {
                        "slots": 8,
                        "reach": 5520
                    }
                }
            ],
            "400": [
                {
                    "BPSK": {
                        "slots": 32,
                        "reach": 5520
                    }
                }
            ],
            "1000": [
                {
                    "BPSK": {
                        "slots": 80,
                        "reach": 5520
                    }
                }
            ]
        }
    \endcode
   * Each JSON item corresponds to an specific bitrate, and each bitrate has a
   * list of modulation formats. Every modulation format in an specific bitrate
   * has its own optical reach and slots demand.
   * @param fileName location of json file, as a relative path. By example:
"../bitrate/bitrate.json"
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