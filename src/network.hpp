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
  Network(std::string filename);
  /**
   * @brief Construct Network object from JSON file
   *
   * @param string
   */
  ~Network();
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
   * @brief Connects the Link and the Node into the Network object
   *
   * @param link the new Link of this Node object
   * @param node the new Node of this Node object
   */

  Node getNode(int pos);
  Link* getLink(int pos);
  void connect(int src, Link* ptr, int dst);

 private:
  std::vector<Node> nodes;
  std::vector<Link> links;

  std::vector<Link*> links_in;
  std::vector<Link*> links_out;
  std::vector<int> nodes_in;
  std::vector<int> nodes_out;
};
