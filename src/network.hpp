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
   * @brief Constructs a new Network object
   *
   */
  Network(void);
  /**
   * @brief Destroys the Network object
   *
   */
  ~Network();
  /**
   * @brief Constructs Network object from JSON file
   *
   * @param filename name of the JSON file
   * \code{.json} Using the JSON API of Niels Lohmann, the constructor builds a n-size node bus with unidirectional link lines between them
            class json.hpp {
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

 private:
  std::vector<Node> nodes;
  std::vector<Link> links;
  std::vector<Link*> links_in;
  std::vector<Link*> links_out;
  std::vector<int> nodes_in;
  std::vector<int> nodes_out;
};
