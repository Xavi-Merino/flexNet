#include <string>

class Node {
 public:
  Node();
  Node(int id);
  Node(int id, string label);
  ~Node();

  void setNodeId(int id);
  int getId();

 private:
  int id;
  std::string label;
};
