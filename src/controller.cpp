#include "controller.hpp"
#include <fstream>

Controller::Controller(){};

Controller::Controller(Network network){};

Controller::~Controller(){};

int assignConnection(int src, int dst, int numberOfSlots){};

void setPaths(std::string filename) {
      // open JSON file
      std::ifstream file(filename);
      nlohmann::json NSFnet;
      file >> NSFnet;
};