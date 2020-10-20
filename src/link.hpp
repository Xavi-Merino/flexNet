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
