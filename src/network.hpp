#include "link.hpp"
#include "node.hpp"
/**
 * @brief Class with the network information.
 *
 * This class makes the connection between nodes and links in the physical network.
 *
 */
class Network {
 public:
 /**
   * @brief Construct a new Network object
   *
   */
  Network(void);
  ~Network();

  void addNode(Node node);
  void addLink(Link link);

  void connect(Link link, Node node);

 private:
  std::vector<Node> nodes;
  std::vector<Link> links;

  std::vector<int> links_in;
  std::vector<int> links_out;
  std::vector<int> nodes_in;
  std::vector<int> nodes_out;
};
