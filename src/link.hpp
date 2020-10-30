#ifndef __LINK_H__
#define __LINK_H__

#include <vector>

#define DEFAULT_SLOTS 320
#define DEFAULT_LENGTH 100.0

/**
 * @brief Class with the link information.
 *
 * This class represents to each link in the physical network.
 *
 */

class Link {
 public:
  /**
   * @brief Construct a new Link object
   *
   */
  Link(void);
  /**
   * @brief Construct a new Link object
   *
   * @param id The id of this new Link object
   * @param length The length of this new Link object
   */
  Link(int id, float length);
  /**
   * @brief Construct a new Link object
   *
   * @param id The id of this new Link object
   * @param length The length of this new Link object
   * @param slots The size of the Slots vector of this new Link object
   */
  Link(int id, float length, int slots);
  /**
   * @brief Destroy the Link object
   *
   */
  ~Link();
  /**
   * @brief Set the Id object
   *
   * @param id The new Id of this Link object
   */
  void setId(int id);
  /**
   * @brief Set the Length object
   *
   * @param length The new Length of this Link object
   */
  void setLength(float length);
  /**
   * @brief Set the size of the Slots object/vector
   *
   * @param slots The new size of the Slots vector of this Link object
   */
  void setSlots(int slots);
  /**
   * @brief Get the Id object
   *
   * @return int The Id of this Link object
   */
  int getId(void);
  /**
   * @brief Get the Length object
   *
   * @return float The Length of this Link object
   */
  float getLength(void);
  /**
   * @brief Get the size of the Slots object/vector
   *
   * @return int The Id of this Link object
   */
  int getSlots(void);

 private:
  int id;
  float length;
  std::vector<bool> slots;
};

#endif