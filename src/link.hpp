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
 * This class is used to create and access link objects, which represent the
 * links between the nodes in the network. Each link is divided in slots.
 */

class Link {
  friend class Network;

 public:
  /**
   * @brief Constructs a new Link object. When calling with no parameters the
   * default link id is -1, the default link length is 100.0 and the default
   * slots are 320.
   *
   */
  Link(void);
  /**
   * @brief Constructs a new Link object. The parameter is assigned to the id
   * attribute of the node, while the link length and slots are set to 100.0 and
   * 320 respectively.
   *
   * @param id The id of this new Link object.
   */
  Link(int id);
  /**
   * @brief Constructs a new Link object. The parameters are assigned to the id
   * and length attributes of the node, while the slots are set to 320.
   *
   * @param id The id of this new Link object.
   * @param length The length of this new Link object.
   */
  Link(int id, float length);
  /**
   * @brief Construct a new Link object. The parameters are assigned to the id,
   * length and slots attributes.
   *
   * @param id The id of this new Link object.
   * @param length The length of this new Link object.
   * @param slots The size of the Slots vector of this new Link object.
   */
  Link(int id, float length, int slots);
  /**
   * @brief Destroys the Link object.
   *
   */
  ~Link();
  /**
   * @brief Set the Id of a link. This method only works on links that were
   * created without a specified id.
   *
   * @param id The new Id of this Link object.
   */
  void setId(int id);
  /**
   * @brief Set the Length of the link. The value must be greater than or equal
   * to zero.
   *
   * @param length The new Length of this Link object.
   */
  void setLength(float length);
  /**
   * @brief Change the size of the Slots vector of the link. The value must
   * be greater than or equal to 1 and every element on the previous vector must
   * be set to false, which represents an inactive slot.
   *
   * @param slots The new size of the Slots vector of this Link object.
   */
  void setSlots(int slots);
  /**
   * @brief Set the value of a specific Slot inside the slots vector. The
   * position indicated must be inside the bounds of the vector, and the value
   * parameter is assigned to the corresponding slot.
   *
   * @param pos The position of the Slot inside the Slots vector.
   * @param value The state (active or inactive) to be assigned to the specified
   * Slot.
   */
  void setSlot(int pos, bool value);
  /**
   * @brief Get the Id of the link.
   *
   * @return int The Id of this Link object.
   */
  int getId(void) const;
  /**
   * @brief Get the Length attribute of the link.
   * @return float The Length of this Link object.
   */
  float getLength(void) const;
  /**
   * @brief Get the size of the Slots vector of the link.
   *
   * @return int The size of the slots vector.
   */
  int getSlots(void) const;
  /**
   * @brief Get the state of a specific Slot inside the slots vector.
   *
   * @param pos The position of the Slot in the Slots vector.
   * @return bool The state of the specified Slot.
   */
  bool getSlot(int pos) const;
  int getSrc(void) const;
  int getDst(void) const;

 private:
  int id;
  int src;
  int dst;
  float length;
  std::vector<bool> slots;
};

#endif