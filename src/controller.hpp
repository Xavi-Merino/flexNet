#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "allocator.hpp"
#include "connection.hpp"
#include "network.hpp"

/**
 * @brief Class with the controller information.
 *
 * This class is used to create and access controller objects. This object
 * handles the connection requests generated inside the Simulator by executing
 * an allocation algorithm from the Allocator object on the network. Once the
 * process is completed, it returns the result to the simulator.
 *
 */
class Controller {
 public:
  /**
   * @brief Constructs a new Controller object. An empty Connection vector is
   * created inside it.
   *
   */
  Controller();
  /**
   * @brief Constructs a new Controller object. It takes a Network object and
   * stores it as an attribute. An empty Connection vector is created and
   * stored as well.
   *
   * @param network the Network object. It must be a pointer.
   */
  Controller(Network *network);
  /**
   * @brief Delete the Controller object.
   *
   */
  ~Controller();
  /**
   * @brief Assigns a connection between a source and a destination node through
   * an allocation method from the allocator. It creates a connection object
   * with idConnection as its id, upon which the connections between nodes are
   * generated.
   *
   * @param src the source node of the connection.
   * @param dst the destination node of the connection.
   * @param bitRate the bitRate object of the connection.
   * @param idConnection the id of the new connection object.
   * @return allocationStatus the result of the allocation process (whether the
   * allocation was succesful or not).
   */
  allocationStatus assignConnection(int src, int dst, BitRate bitRate,
                                    long long idConnection);
  /**
   * @brief Unnasigns the requested connection making the resources that were
   * being used become available again. It deactivates the slots that were
   * taken.
   *
   * @param idConnection the id of the Connection object.
   * @return int number zero if unsuccessful.
   */
  int unassignConnection(long long idConnection);
  /**
   * @brief Sets the paths vector from the routes on the JSON file. From the
   file, this method creates the paths vector based on an array of routes. This
   array contains the source and destination nodes, as well as an array with all
   the existing paths between them.
   *
   *
   * In the example below, the network consists of three nodes: 0, 1 and 2. Node
   0 is connected to node 1 directly and through node 2, to which it's directly
   connected as well. Node 2 is connected directly to node one. All the
   connections are unidirectional.
   *
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
   * This is the network that the controller will now handle.
   *
   * @param network the network given to the controller. It must be a pointer.
   */
  void setNetwork(Network *network);
  /**
   * @brief Get the Network in the controller.
   *
   * @return Network a pointer to the network object from the controller.
   */
  Network *getNetwork(void);
  /**
   * @brief Set the allocator of the controller. From this Allocator object, the
   * controller will be able to use an allocation method on the network.
   *
   * @param allocator a pointer to the allocator object.
   */
  void setAllocator(Allocator *allocator);
  /**
   * @brief Get the Allocator object of the controller.
   *
   * @return Allocator* a pointer to the allocator object.
   */
  Allocator *getAllocator(void);
  /**
   * @brief Get the Paths vector.
   *
   * @return std::vector<std::vector<std::vector<std::vector<Link *>>>>* a
   * pointer to the paths vector.
   */
  std::vector<std::vector<std::vector<std::vector<Link *>>>> *getPaths();

 private:
  Network *network;
  Allocator *allocator;
  std::vector<std::vector<std::vector<std::vector<Link *>>>> path;
  std::vector<Connection> connections;
  allocationStatus rtnAllocation;
};

#endif