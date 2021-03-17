#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "link.hpp"
#include "node.hpp"
// to use json remember to call nlohmann::json for example
#include "json.hpp"

/**
 * @brief Class with the network information.
 *
 * This class is used to create and access network objects, inside of which are
 * nodes connected to eachother by links. Once created, the connections will be
 * simulated on it.
 *
 */
class Network {
 public:
  /**
   * @brief Constructs a new Network object. This new network is empty and ready
   * to be filled with nodes and links.
   *
   */
  Network(void);
  /**
   * @brief Destroys the Network object
   *
   */
  ~Network();
  /**
 * @brief Constructs Network object from JSON file. From a JSON file, the
 constructor builds a network based on
 * one array of nodes and one array of links. The node array must contain a
 list of nodes ID's. The links array
 * will contain the link id, the source node (end1), the destination node
 (end2), the length of the link and the
 * number of slots.
 *
 * In the example above, the node 0 goes to the nodes 1 and 2. The node 1 goes
 to the node 2. They are connected by
 * unidirectional links with 100 slots and their own lengths.
 *
 * @param filename name of the JSON file
 * \code{.json}
       {
      "name": "4-node bus",
      "alias": "example",
      "nodes": [
          {
              "id": 0
          },
          {
              "id": 1
          },
          {
              "id": 2
          },
          {
              "id": 3
          },
      ],
       "links": [
           {
          "id": 0,
          "end1": 0,
          "end2": 1,
          "lenght": 1130,
          "slots": 100
          },
          {
          "id": 1,
          "end1": 0,
          "end2": 2,
          "lenght": 1710,
          "slots": 100
          },
          {
          "id": 3,
          "end1": 1,
          "end2": 2,
          "lenght": 700,
          "slots": 100
               },
          ]
      };
    \endcode
 */
  Network(std::string filename);
  /**
   * @brief Constructs a new Network object from the given Network, which is
   * passed by reference. This new network will share the same values.
   *
   * @param net the network object. Its values won't be modified.
   */
  Network(const Network &net);
  /**
   * @brief Adds a new Node to the Network object. Its id must match the amount
   * of nodes that were on the network.
   *
   * @param node the new Node of the Network object. It must be a pointer.
   */
  void addNode(Node *node);
  /**
   * @brief Adds a new Link to the Network object. Its id must match the amount
   * of links that were on the network.
   *
   * @param link the new Link of the Network object. It must be a pointer.
   */
  void addLink(Link *link);
  /**
   * @brief Gets the Node at a index position "pos" inside the Nodes vector.
   *
   * @param pos the position of the Node inside Nodes vector.
   * @return Node the pointer to the node located at the required position.
   */
  Node *getNode(int pos);
  /**
   * @brief Gets the Link pointer at a index position "pos" inside Links vector.
   *
   * @param pos the position of the Link inside Links vector
   * @return Link the pointer to the link located at the required position.
   */
  Link *getLink(int pos);
  /**
   * @brief Connects the source node to the destination node through the link
   * with the specified id. The direction of the connection is from source to
   * destiny. The nodes and link must previously exist on the network.
   *
   * @param src the source node of the connection.
   * @param link the Id/position of the link used to connect the nodes.
   * @param dst the destination node of the connection.
   */
  void connect(int src, int link, int dst);
  /**
   * @brief Checks if the source and destination nodes are connected through a
   * link. If the connection exists, the id of the link is returned; otherwise,
   * it returns -1.
   *
   * @param src the source node of the connection.
   * @param dst the destination node of the connection
   */
  int isConnected(int src, int dst);
  /**
   * @brief Activates a single slot to be used in the required link position.
   *
   * @param linkPos the position of the link in the links vector.
   * @param slotPos the position of the single slot inside the slot vector.
   */
  void useSlot(int linkPos, int slotPos);
  /**
   * @brief  Activates an interval of slots to be used in the required link
   * position.
   *
   * @param linkPos the position of the link in the links vector.
   * @param slotFrom the position of the slot at the beggining of the
   * required interval.
   * @param slotTo the position of the slot at the end of the required
   * interval. Its value must be greater than slotFrom.
   */
  void useSlot(int linkPos, int slotFrom, int slotTo);
  /**
   * @brief Deactivates a single slot that is no longer being used located at
   * the required link position.
   *
   * @param linkPos the position of the link inside the links vector.
   * @param slotPos the position of the slot inside the slots vector.
   */
  void unuseSlot(int linkPos, int slotPos);
  /**
   * @brief Deactivates an interval of slots that are no longer being used
   * located at the required link position.
   *
   * @param linkPos the position of the link inside the links vector.
   * @param slotFrom the position of the slot at the beggining of the required
   * interval.
   * @param slotTo the position of the slot at the end of the required interval.
   */
  void unuseSlot(int linkPos, int slotFrom, int slotTo);

  //   int distanceClass(int src, int dst);

  //   void distanceClassUntil(int s, int d, int& path_counter);
  /**
   * @brief Get the amount of links that the Network object has.
   *
   * @return int the number of links in the Network.
   */
  int getNumberOfLinks();
  /**
   * @brief Get the amount of nodes that the Network object has.
   *
   * @return int the number of nodes in the Network.
   */
  int getNumberOfNodes();
  /**
   * @brief This method is used to determine whether the slot in the specified
   * link is already being used or not.
   *
   * @param link the position of the link inside the links vector.
   * @param slot the position of the slot inside the slots vector.
   * @return bool the state of the specified slot. If it's active it returns
   * true, otherwise it returns false.
   *
   */
  bool isSlotUsed(int link, int slot);
  /**
   * @brief This method is used to determine whether the slot interval in the
   * specified link is already being used or not.
   *
   * @param link the position of the link inside the links vector.
   * @param fromSlot the position of the slot at the beggining of the required
   * interval.
   * @param toSlot the position of the slot at the end of the required interval.
   * @return bool the state of the specified interval of slots. If it's active
   * it returns true, otherwise it returns false.
   *
   */
  bool isSlotUsed(int link, int fromSlot, int toSlot);
  /**
   * @brief Obtain the nodal average in the network.
   *
   * @return float the average value.
   */
  float averageNeighborhood();
  /**
   * @brief Is similar but normalized, 1 show the network have full connection,
   * and 0 null connection.
   *
   * @return float Return nodal neighborhood normalized.
   */
  float normalAverageNeighborhood();
  /**
   * @brief In base a averageNeighborhood, obtain the variance of nodal average.
   *
   * @return float variance of average neighborhood
   */
  float nodalVariance();
  /**
   * @brief Return true or false, if at least one node is isolated.
   *
   * @return true
   * @return false
   */
  bool existNodeIsolated();
  /**
   * @brief Return true or false, if the network is total related. If exist a
   * graph or node separated from the network return false.
   *
   * @return true
   * @return false
   */
  bool isGraphRelated();

 private:
  std::vector<Node *> nodes;
  std::vector<Link *> links;
  std::vector<Link *> links_in;
  std::vector<Link *> links_out;
  std::vector<int> nodes_in;
  std::vector<int> nodes_out;
  int link_counter;
  int node_counter;
  //   int path_counter;
};

#endif