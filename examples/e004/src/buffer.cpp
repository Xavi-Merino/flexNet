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

    bool operator>(const buffer_element &e) const
    {
      return bitRate->getBitRate() > e.bitRate->getBitRate();
    }

    bool operator<(const buffer_element &e) const
    {
      return bitRate->getBitRate() < e.bitRate->getBitRate();
    }
};

  
buffer_element::~buffer_element(){}
