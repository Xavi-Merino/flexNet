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
   * @brief Adds the Node to the Network object
   *
   * @param node  the new Node of this Node object
   */
  void addNode(Node node);
  /**
   * @brief Adds the Link to the Network object
   *
   * @param link the new Link of this Node object
   */
  void addLink(Link link);
  /**
   * @brief Gets the Node at a index position "pos" inside Nodes vector.
   *
   * @param pos the position of the Node inside Nodes vector
   * @return Node the node at the required position
   */
  Node getNode(int pos);
  /**
   * @brief Gets the Link pointer at a index position "pos" inside Links vector.
   *
   * @param pos the position of the Link inside Links vector
   * @return Link the link at the required position
   */
  Link* getLink(int pos);
  /**
   * @brief Connects two nodes through a determined link pointer.
   *
   * @param src the source node of the connection
   * @param ptr the link pointer of the connection
   * @param dst the destination node of the connection
   */
  void connect(int src, Link* ptr, int dst);
  /**
   * @brief Checks if two nodes are directly connected.
   *
   * @param id1 the source node of the connection
   * @param id2 the link pointer of the connection
   */
  bool isConnected(int id1, int id2);

 private:
  std::vector<Node> nodes;
  std::vector<Link> links;
  std::vector<Link*> links_in;
  std::vector<Link*> links_out;
  std::vector<int> nodes_in;
  std::vector<int> nodes_out;
};
