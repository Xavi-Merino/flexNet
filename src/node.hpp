#ifndef __NODE_H__
#define __NODE_H__

#include <stdexcept>
#include <string>

/**
 * @brief Class with the node information.
 *
 * This class is used to create and access node objects, which represent the
 * nodes in the network.
 *
 */

class Node {
 public:
  /**
   * @brief Constructs a new Node object. By default, the id attribute is set to
   * -1 and the label to an empty string.
   *
   */
  Node(void);
  /**
   * @brief Constructs a new Node object. The id attribute is set to the value
   * passed as a parameter, and the label is set to an empty string.
   *
   * @param id The id of the new Node.
   */
  Node(int id);
  /**
   * @brief Constructs a new Node object. The id and label attributes of the
   * node are set to the values passed as parameters.
   *
   * @param id The id of the new Node.
   * @param label The label of the new Node.
   */
  Node(int id, std::string label);
  /**
   * @brief Destroys the Node object.
   *
   */
  ~Node();
  /**
   * @brief Set the Id of a node. This method only works on nodes that were
   * created without a specified id.
   *
   * @param id the new id of the Node object.
   */
  void setId(int id);
  /**
   * @brief Get the Id attribute of the node.
   *
   * @return int the Id of the Node object.
   */
  int getId(void) const;
  /**
   * @brief Set the Label of the node.
   *
   * @param label The new label of the Node object. It must be a string.
   */
  void setLabel(std::string label);
  /**
   * @brief Get the Label attribute of the node.
   *
   * @return std::string The label of the Node object.
   */
  std::string getLabel(void) const;

 private:
  int id;
  std::string label;
};

#endif