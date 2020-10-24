#ifndef __NODE_H__
#define __NODE_H__

#include <string>

class Node {
 public:
  Node(void);
  Node(int id);
  Node(int id, std::string label);
  ~Node();

  void setId(int id);
  int getId(void);

  void setLabel(std::string label);
  std::string getLabel(void);

 private:
  int id;
  std::string label;
};

#endif