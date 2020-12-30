#ifndef __NODE_H__
#define __NODE_H__

#include <string>

/**
 * @brief Class with the node information.
 *
 * This class represents to each node in the physical network.
 */

class Node {
 public:
  /**
<<<<<<< HEAD
   * @brief Construct a new Node object. When calling with no parameters the
   * default node id is -1 and the node label is an empty string ("")
=======
   * @brief Construct a new Node object
>>>>>>> dfcd0ea50f92f78702e82823347fbc269db0c7b2
   *
   */
  Node(void);
  /**
   * @brief Construct a new Node object
   *
   * @param id The id of the new Node
   */
  Node(int id);
  /**
   * @brief Construct a new Node object
   *
   * @param id The id of the new Node
   * @param label The label of the new Node
   */
  Node(int id, std::string label);
  /**
   * @brief Destroy the Node object
   *
   */
  ~Node();
  /**
   * @brief Set the Id object
   *
   * @param id the new id of this Node object
   */
  void setId(int id);
  /**
   * @brief Get the Id object
   *
   * @return int the Id of this Node object
   */
  int getId(void) const;
  /**
   * @brief Set the Label object
   *
   * @param label The new label of this Node object
   */
  void setLabel(std::string label);
  /**
   * @brief Get the Label object
   *
   * @return std::string The label of this Node object
   */
  std::string getLabel(void) const;

 private:
  int id;
  std::string label;
};

#endif