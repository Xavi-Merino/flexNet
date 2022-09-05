#include "aux_route.hpp"

AuxRoute::AuxRoute(const std::vector<Link *> &link, int slots, std::string modulation, int modulation_int, double reach, int band_int, std::string band, double bitrate, int totalLinkLength, int routeId){
    this->links = link;
    this->slots = slots;
    this->modulation = modulation;
    this->modulation_int = modulation_int;
    this->reach = reach;
    this->resources = this->slots*link.size();
    this->bitrate = bitrate;
    this->totalLinkLength = totalLinkLength;
    this->routeId = routeId;
    this->band = band;
    this->band_int = band_int;
    };

AuxRoute::AuxRoute(){
    this->links = std::vector<Link *>();
    this->slots = 0;
    this->modulation = std::string();
    this->reach = 0;
    this->resources = 0;
    this->bitrate = 0;
}

AuxRoute::~AuxRoute() {}

int AuxRoute::getSlots(){
    return this->slots;
}

double AuxRoute::getReach(){
    return this->reach;
}

double AuxRoute::getBitrate(){
    return this->bitrate;
}

std::vector<Link *> AuxRoute::getLinks(){
    return this->links;
}

std::string AuxRoute::getModulation(){
    return this->modulation;
}

int AuxRoute::getModulationInt(){
    return this->modulation_int;
}

int AuxRoute::getResources(){
    return this->resources;
}

std::string AuxRoute::getBand(){
    return this->band;
}

int AuxRoute::getBandInt(){
    return this->band_int;
}

int AuxRoute::getTotalLength(){
    return this->totalLinkLength;
}

int AuxRoute::getRouteId(){
    return this->routeId;
}
      