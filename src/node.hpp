#include <string>

class Node {
 public:
  Node();
  Node(int id);
  Node(int id, std::string label);
  ~Node();

  void setId(int id);
  int getId();

 private:
  int id;
  std::string label;
};
