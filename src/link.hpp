#ifndef __LINK_H__
#define __LINK_H__

#define DEFAULT_SLOTS 320
#define DEFAULT_LENGTH 100.0

#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Class with the link information.
 *
 * This class represents to each link in the physical network.
 *
 */

class Link {
 public:
  /**
   * @brief Construct a new Link object. When calling with no parameters the
   * default link id is -1, the default link length is 100.0 and the default
   * slots are 320.
   *
   */
  Link(void);
  /**
   * @brief Construct a new Link object
   *
   * @param id The id of this new Link object
   */
  Link(int id);
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
   * @brief Set the size of the Slots object
   *
   * @param slots The new size of the Slots vector of this Link object
   */
  void setSlots(int slots);
  /**
   * @brief Set the value of a specific Slot
   *
   * @param pos The position of the Slot inside the Slots vector
   * @param value The value desired to assign to the specified Slot
   */
  void setSlot(int pos, bool value);
  /**
   * @brief Get the Id object
   *
   * @return int The Id of this Link object
   */
  int getId(void) const;
  /**
   * @brief Get the Length object
   *
   * @return float The Length of this Link object
   */
  float getLength(void) const;
  /**
   * @brief Get the size of the Slots object
   *
   * @return int The Id of this Link object
   */
  int getSlots(void) const;
  /**
   * @brief Get the value of a specific Slot
   *
   * @param pos The position of the Slot in the Slots vector
   * @return bool The value of the specified Slot
   */
  bool getSlot(int pos) const;

 private:
  int id;
  float length;
  std::vector<bool> slots;
};

#endif