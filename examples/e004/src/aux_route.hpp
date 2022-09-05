#include "./simulator.hpp"
#include <bits/stdc++.h>

class AuxRoute {

public:

    AuxRoute();
    AuxRoute(const std::vector<Link *> &link, int slots, std::string modulation, int modulation_int, double reach, int band_int, std::string band, double bitrate, int totalLinkLength, int routeId);

    ~AuxRoute();

    int getSlots();
    double getReach();
    double getBitrate();
    std::vector<Link *> getLinks();
    std::string getModulation();
    int getModulationInt();
    int getResources();
    int getBandInt();
    std::string getBand();
    int getTotalLength();
    int getRouteId();

private:

    int resources;
    std::vector<Link *> links;
    std::string modulation;
    int modulation_int;
    int slots;
    double reach;
    double bitrate;
    int band_int;
    int routeId;
    int totalLinkLength;
    std::string band;

};

// Functions that help us sort pathsOffline by our preference
bool RMB(AuxRoute *v1, AuxRoute *v2){
    if (v1->getRouteId() != v2->getRouteId()) return v1->getRouteId() < v2->getRouteId();
    if (v1->getModulationInt() != v2->getModulationInt()) return v1->getModulationInt() < v2->getModulationInt();
    return v1->getBandInt() < v2->getBandInt();
}

bool BMR(AuxRoute *v1, AuxRoute *v2){
    if (v1->getBandInt() != v2->getBandInt()) return v1->getBandInt() < v2->getBandInt();
    if (v1->getModulationInt() != v2->getModulationInt()) return v1->getModulationInt() < v2->getModulationInt();
    return v1->getRouteId() < v2->getRouteId();
}

bool BRM(AuxRoute *v1, AuxRoute *v2){
    if (v1->getBandInt() != v2->getBandInt()) return v1->getBandInt() < v2->getBandInt();
    if (v1->getRouteId() != v2->getRouteId()) return v1->getRouteId() < v2->getRouteId();
    return v1->getModulationInt() < v2->getModulationInt();
}

bool RBM(AuxRoute *v1, AuxRoute *v2){
    if (v1->getRouteId() != v2->getRouteId()) return v1->getRouteId() < v2->getRouteId();
    if (v1->getBandInt() != v2->getBandInt()) return v1->getBandInt() < v2->getBandInt();
    return v1->getModulationInt() < v2->getModulationInt();
}

bool MRB(AuxRoute *v1, AuxRoute *v2){
    if (v1->getModulationInt() != v2->getModulationInt()) return v1->getModulationInt() < v2->getModulationInt();
    if (v1->getRouteId() != v2->getRouteId()) return v1->getRouteId() < v2->getRouteId();
    return v1->getBandInt() < v2->getBandInt();
}

bool comByResource(AuxRoute *v1, AuxRoute *v2){
    if (v1->getResources() != v2->getResources()) return v1->getResources() < v2->getResources();
    if (v1->getBandInt() != v2->getBandInt()) return v1->getBandInt() < v2->getBandInt();
    if (v1->getModulationInt() != v2->getModulationInt()) return v1->getModulationInt() < v2->getModulationInt();
    return v1->getRouteId() < v2->getRouteId();
}

bool comByResourceModulationBand(AuxRoute *v1, AuxRoute *v2){
    if (v1->getResources() != v2->getResources()) return v1->getResources() < v2->getResources();
    if (v1->getModulationInt() != v2->getModulationInt()) return v1->getModulationInt() < v2->getModulationInt();
    if (v1->getBandInt() != v2->getBandInt()) return v1->getBandInt() < v2->getBandInt();
    return v1->getRouteId() < v2->getRouteId();
}

bool MBR(AuxRoute *v1, AuxRoute *v2){
    if (v1->getModulationInt() != v2->getModulationInt()) return v1->getModulationInt() < v2->getModulationInt();
    if (v1->getBandInt() != v2->getBandInt()) return v1->getBandInt() < v2->getBandInt();
    return v1->getRouteId() < v2->getRouteId();
}