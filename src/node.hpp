#ifndef __NODE_H__
#define __NODE_H__

#include <stdexcept>
#include <string>

/**
 * @brief Class with the node information.
 *
 * The Node class is used to represent a Node in an Optical Fiber Network inside
 * the simulator.
 *
 * Nodes objects are needed, alongside Link objects, in order to create an
 * Optical Fiber Network architecture used as a base for the simulations.
 *
 * The Node class enfolds/contains two attributes:
 * -A mandatory/necessary attribute called Id (id) used as an unique identifier.
 * -An optional attribute called Label (label) used for adding extra information
 * for identifying the node.
 *
 * The Node class consists of 4 methods for setting/accessing the values of
 * their Id and Label attributes respectively.
 */

class Node {
 public:
  /**
   * @brief Constructs a new Node object that represents the default Node
   * allocated by calling the void constructor. This constructor sets the Id
   * attribute to -1. This constructor sets the Label to an empty string ("").
   *
   */
  Node(void);
  /**
   * @brief Constructs a new Node object that represents a Node with the given
   * Id number. This constructor sets the Id attribute to id parameter given
   * when invoking the constructor. This constructor sets the Label to an empty
   * string ("").
   *
   * @param id The desired Id (integer) number used to identify the Node.
   */
  Node(int id);
  /**
   * @brief Constructs a new Node object that represents a Node with the given
   * Id number. This constructor sets the Id attribute to the value of the id
   * parameter given when invoking the constructor. This constructor sets the
   * Label to the value of the label parameter given when invoking the
   * constructor.
   *
   * @param id The desired Id (integer) number used to identify the Node.
   * @param label The desired Label (string) used to add extra information to
   * the Node.
   */
  Node(int id, std::string label);
  /**
   * @brief Destroys the Node object.
   *
   */
  ~Node();
  /**
   * @brief Set the Id unique number of the current Node object.
   * This method is meant to be used on Nodes that were created as default Nodes
   * with no previous Id number given (and automatically set to -1).
   *
   * @param id The desired Id (integer) number used to identify the Node.
   */
  void setId(int id);
  /**
   * @brief Gt the Id unique number of the current Node object.
   *
   * @return The Id (integer) unique number used to identify the Node.
   */
  int getId(void) const;
  /**
   * @brief Set the optional Label string of the current Node object.
   *
   * @param label The desired string (std::string) used to label the Node for
   * additional information.
   */
  void setLabel(std::string label);
  /**
   * @brief Get the Label string of the current Node object.
   *
   * @return The Label string (std::string) of the Node.
   */
  std::string getLabel(void) const;

 private:
  int id;
  std::string label;
};

#endif