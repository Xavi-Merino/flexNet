#ifndef __BITRATE_H__
#define __BITRATE_H__

#include <string>
#include <vector>

#include "json.hpp"

/**
 * @brief Class BitRate
 *
 * The BitRate class is used to represent the Bit Rate, which is the amount of
 * data transmitted/processed (in bits) per unit of time inside a digital
 * transmission system or a computer network (and, for simulation purposes,
 * inside the Simulator (class) itself.)
 *
 * A Bit Rate of certain magnitude can be represented in different modulation
 * formats, each with it's own number of slots and maximum reach.
 *
 * The BitRate class contains 2 main attributes: the bit rate itself (as a
 * magnitude/quantity in Gbps) represented as a float (bitRate) and it's
 * stringified equivalent (bitRateStr); and 3 vectors attributes: a string
 * vector to store the name of the different desired modulations formats for the
 * bit rate (such as "BPSK", "QPSK", "8-QAM", etc), an integer vector to store
 * the number of slots desired for each modulation and a float vector to store
 * the reach for each modulation. Throughout all these vectors, each modulation
 * is located at the same index.
 *
 * The BitRate class contains several methods: getters for all it's attributes,
 * a method to add a new modulation format and another to get a BitRate objects
 * vector filled with information from a given JSON file.
 */
class BitRate {

  /**
   * @brief Copy construct that initializes a new bitRate object using another
   * object of the same class.
   *
   * @param bitRate (BitRate): Object to copy.
   */
  BitRate(const BitRate &bitRate);
 public:
  /**
   * @brief Construct a new bitRate object using the given double for setting
   * the object's bit rate magnitude and it's respective string.
   *
   * @param bitRate (float): The desired bit rate magnitude.
   */
  BitRate(double bitRate);

  /**
   * @brief Delete the BitRate object.
   */
  ~BitRate();

  /**
   * @brief Adds a new modulation to the current BitRate object, given it's
   * name, slots numbers and reach distance.
   *
   * @param modulation (std::string): The name of the desired modulation.
   * @param slots (integer): The number of slots to be set for the modulation.
   * @param reach (double): The maximum distance to be reached for this
   * modulation.
   */
  void addModulation(std::string modulation, int slots, double reach);

  /**
   * @brief Gets the modulation name in a given position (argument) inside the
   * modulations string (names) vector.
   *
   * @param pos (integer): The position index for the desired modulation.
   *
   * @return (std::string): The modulation name of the desired modulation.
   */
  std::string getModulation(int pos);

  /**
   * @brief Gets the number of slots in a given position (argument) inside the
   * modulations slots vector.
   *
   * @param pos (integer): The position index for the desired modulation.
   *
   * @return (integer): The number of slots of the desired modulation.
   */
  int getNumberOfSlots(int pos);

  /**
   * @brief Gets the maximum reach distance in a given position (argument)
   * inside the modulations reach vector.
   *
   * @param pos (integer): The position index for the desired modulation.
   *
   * @return (double): The maximum reach distance of the desired modulation.
   */
  double getReach(int pos);

  /**
   * @brief Gets the bit rate magnitude (quantity) in string format, i.e. "10.0"
   * (Gbps).
   *
   * @return (std::string): The bit rate of the current BitRate object in string
   * format.
   */
  std::string getBitRateStr();

  /**
   * @brief Gets the number of modulations available in the current object
   *
   * @return (int): The number of modulations in the current BitRate object.
   */
  int getNumberOfModulations();

  /**
   * @brief Gets the bit rate magnitude (quantity) in numerical (double) format,
   * i.e. 10.0 (Gbps).
   *
   * @return (double): The bit rate magnitude of the current BitRate object.
   */
  double getBitRate();

  /**
   * @brief Reads a JSON file and extracts it's information to fill
   * a vector of BitRate objects.
   *
   * The JSON format is shown as an example in the next code snippet:
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
   * Each JSON item corresponds to an specific bit rate, and each bit rate has a
   * list of modulation formats.
   * Every modulation format inside an specific bit rate has its own optical
reach and slots demand.
   * @param fileName location of JSON file, as a relative path. For example:
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