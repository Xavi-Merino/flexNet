#include "controller.hpp"

#include <fstream>

Controller::Controller(){};

Controller::Controller(Network network){};

Controller::~Controller(){};

int Controller::assignConnection(int src, int dst, int numberOfSlots){};

void Controller::setPaths(std::string filename) {
      // open JSON file
      std::ifstream file(filename);
      nlohmann::json NSFnet;
      file >> NSFnet;

      // need to implement getNode_counter()
      // this->network.getNode_counter();
      
};