#include "link.hpp"
#include "node.hpp"

class Network {
 public:
  Network(void);
  ~Network();

  void addNode(Node node);
  void addLink(Link link);

  void connect(Link link, Node node);

 private:
  std::vector<Node> nodes;
  std::vector<Link> links;

  std::vector<Link> links_in;
  std::vector<Link> links_out;
  std::vector<int> nodes_in;
  std::vector<int> nodes_out;
};
