#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "link.hpp"
#include "node.hpp"
// to use json remember to call nlohmann::json for example
#include "json.hpp"

/**
 * @brief Class with the network information.
 *
 * The Network class is used to represent an Optical Fiber Network architecture,
 * made up with connection Links and Nodes, inside the simulator. Hence, Network
 * class requires and implements Link and Node objects.
 *
 * The Network class consists of several methods for adding Links and Nodes, for
 * connecting them, check connection, use/unuse slots in Links, and getting
 * metrics.
 *
 */
class Network {
 public:
  /**
   * @brief Constructs a new Network object that represents the default
   * initialization of a Network allocated by calling the void constructor. This
   * constructor sets all the information about the Network's Links and Nodes as
   * empty so the Network is ready to be built from a clean state.
   *
   */
  Network(void);
  /**
   * @brief Destroys the Network object.
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
   * @brief Constructs a new Network object that represents a (deep) copy of an
   * already existing Network object. The new Network object is allocated via
   * this copy constructor that sets all the information about the new Network's
   * Links and Nodes to be equal to the Network passed by reference.
   *
   * @param net the original Network to be (deep) copied into a
   * new Network object. The original Network doesn't get modified.
   */
  Network(const Network &net);
  /**
   * @brief Adds a new Node object to the Network object. To add a new Node to a
   * Network, the new Node's Id must match the amount of nodes that were already
   * on the network.
   *
   * @param node the pointer to the Node desired to be added into the Network
   * object.
   */
  void addNode(Node *node);
  /**
   * @brief Adds a new Link object to the Network object. To add a new Link to a
   * Network, the new Link's Id must match the amount of links that were already
   * on the network.
   *
   * @param link the pointer to the Link desired to be added into the Network
   * object.
   */
  void addLink(Link *link);
  /**
   * @brief Gets the Node at a index position "pos" inside the Nodes vector.
   *
   * @param nodePos the position of the Node inside Nodes vector.
   * @return (Node) the pointer to the Node located at the required position.
   */
  Node *getNode(int nodePos);
  /**
   * @brief Gets the Link pointer at a index position "pos" inside Links vector.
   *
   * @param linkPos the position of the Link inside Links vector
   * @return (Link) the pointer to the Link located at the required position.
   */
  Link *getLink(int linkPos);
  /**
   * @brief The Connect methods establishes an Optical Fiber connection between
   * two Nodes through a Link inside the Network object. The different
   * connections between the different Links and Nodes of the Network build up
   * the Network's architecture.
   *
   * To connect the two Nodes through a Link, both Link
   * and (the 2) Nodes must already exist inside the Network object, that is,
   * they need to have been added previously.
   *
   * @param src the Id/position of the source node of the connection.
   * @param linkPos the Id/position of the Link used to connect the nodes.
   * @param dst the Id/position of the destination node of the connection.
   */
  void connect(int src, int linkPos, int dst);
  /**
   * @brief The isConnected method checks if the source and destination Nodes
   * are connected through a Link. If there's a connection between the two Nodes
   * through a Link, the Id/position of that Link is returned; otherwise, -1 is
   * returned.
   *
   * @param src the Id/position of the source node of the connection to be
   * checked.
   * @param dst the Id/position of the destination node of the connection to be
   * checked.
   */
  int isConnected(int src, int dst);
  /**
   * @brief The useSlot method activates a single Slot of a given position
   * inside a Link of a given position inside the Network.
   *
   * @param linkPos the position of the Link inside the links vector.
   * @param slotPos the position of the single Slot to be used/activated inside
   * the slot vector.
   */
  void useSlot(int linkPos, int slotPos);
  /**
   * @brief The useSlot method activates a range of slots inside a Link of a
   * given position inside the Network.
   *
   * The range of slots starts from the given position slotFrom and activates
   * all the slots up to the (slotTo - 1) position
   *
   * @param linkPos the position of the Link inside the links vector.
   * @param slotFrom the starting position of the Slots to be used/activated
   * inside the slot vector.
   * @param slotTo the limit before the ending position of the Slots to be
   * used/activated inside the slot vector (activates up to the (slotTo - 1)th
   * slot). It's value must be greater than slotFrom.
   */
  void useSlot(int linkPos, int slotFrom, int slotTo);
  /**
   * @brief The unuseSlot method deactivates a single Slot of a given position
   * inside a Link of a given position inside the Network.
   *
   * @param linkPos the position of the Link inside the links vector.
   * @param slotPos the position of the single Slot to be unused/deactivated
   * inside the slot vector.
   */
  void unuseSlot(int linkPos, int slotPos);
  /**
   * @brief The unuseSlot method deactivates a range of slots inside a Link of a
   * given position inside the Network.
   *
   * The range of slots starts from the given position slotFrom and deactivates
   * all the slots up to the (slotTo - 1) position
   *
   * @param linkPos the position of the Link inside the links vector.
   * @param slotFrom the starting position of the Slots to be unused/deactivated
   * inside the slot vector.
   * @param slotTo the limit before the ending position of the Slots to be
   * unused/deactivated inside the slot vector (deactivates up to the (slotTo -
   * 1)th slot). It's value must be greater than slotFrom.
   */
  void unuseSlot(int linkPos, int slotFrom, int slotTo);

  //   int distanceClass(int src, int dst);

  //   void distanceClassUntil(int s, int d, int& path_counter);
  /**
   * @brief The getNumberOfLinks method retrieves the amount of Links that have
   * been added to the Network object.
   *
   * @return int The number of Links inside the Network object.
   */
  int getNumberOfLinks();
  /**
   * @brief The getNumberOfNodes method retrieves the amount of Nodes that have
   * been added to the Network object.
   *
   * @return int The number of Nodes inside the Network object.
   */
  int getNumberOfNodes();
  /**
   * @brief The isSlotUsed method determines whether the slot in the specified
   * Link is already being used or not.
   *
   * @param linkPos the position of the specified Link to check it's Slot inside
   * the links vector.
   * @param slotPos the position of the specified Slot to check inside the slots
   * vector (inside the specified Link).
   *
   * @return bool The condition of the specified Slot. If it's active it returns
   * true, otherwise it returns false.
   *
   */
  bool isSlotUsed(int linkPos, int slotPos);
  /**
   * @brief The isSlotUsed method determines whether a range of Slots in the
   * specified Link are already being used or not.
   *
   * @param linkPos the position of the specified Link to check it's Slot inside
   * the links vector.
   * @param slotFrom the starting position of the Slots to be checked if they
   * are being used inside the slot vector.
   * @param slotTo the limit before the ending position of the Slots to be
   * checked if they are being used inside the slot vector (checks up to the
   * (slotTo - 1)th slot). It's value must be greater than slotFrom.
   *
   * @return bool The condition of the specified range of Slots. If it finds at
   * least one Slot activated/used then the entire desired range of Slots is
   * considered used and returns true, otherwise they are all unused returns
   * false.
   */
  bool isSlotUsed(int linkPos, int slotFrom, int slotTo);
  /**
   * @brief The averageNeighborhood method obtains the Nodal average metric of
   * the Network.
   *
   * @return float The Nodal neighborhood average metric value.
   */
  float averageNeighborhood();
  /**
   * @brief @brief The normalAverageNeighborhood method obtains the
   * Normalized/Standarized Nodal average metric of the Network.
   *
   * @return float The Normalized Nodal neighborhood average metric value, 1
   * representing Full Connection and 0 representing Null Connection.
   */
  float normalAverageNeighborhood();
  /**
   * @brief The nodalVariance method obtains the Nodal Variance given the Nodal
   * Average.
   *
   * @return float The Nodal Variance value of average neighborhood
   */
  float nodalVariance();

 private:
  std::vector<Node *> nodes;
  std::vector<Link *> links;
  std::vector<Link *> linksIn;
  std::vector<Link *> linksOut;
  std::vector<int> nodesIn;
  std::vector<int> nodesOut;
  int linkCounter;
  int nodeCounter;

  void validateSlotFromTo(int linkPos, int slotFrom, int slotTo);
};

#endif