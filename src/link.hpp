#ifndef __LINK_H__
#define __LINK_H__

#define DEFAULT_SLOTS 320
#define DEFAULT_LENGTH 100.0
#define DEFAULT_CORES 1
#define DEFAULT_MODES 1

#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Class type Link, this class allows you to create and manipulate the
 link objects. A link
 * within the simulator is the union between two nodes. The connection between
 * two nodes is unidirectional.

 * Each link has a number of cores, where each core has an associated number 
 * of modes, and at the same time, each mode has a number of slots, these
 * atributes are arranged in a three-dimensional vector. In case of simulating
 * single-core and single-mode network, the modes and cores can be omitted
 */

class Link {
  friend class Network;

 public:
  /**
   * @brief Constructs a Link object. When calling with no parameters the
   * default link "id" is -1, the default link "length" is 100.0, the default
   * "number_of_cores" is 1, the default "number_of_modes" is 1 and the default
   * "slots" are 320.
   *
   */
  Link(void);
  /**
   * @brief Constructs a Link object. The parameter is assigned to the id
   * attribute of the node, while the link length is set to 100.0, both number
   * of cores and modes are set to 1, and the slots to 320 by default
   * 
   *
   * @param id Identifier of the object. Type int. It serves to differentiate
   * the created object from the others.
   */
  Link(int id);
  /**
   * @brief Constructs a Link object. The parameters are assigned to the "id"
   * and length attributes of the node, while both number of cores and modes are
   * set to 1 and the slots are set to 320.
   *
   * @param id Identifier of the object. Type int. It serves to differentiate
   * the created object from the others.
   * @param length The length of this new Link object. Type float. The length
   * indicates the distance between two nodes. The unit of measure for length is
   * kilometers.
   */
  Link(int id, float length);  
  /**
   * @brief Construct a Link object. The parameters are assigned to the id,
   * length and slots attributes, while both number of cores and modes are set
   * to 1 assuming a single-mode/single-core network.
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
   * @brief Construct a Link object. The parameters are assigned to the id,
   * length, slots and number_of_cores, while the number_of_modes is set to 1.
   *
   * @param id Identifier of the object. Type int. It serves to differentiate
   * the created object from the others.
   * @param length The length of this new Link object. Type int. The length
   * indicates the distance between two nodes. The unit of measure for length is
   * kilometers.
   * @param slots The size of the slots vector of this Link object. Type int.
   * The slot is the channel through which the information is transmitted.
   * @param number_of_cores The number of cores of this Link object. Type int.
   * The number of cores determine the number of paths through which the information
   * can be transmited within the same link.
   */
  Link(int id, float length, int slots, int number_of_cores);
  /**
   * @brief Construct a Link object. The parameters are assigned to the id,
   * length, number_of_cores, number_of_modes and slots attributes.
   *
   * @param id Identifier of the object. Type int. It serves to differentiate
   * the created object from the others.
   * @param length The length of this new Link object. Type int. The length
   * indicates the distance between two nodes. The unit of measure for length is
   * kilometers.
   * @param slots The size of the slots vector of this Link object. Type int.
   * The slot is the channel through which the information is transmitted.
   * @param number_of_cores The number of cores of this Link object. Type int.
   * The number of cores determine the number of paths through which the information
   * can be transmited within the same link.
   * @param number_of_modes The number of modes of this Link object. Type int.
   * The number of modes determines how many modes each core belonging to this link
   * can support.
   */
  Link(int id, float length, int slots, int number_of_cores, int number_of_modes);
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
   * In case of a multi-core/multi-mode link, the size of every slot vector 
   * within that link is changed.
   *
   * @param slots The size of the slots vector of this Link object. Type int.
   * The slot is the channel through which the information is transmitted.
   */
  void setSlots(int slots);
    /**
   * @brief Change the size of the Slots vector of the link in the specific
   * mode of a especific core. The value must be greater than or equal to 1 
   * and every element on the previous vector must be set to false, which 
   * represents an inactive slot.
   *
   * @param slots The size of the slots vector of this Link object. Type int.
   * The slot is the channel through which the information is transmitted. 
   * @param core The core index on the object Link. Type int, greater than or 
   * equal to zero. 
   * @param mode The mode index on the object Link. Type int, greater than or
   * equal to zero.
   */
  void setSlots(int slots, int core, int mode);
  /**
   * @brief Set the value of a specific Slot inside the slots vector. The
   * position indicated must be inside the bounds of the vector, and the value
   * parameter is assigned to the corresponding slot.
   * 
   * This method assumes a single-mode/single-core network.
   *
   * @param pos The position of the Slot inside the Slots vector. Type int,
   * greater than or equal to zero.
   * @param value The state (active or inactive) to be assigned to the specified
   * Slot. Type bool, true or false.
   */
  void setSlot(int pos, bool value);
  /**
   * @brief Set the value of a specific Slot inside the slots vector of the
   * specified core and mode. The position indicated must be inside the bounds 
   * of the vector, mode and core, and the value parameter will be assigned to
   * the corresponding slot.
   *
   * @param core The core index on the object Link. Type int, greater than or
   * equal to zero.
   * @param mode The mode index on the object Link. Type int, greater than or
   * equal to zero.
   * @param pos The position of the Slot inside the Slots vector. Type int,
   * greater than or equal to zero.
   * @param value The state (active or inactive) to be assigned to the specified
   * Slot. Type bool, true or false.
   */
  void setSlot(int core, int mode, int pos, bool value);
  /**
   * @brief Change the number of cores of the link. The value must be greater
   * than or equal to 1 and every element of every slots vector asociated to 
   * this link must be set to false, state that represent an inactive slot.
   *
   * @param number_of_cores The number of cores of this Link object. Type int.
   * The number of cores determine the number of paths through which the information
   * can be transmited within the same link.
   */
  void setCores(int number_of_cores);
  /**
   * @brief Change the number of modes of the link. The value must be greater
   * than or equal to 1 and every element of every slots vector asociated to 
   * this link must be set to false, state that represent an inactive slot.
   *
   * @param number_of_modes The number of modes of this Link object. Type int.
   * The number of modes determines how many modes each core belonging to this link
   */
  void setModes(int number_of_modes);
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
   * This method assumes a single-mode/single-core network.
   *
   * @return int, the size of the slots vector.
   */
  int getSlots(void) const;
  /**
   * @brief Get the size of the slots vector from a specific Core and Mode
   * of the link.
   *
   * @param core The core index on the object Link. Type int, greater than or
   * equal to zero.
   * @param mode The mode index on the object Link. Type int, greater than or
   * equal to zero.
   * 
   * @return int, the size of the slots vector.
   */
  int getSlots(int core, int mode) const;
  /**
   * @brief Get the state of a specific Slot inside the slots vector. Active or
   * inactive.
   * 
   * This method assumes a single-mode/single-core network.
   *
   * @param pos The position you want to consult in the Slots vector.
   * @return bool, the state of the specified Slot.
   */
  bool getSlot(int pos) const;
  /**
   * @brief Get the state of a specific Slot inside the slots vector of the 
   * specified Core and Mode. Active or inactive.
   *
   * @param core The core to which the mode of the Slots vector belongs.
   * @param mode The mode to which the Slots vector belongs.
   * @param pos The position you want to consult in the Slots vector.
   * @return bool, the state of the specified Slot.
   */
  bool getSlot(int core, int mode, int pos) const;
  /**
   * @brief Get the Number of cores attribute of the Link object.
   *
   * @return int, the number_of_cores of this Link object.
   */
  int getCores(void) const;
  /**
   * @brief Get the Number of modes attribute of the Link object.
   *
   * @return int, the number_of_modes of this Link object.
   */
  int getModes(void) const;
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
  int number_of_cores;
  int number_of_modes;
  std::vector<std::vector<std::vector<bool>>>slots;

};

#endif