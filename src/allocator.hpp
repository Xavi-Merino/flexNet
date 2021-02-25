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
 * The allocator handles the assignment of the connections in the network. This
 * class must be inherited by a new class, in which a method to allocate
 * resources has to be implemented.
 *
 */
class Allocator {
 public:
  /**
   * @brief Constructs a new Allocator object.
   *
   */
  Allocator(void);
  /**
   * @brief Constructs a new Allocator object and assigns the network, which is
   * passed as a pointer, to the network attribute.
   * @param network the network that the new allocator object will handle.
   */
  Allocator(Network *network);
  /**
   * @brief Set the Network attribute of the allocator. The network must be
   * passed as a pointer.
   *
   * @param net the pointer to the network.
   */
  void setNetwork(Network *net);
  /**
   * @brief Set the path attribute of the allocator.
   *
   * @param path the pointer to the path vector. This vector contains the
   * information regarding the routes between the nodes on the network.
   */
  void setPaths(
      std::vector<std::vector<std::vector<std::vector<Link *>>>> *path);
  /**
   * @brief Destroys the Allocator object.
   *
   */
  ~Allocator();
  /**
   * @brief The method to allocate resources. It must be implemented on an
   * inherited class.
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
   * @brief A pointer to the network object that the allocator will generate
   * connections on.
   *
   */
  Network *network;
  /**
   * @brief A pointer to a four dimensional vector that contains pointers to
   * each link on every existing route between the nodes on the network.
   *
   */
  std::vector<std::vector<std::vector<std::vector<Link *>>>> *path;
  /**
   * @brief The name of the allocation function.
   *
   */
  std::string name;
};
#endif