#include "aux_route.cpp"
#include <fstream>
#include <deque>

class buffer_element {
  public:
    buffer_element(int src, int dst, long long id, BitRate *bitRate){
      this->src = src;
      this->dst = dst;
      this->bitRate = new BitRate(*bitRate);
      this->id = id;
    }

    ~buffer_element();

    int src;
    int dst;
    long long id;
    BitRate *bitRate;
};

  
buffer_element::~buffer_element(){}