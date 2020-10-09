class Node {
 private:
  int id;
  string label;

 public:
  Node();
  Node(int id);
  Node(int id, string label);
  ~Node();

  void setNodeId(int id);
  int getId();
};
