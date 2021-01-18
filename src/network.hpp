#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "link.hpp"
#include "node.hpp"
// to use json remember to call nlohmann::json for example
#include "json.hpp"

/**
 * @brief Class with the network information.
 *
 * This class makes the connection between nodes and links in the physical
 * network.
 *
 */
class Network {
 public:
  /**
   * @brief Constructs a new Network object. When calling Network() creates an
   * empty network ready to be filled with nodes and links.
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
   * @brief Adds the Node to the Network object. Node must be a pointer.
   *
   * @param node  the new Node of this Node object
   */
  Network(const Network &net);

  void addNode(Node *node);
  /**
   * @brief Adds the Link to the Network object. Link must be a pointer
   *
   * @param link the new Link of this Node object
   */
  void addLink(Link *link);
  /**
   * @brief Gets the Node at a index position "pos" inside Nodes vector.
   *
   * @param pos the position of the Node inside Nodes vector
   * @return Node the node pointer at the required position
   */
  Node *getNode(int pos);
  /**
   * @brief Gets the Link pointer at a index position "pos" inside Links vector.
   *
   * @param pos the position of the Link inside Links vector
   * @return Link the link pointer at the required position
   */
  Link *getLink(int pos);
  /**
   * @brief Connects two nodes through a determined link pointer.
   *
   * @param src the source node of the connection
   * @param link the Id/position of the link to connect
   * @param dst the destination node of the connection
   */
  void connect(int src, int link, int dst);
  /**
   * @brief Checks if two nodes are directly connected.
   *
   * @param src the source node of the connection
   * @param dst the link pointer of the connection
   */
  int isConnected(int src, int dst);
  /**
   * @brief  Activates a single slot to be used in the required link position.
   *
   * @param linkPos the position of the link vector
   * @param slotPos the position of the slot vector
   */
  void useSlot(int linkPos, int slotPos);
  /**
   * @brief  Activates an interval of slots to be used in the required link
   * position.
   *
   * @param linkPos the position of the link vector
   * @param slotFrom the position of the slot vector at the beggining of the
   * interval.
   * @param slotTo the position of the slot vector at the end of the interval.
   */
  void useSlot(int linkPos, int slotFrom, int slotTo);

  void unuseSlot(int linkPos, int slotPos);

  void unuseSlot(int linkPos, int slotFrom, int slotTo);

  //   int distanceClass(int src, int dst);

  //   void distanceClassUntil(int s, int d, int& path_counter);

  int getNumberOfLinks();

  int getNumberOfNodes();

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