#ifndef __LINK_H__
#define __LINK_H__

class Link {
 public:
  Link(void);
  Link(int id);
  ~Link();

  void setId(int id);
  int getId(void);

 private:
  int id;
};

#endif