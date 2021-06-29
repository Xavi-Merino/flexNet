#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__
#include <string>

#include "bitrate.hpp"
#include "connection.hpp"
#include "network.hpp"

typedef enum allocStatus { ALLOCATED, NOT_ALLOCATED, N_A } allocationStatus;
/**
 * @brief Class with the allocator information.
 *
 * The Allocator class handles the assignment of connections inside a Network.
 * This class must be inherited by another class, in which a method to allocate
 * resources has to be implemented.
 *
 * The Allocator class consists of a Network object, a Path vector to the Links
 * inside said Network and a Name attribute and methods for setting them.
 */
class Allocator {
 public:
  /**
   * @brief Constructs a new Allocator object that represents the default
   * initialization of an Allocator invoked by calling the void constructor.
   This
   * constructor sets all the information about the Allocator's Networks and
   Paths as
   * empty so the Allocator is ready to be built from a clean state.

   */
  Allocator(void);
  /**
   * @brief Constructs a new Allocator object that represents the
   * initialization of an Allocator with a given (already existing) Network
   * to be set as the Allocator's own Network.
   *
   * @param network the pointer to the Network object that will be set as the
   * Allocator's own Network.
   */
  Allocator(Network *network);
  /**
   * @brief Set the Allocator's Network as the one given by the pointer of an
   * already existing Network object.
   *
   * @param network the pointer to the Network object that will be set as the
   * Allocator's own Network.
   */
  void setNetwork(Network *network);
  /**
   * @brief Set the Path attribute of the Allocator.
   *
   * @param path the pointer to the Path vector. This vector contains the
   * information regarding the routes between the Nodes inside Network.
   */
  void setPaths(
      std::vector<std::vector<std::vector<std::vector<Link *>>>> *path);
  /**
   * @brief Destroys the Allocator object.
   *
   */
  ~Allocator();
  /**
   * @brief The method to allocate resources inside the Network of the
   * Allocator. It must be implemented inside an inherited class, such as a
   * controller.
   *
   * @param src the id of the source Node in the requested connection.
   * @param dst the id of the destination Node in the requested connection.
   * @param bitRate the BitRate object.
   * @param con the Connection object. It's passed by reference.
   * @return allocationStatus the result of the allocation process, whether the
   * resources were allocated or not.
   */
  virtual allocationStatus exec(int src, int dst, BitRate bitRate,
                                Connection &con);
  /**
   * @brief Get the name attribute of the allocator object.
   *
   * @return std::string the name of the allocator.
   */
  std::string getName(void);

 protected:
  /**
   * @brief A pointer to the Network object that the Allocator will generate
   * connections on.
   *
   */
  Network *network;
  /**
   * @brief A pointer to a four dimensional vector that contains pointers to
   * each Link on every existing route between the Nodes inside the Network.
   *
   */
  std::vector<std::vector<std::vector<std::vector<Link *>>>> *path;
  /**
   * @brief The Name of the allocation algorithm.
   *
   */
  std::string name;
};
#endif