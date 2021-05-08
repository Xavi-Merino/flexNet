#ifndef __LINK_H__
#define __LINK_H__

#define DEFAULT_SLOTS 320
#define DEFAULT_LENGTH 100.0

#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Class type Link, this class allows you to create and manipulate the
 link objects. A link
 * within the simulator is the union between two nodes. The connection between
 * two nodes is unidirectional.

 * Each link has a number of slots, these slots are similar to arrays.
 */

class Link {
  friend class Network;

 public:
  /**
   * @brief Constructs a Link object. When calling with no parameters the
   * default link "id" is -1, the default link "length" is 100.0 and the default
   * "slots" are 320.
   *
   */
  Link(void);
  /**
   * @brief Constructs a Link object. The parameter is assigned to the id
   * attribute of the node, while the link length and slots are set to 100.0
   * and 320 respectively by default.
   *
   * @param id Identifier of the object. Type int. It serves to differentiate
   * the created object from the others.
   */
  Link(int id);
  /**
   * @brief Constructs a Link object. The parameters are assigned to the "id"
   * and length attributes of the node, while the slots are set to 320.
   *
   * @param id Identifier of the object. Type int. It serves to differentiate
   * the created object from the others.
   * @param length The length of this new Link object. Type int. The length
   * indicates the distance between two nodes. The unit of measure for length is
   * kilometers.
   */
  Link(int id, float length);
  /**
   * @brief Construct a Link object. The parameters are assigned to the id,
   * length and slots attributes.
   *
   * @param id Identifier of the object. Type int. It serves to differentiate
   * the created object from the others.
   * @param length The length of this new Link object. Type int. The length
   * indicates the distance between two nodes. The unit of measure for length is
   * kilometers.
   * @param slots The size of the slots vector of this Link object. Type int.
   * The slot is the channel through which the information is transmitted.
   */
  Link(int id, float length, int slots);
  /**
   * @brief Destroys the Link object.
   *
   */
  ~Link();
  /**
   * @brief Set the Id of a link. This method only works on links that were
   * created without a specified id. That is, objects created with the
   * constructor Link(void).
   *
   * @param id Identifier of the object. Type int. It serves to differentiate
   * the created object from the others.
   */
  void setId(int id);
  /**
   * @brief Set the Length of the link. The value must be greater than or equal
   * to zero.
   *
   * @param length The length of this new Link object. Type int. The length
   * indicates the distance between two nodes. The unit of measure for length is
   * kilometers.
   */
  void setLength(float length);
  /**
   * @brief Change the size of the Slots vector of the link. The value must
   * be greater than or equal to 1 and every element on the previous vector must
   * be set to false, which represents an inactive slot.
   *
   * @param slots The size of the slots vector of this Link object. Type int.
   * The slot is the channel through which the information is transmitted.
   */
  void setSlots(int slots);
  /**
   * @brief Set the value of a specific Slot inside the slots vector. The
   * position indicated must be inside the bounds of the vector, and the value
   * parameter is assigned to the corresponding slot.
   *
   * @param pos The position of the Slot inside the Slots vector. Type int,
   * greater than or equal to zero.
   * @param value The state (active or inactive) to be assigned to the specified
   * Slot. Type bool, true or false.
   */
  void setSlot(int pos, bool value);
  /**
   * @brief Get the id of the Link object. The id serves to differentiate
   * the created object from the others.
   *
   * @return int the id of this Link object.
   */
  int getId(void) const;
  /**
   * @brief Get the Length attribute of the Link object.
   *
   * @return float, the Length of this Link object.
   */
  float getLength(void) const;
  /**
   * @brief Get the size of the slots vector of the link.
   *
   * @return int, the size of the slots vector.
   */
  int getSlots(void) const;
  /**
   * @brief Get the state of a specific Slot inside the slots vector. Active or
   * inactive.
   *
   * @param pos The position you want to consult in the Slots vector.
   * @return bool, the state of the specified Slot.
   */
  bool getSlot(int pos) const;
  /**
   * @brief Get the identifier (int) of the source attribute of a Link. This
   * value is used to identify the source node. The source and destination
   * values are unique.
   *
   * @return int, the identifier of the source attribute of this Link.
   */
  int getSrc(void) const;
  /**
   * @brief Get the identifier (int) of the destination attribute of a Link.
   * This value is used to identify the source node. The source and destination
   * values are unique.
   *
   * @return int, the identifier of the destination attribute of this Link.
   */
  int getDst(void) const;

 private:
  int id;
  int src;
  int dst;
  float length;
  std::vector<bool> slots;
};

#endif