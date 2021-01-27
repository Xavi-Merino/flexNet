#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "allocator.hpp"
#include "connection.hpp"
#include "network.hpp"

/**
 * @brief Class with the controller information.
 *
 */
class Controller {
 public:
  /**
   * @brief Constructs a new Controller object. An empty Connection vector is
   * created inside the controller.
   *
   */
  Controller();
  /**
   * @brief Constructs a new Controller object. It takes a Network object and
   * stores it as an attribute. An empty Connection vector is created and
   * stored as well.
   *
   * @param network the Network object.
   */
  Controller(Network *network);
  /**
   * @brief Destroys the Controller object.
   *
   */
  ~Controller();
  /**
   * @brief Assigns a connection between a source and a destination node through
   * an allocation method from the allocator.
   *
   * @param src the source node of the connection.
   * @param dst the destination node of the connection.
   * @param bitRate the bit rate of the connection.
   * @param idConnection the id of the connection.
   * @return allocationStatus the result of the allocation process (whether the
   * allocation was succesful or not).
   */
  allocationStatus assignConnection(int src, int dst, BitRate bitRate,
                                    long long idConnection);
  /**
   * @brief Unnasigns the requested connection making the resources that were
   * used become available again. It deactivates the slots that were taken.
   *
   * @param idConnection the id of the Connection object.
   * @return int number zero.
   */
  int unassignConnection(long long idConnection);
  /**
   * @brief Sets the paths vector from the routes on the JSON file.
   *
   * @param filename name of the JSON file that contains the routes.
   * \code{.json}
     {
      "name": "Example routes between 3 nodes",
      "alias": "example",
      "routes": [
        {
          "src": 0,
          "dst": 1,
          "paths": [
            [
              0,
              1
            ],
            [
              0,
              2,
              1
            ]
          ]
        },
        {
          "src": 0,
          "dst": 2,
          "paths": [
            [
              0,
              2
            ]
          ]
        },
        {
          "src": 2,
          "dst": 1,
          "paths": [
            [
              2,
              1
            ]
          ]
        }
      ]
     };
    \endcode
   *
   */
  void setPaths(std::string filename);
  /**
   * @brief Sets the Network object as the network attribute of the controller.
   *
   * @param network the network given to the controller.
   */
  void setNetwork(Network *network);
  /**
   * @brief Get the Network in the controller.
   *
   * @return Network the network object from the controller.
   */
  Network *getNetwork(void);
  /**
   * @brief Set the allocator of the controller. The Allocator object
   * must be given as a pointer.
   *
   * @param allocator a pointer to the allocator object.
   */
  void setAllocator(Allocator *allocator);
  Allocator *getAllocator(void);

  std::vector<std::vector<std::vector<std::vector<Link *>>>> *getPaths();

 private:
  Network *network;
  Allocator *allocator;
  std::vector<std::vector<std::vector<std::vector<Link *>>>> path;
  std::vector<Connection> connections;
  allocationStatus rtnAllocation;
};

#endif