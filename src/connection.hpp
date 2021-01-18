#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <vector>

class Connection {
 public:
  Connection(long long id);

  ~Connection();

  void addLink(int idLink, std::vector<int> slots);
  void addLink(int idLink, int fromSlot, int toSlot);

 private:
  long long id;
  std::vector<int> links;
  std::vector<std::vector<int> > slots;

  friend class Controller;
};
#endif