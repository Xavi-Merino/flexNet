#include "aux_route.cpp"
#include <fstream>
#include <deque>

class buffer_element {

  public:
    buffer_element(int src, int dst, long long id, BitRate *bitRate, double time_arrival){
      this->src = src;
      this->dst = dst;
      this->bitRate = new BitRate(*bitRate);
      this->id = id;
      this->time_arrival = time_arrival;
      this->current_attempts = 0;
    }

    ~buffer_element() {};

    int src;
    int dst;
    long long id;
    BitRate *bitRate;
    double time_arrival;

    void setAttempts(int attempts){
      this->current_attempts = attempts;
    }

    int getAttempts(){
      return this->current_attempts;
    }

    bool operator>(const buffer_element &e) const
    {
      return bitRate->getBitRate() > e.bitRate->getBitRate();
    }

    bool operator<(const buffer_element &e) const
    {
      return bitRate->getBitRate() < e.bitRate->getBitRate();
    }

  private:
    int current_attempts;

};

class Buffer {

  public:

    Buffer(){
      this->elements = std::deque<buffer_element>();
      this->last_time = 0;
      this->poped = 0;
      this->pushed = 0;
      this->mean_size_time = 0;
      this->mean_service_time = 0;
    }

    void addElement(buffer_element new_element){
      this->elements.push_back(new_element);
    }

    void pop_front(){
      this->elements.pop_front();
    }

    int size(){
      return this->elements.size();
    }

    void clear(){
      this->elements.clear();
    }

    buffer_element front(){
      return this->elements.front();
    }

    buffer_element back(){
      return this->elements.back();
    }

    std::deque<buffer_element> elements;
    double last_time;
    double mean_size_time;
    double mean_service_time;
    double mean_attempts;

    // Number of connections popped from buffer (allocated succesfully)
    int poped;
    int pushed;

};
