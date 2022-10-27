#include "offline_approach.cpp"

// Vector of shortest lengths per route
std::vector<double> shortestLengths(std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline){
    std::vector<double> lengths;
    for (int source = 0; source < pathsOffline[0].size(); source++){
        double min_length = INFINITY;
        for (int destination = 0; destination < pathsOffline[0].size(); destination++){
            if (source == destination) continue;
            //double min_length = INFINITY;
            for (int route = 0; route < pathsOffline[0][source][destination].size(); route++){
                if (pathsOffline[0][source][destination][route]->getTotalLength() <= min_length){
                    min_length = pathsOffline[0][source][destination][route]->getTotalLength();
                }
            }
            lengths.push_back(min_length);
        }
    }
    std::sort(lengths.begin(), lengths.end());
    return lengths;
}

//  For Variant 1 proposal
std::map<std::string, int> bands_v1_set1 { { "L", 0 }, { "C", 1 }, { "S", 2 }, { "E", 3 }};
std::map<std::string, int> bands_v1_set2 { { "E", 0 }, { "S", 1 }, { "C", 2 }, { "L", 3 }};

// SET 1
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> offlineApproachOrder_V1_Set1(std::vector<BitRate> bitRates,
                                                                std::vector<std::vector<std::vector<std::vector<Link *>>>> *pathsNetwork)
{
    std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline = std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>>(bitRates.size());
    for (int b = 0; b < bitRates.size(); b++){ // For bitRate
        pathsOffline[b] = std::vector<std::vector<std::vector<AuxRoute *>>>((*pathsNetwork).size());
        for (int s = 0; s < pathsNetwork->size(); 
            s++){ // For SRC s
            pathsOffline[b][s] = std::vector<std::vector<AuxRoute *>>((*pathsNetwork)[s].size());
            for (int d = 0; d < (*pathsNetwork)[s].size();
                d++){ // For DST d
                if (s == d) continue; // IF SRC = DST, skip
                pathsOffline[b][s][d] = std::vector<AuxRoute *>();
                for (int r = 0; r < (*pathsNetwork)[s][d].size();
                    r++){ // For every ROUTE r between s and d

                    // We store the total length of the route r
                    double TotalLength = 0;
                    for (int l = 0; l < (*pathsNetwork)[s][d][r].size(); l++)
                            TotalLength += (*pathsNetwork)[s][d][r][l]->getLength();
                    
                    for (int m = 0; m < bitRates[b].getNumberOfModulations();
                        m++){ // For every modulation m in bitrate b
                        if (bitRates[b].getReach(m) >= TotalLength){ // Check if the reach of modulation m meets requirements of total length of route

                            // if does, we create Auxroute object of route r, bitrate b and modulation m
                            // and we add it to pathsOffline in [bitrate][source][destination] indexes

                            // but first we separate modulation from band strings and store it accordingly
                            std::string modulation_and_band = bitRates[b].getModulation(m);

                            // previously was 'BPSK/C'
                            //std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('/'));
                            //modulation_and_band.erase(0, modulation_and_band.find('/') + 1);
                            //std::string band = modulation_and_band.substr(0, modulation_and_band.find('/'));

                            // now is 'BPSK-C' ... depends of json format
                            std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            modulation_and_band.erase(0, modulation_and_band.find('-') + 1);
                            std::string band = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            
                            // Select the bands we want to work..in this case only C:
                            // if (bands[band] != 0) continue;

                            pathsOffline[b][s][d].push_back(new AuxRoute((*pathsNetwork)[s][d][r],
                                                    bitRates[b].getNumberOfSlots(m),
                                                    modulation,
                                                    modulations[modulation],
                                                    bitRates[b].getReach(m),
                                                    bands_v1_set1[band],
                                                    band,
                                                    bitRates[b].getBitRate(),
                                                    TotalLength,
                                                    r));
                        }
                    }
                }
                // Done with the routes between [s][d] in bitrate b we sort routes:
                std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), RBM_V);
            }
        }
    }
    return pathsOffline;
}

// SET 2
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> offlineApproachOrder_V1_Set2(std::vector<BitRate> bitRates,
                                                                std::vector<std::vector<std::vector<std::vector<Link *>>>> *pathsNetwork)
{
    std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline = std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>>(bitRates.size());
    for (int b = 0; b < bitRates.size(); b++){ // For bitRate
        pathsOffline[b] = std::vector<std::vector<std::vector<AuxRoute *>>>((*pathsNetwork).size());
        for (int s = 0; s < pathsNetwork->size(); 
            s++){ // For SRC s
            pathsOffline[b][s] = std::vector<std::vector<AuxRoute *>>((*pathsNetwork)[s].size());
            for (int d = 0; d < (*pathsNetwork)[s].size();
                d++){ // For DST d
                if (s == d) continue; // IF SRC = DST, skip
                pathsOffline[b][s][d] = std::vector<AuxRoute *>();
                for (int r = 0; r < (*pathsNetwork)[s][d].size();
                    r++){ // For every ROUTE r between s and d

                    // We store the total length of the route r
                    double TotalLength = 0;
                    for (int l = 0; l < (*pathsNetwork)[s][d][r].size(); l++)
                            TotalLength += (*pathsNetwork)[s][d][r][l]->getLength();
                    
                    for (int m = 0; m < bitRates[b].getNumberOfModulations();
                        m++){ // For every modulation m in bitrate b
                        if (bitRates[b].getReach(m) >= TotalLength){ // Check if the reach of modulation m meets requirements of total length of route

                            // if does, we create Auxroute object of route r, bitrate b and modulation m
                            // and we add it to pathsOffline in [bitrate][source][destination] indexes

                            // but first we separate modulation from band strings and store it accordingly
                            std::string modulation_and_band = bitRates[b].getModulation(m);

                            // previously was 'BPSK/C'
                            //std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('/'));
                            //modulation_and_band.erase(0, modulation_and_band.find('/') + 1);
                            //std::string band = modulation_and_band.substr(0, modulation_and_band.find('/'));

                            // now is 'BPSK-C' ... depends of json format
                            std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            modulation_and_band.erase(0, modulation_and_band.find('-') + 1);
                            std::string band = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            
                            // Select the bands we want to work..in this case only C:
                            // if (bands[band] != 0) continue;

                            pathsOffline[b][s][d].push_back(new AuxRoute((*pathsNetwork)[s][d][r],
                                                    bitRates[b].getNumberOfSlots(m),
                                                    modulation,
                                                    modulations[modulation],
                                                    bitRates[b].getReach(m),
                                                    bands_v1_set2[band],
                                                    band,
                                                    bitRates[b].getBitRate(),
                                                    TotalLength,
                                                    r));
                        }
                    }
                }
                // Done with the routes between [s][d] in bitrate b we sort routes:
                std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), RBM_V);
            }
        }
    }
    return pathsOffline;
}

// Calculate median
double calculateMedian(std::vector<double> lengths)
{
  size_t size = lengths.size();

  if (size == 0)
  {
    return 0;  // Undefined, really.
  }
  else
  {
    sort(lengths.begin(), lengths.end());
    if (size % 2 == 0)
    {
      return (lengths[size / 2 - 1] + lengths[size / 2]) / 2;
    }
    else 
    {
      return lengths[size / 2];
    }
  }
}

//  For Variant 2 proposal
std::map<std::string, int> bands_v2_set1 { { "E", 0 }, { "S", 1 }, { "C", 2 }, { "L", 3 }};
std::map<std::string, int> bands_v2_set2 { { "S", 0 }, { "C", 1 }, { "L", 2 }, { "E", 3 }};
std::map<std::string, int> bands_v2_set3 { { "C", 0 }, { "L", 1 }, { "E", 2 }, { "S", 3 }};
std::map<std::string, int> bands_v2_set4 { { "L", 0 }, { "E", 1 }, { "S", 2 }, { "C", 3 }};

// SET 1
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> offlineApproachOrder_V2_Set1(std::vector<BitRate> bitRates,
                                                                std::vector<std::vector<std::vector<std::vector<Link *>>>> *pathsNetwork)
{
    std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline = std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>>(bitRates.size());
    for (int b = 0; b < bitRates.size(); b++){ // For bitRate
        pathsOffline[b] = std::vector<std::vector<std::vector<AuxRoute *>>>((*pathsNetwork).size());
        for (int s = 0; s < pathsNetwork->size(); 
            s++){ // For SRC s
            pathsOffline[b][s] = std::vector<std::vector<AuxRoute *>>((*pathsNetwork)[s].size());
            for (int d = 0; d < (*pathsNetwork)[s].size();
                d++){ // For DST d
                if (s == d) continue; // IF SRC = DST, skip
                pathsOffline[b][s][d] = std::vector<AuxRoute *>();
                for (int r = 0; r < (*pathsNetwork)[s][d].size();
                    r++){ // For every ROUTE r between s and d

                    // We store the total length of the route r
                    double TotalLength = 0;
                    for (int l = 0; l < (*pathsNetwork)[s][d][r].size(); l++)
                            TotalLength += (*pathsNetwork)[s][d][r][l]->getLength();
                    
                    for (int m = 0; m < bitRates[b].getNumberOfModulations();
                        m++){ // For every modulation m in bitrate b
                        if (bitRates[b].getReach(m) >= TotalLength){ // Check if the reach of modulation m meets requirements of total length of route

                            // if does, we create Auxroute object of route r, bitrate b and modulation m
                            // and we add it to pathsOffline in [bitrate][source][destination] indexes

                            // but first we separate modulation from band strings and store it accordingly
                            std::string modulation_and_band = bitRates[b].getModulation(m);

                            // previously was 'BPSK/C'
                            //std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('/'));
                            //modulation_and_band.erase(0, modulation_and_band.find('/') + 1);
                            //std::string band = modulation_and_band.substr(0, modulation_and_band.find('/'));

                            // now is 'BPSK-C' ... depends of json format
                            std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            modulation_and_band.erase(0, modulation_and_band.find('-') + 1);
                            std::string band = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            
                            // Select the bands we want to work..in this case only C:
                            // if (bands[band] != 0) continue;

                            pathsOffline[b][s][d].push_back(new AuxRoute((*pathsNetwork)[s][d][r],
                                                    bitRates[b].getNumberOfSlots(m),
                                                    modulation,
                                                    modulations[modulation],
                                                    bitRates[b].getReach(m),
                                                    bands_v2_set1[band],
                                                    band,
                                                    bitRates[b].getBitRate(),
                                                    TotalLength,
                                                    r));
                        }
                    }
                }
                // Done with the routes between [s][d] in bitrate b we sort routes:
                std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), RBM_V);
            }
        }
    }
    return pathsOffline;
}

// SET 2
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> offlineApproachOrder_V2_Set2(std::vector<BitRate> bitRates,
                                                                std::vector<std::vector<std::vector<std::vector<Link *>>>> *pathsNetwork)
{
    std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline = std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>>(bitRates.size());
    for (int b = 0; b < bitRates.size(); b++){ // For bitRate
        pathsOffline[b] = std::vector<std::vector<std::vector<AuxRoute *>>>((*pathsNetwork).size());
        for (int s = 0; s < pathsNetwork->size(); 
            s++){ // For SRC s
            pathsOffline[b][s] = std::vector<std::vector<AuxRoute *>>((*pathsNetwork)[s].size());
            for (int d = 0; d < (*pathsNetwork)[s].size();
                d++){ // For DST d
                if (s == d) continue; // IF SRC = DST, skip
                pathsOffline[b][s][d] = std::vector<AuxRoute *>();
                for (int r = 0; r < (*pathsNetwork)[s][d].size();
                    r++){ // For every ROUTE r between s and d

                    // We store the total length of the route r
                    double TotalLength = 0;
                    for (int l = 0; l < (*pathsNetwork)[s][d][r].size(); l++)
                            TotalLength += (*pathsNetwork)[s][d][r][l]->getLength();
                    
                    for (int m = 0; m < bitRates[b].getNumberOfModulations();
                        m++){ // For every modulation m in bitrate b
                        if (bitRates[b].getReach(m) >= TotalLength){ // Check if the reach of modulation m meets requirements of total length of route

                            // if does, we create Auxroute object of route r, bitrate b and modulation m
                            // and we add it to pathsOffline in [bitrate][source][destination] indexes

                            // but first we separate modulation from band strings and store it accordingly
                            std::string modulation_and_band = bitRates[b].getModulation(m);

                            // previously was 'BPSK/C'
                            //std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('/'));
                            //modulation_and_band.erase(0, modulation_and_band.find('/') + 1);
                            //std::string band = modulation_and_band.substr(0, modulation_and_band.find('/'));

                            // now is 'BPSK-C' ... depends of json format
                            std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            modulation_and_band.erase(0, modulation_and_band.find('-') + 1);
                            std::string band = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            
                            // Select the bands we want to work..in this case only C:
                            // if (bands[band] != 0) continue;

                            pathsOffline[b][s][d].push_back(new AuxRoute((*pathsNetwork)[s][d][r],
                                                    bitRates[b].getNumberOfSlots(m),
                                                    modulation,
                                                    modulations[modulation],
                                                    bitRates[b].getReach(m),
                                                    bands_v2_set2[band],
                                                    band,
                                                    bitRates[b].getBitRate(),
                                                    TotalLength,
                                                    r));
                        }
                    }
                }
                // Done with the routes between [s][d] in bitrate b we sort routes:
                std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), RBM_V);
            }
        }
    }
    return pathsOffline;
}

// SET 3
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> offlineApproachOrder_V2_Set3(std::vector<BitRate> bitRates,
                                                                std::vector<std::vector<std::vector<std::vector<Link *>>>> *pathsNetwork)
{
    std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline = std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>>(bitRates.size());
    for (int b = 0; b < bitRates.size(); b++){ // For bitRate
        pathsOffline[b] = std::vector<std::vector<std::vector<AuxRoute *>>>((*pathsNetwork).size());
        for (int s = 0; s < pathsNetwork->size(); 
            s++){ // For SRC s
            pathsOffline[b][s] = std::vector<std::vector<AuxRoute *>>((*pathsNetwork)[s].size());
            for (int d = 0; d < (*pathsNetwork)[s].size();
                d++){ // For DST d
                if (s == d) continue; // IF SRC = DST, skip
                pathsOffline[b][s][d] = std::vector<AuxRoute *>();
                for (int r = 0; r < (*pathsNetwork)[s][d].size();
                    r++){ // For every ROUTE r between s and d

                    // We store the total length of the route r
                    double TotalLength = 0;
                    for (int l = 0; l < (*pathsNetwork)[s][d][r].size(); l++)
                            TotalLength += (*pathsNetwork)[s][d][r][l]->getLength();
                    
                    for (int m = 0; m < bitRates[b].getNumberOfModulations();
                        m++){ // For every modulation m in bitrate b
                        if (bitRates[b].getReach(m) >= TotalLength){ // Check if the reach of modulation m meets requirements of total length of route

                            // if does, we create Auxroute object of route r, bitrate b and modulation m
                            // and we add it to pathsOffline in [bitrate][source][destination] indexes

                            // but first we separate modulation from band strings and store it accordingly
                            std::string modulation_and_band = bitRates[b].getModulation(m);

                            // previously was 'BPSK/C'
                            //std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('/'));
                            //modulation_and_band.erase(0, modulation_and_band.find('/') + 1);
                            //std::string band = modulation_and_band.substr(0, modulation_and_band.find('/'));

                            // now is 'BPSK-C' ... depends of json format
                            std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            modulation_and_band.erase(0, modulation_and_band.find('-') + 1);
                            std::string band = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            
                            // Select the bands we want to work..in this case only C:
                            // if (bands[band] != 0) continue;

                            pathsOffline[b][s][d].push_back(new AuxRoute((*pathsNetwork)[s][d][r],
                                                    bitRates[b].getNumberOfSlots(m),
                                                    modulation,
                                                    modulations[modulation],
                                                    bitRates[b].getReach(m),
                                                    bands_v2_set3[band],
                                                    band,
                                                    bitRates[b].getBitRate(),
                                                    TotalLength,
                                                    r));
                        }
                    }
                }
                // Done with the routes between [s][d] in bitrate b we sort routes:
                std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), RBM_V);
            }
        }
    }
    return pathsOffline;
}

// SET 4
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> offlineApproachOrder_V2_Set4(std::vector<BitRate> bitRates,
                                                                std::vector<std::vector<std::vector<std::vector<Link *>>>> *pathsNetwork)
{
    std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline = std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>>(bitRates.size());
    for (int b = 0; b < bitRates.size(); b++){ // For bitRate
        pathsOffline[b] = std::vector<std::vector<std::vector<AuxRoute *>>>((*pathsNetwork).size());
        for (int s = 0; s < pathsNetwork->size(); 
            s++){ // For SRC s
            pathsOffline[b][s] = std::vector<std::vector<AuxRoute *>>((*pathsNetwork)[s].size());
            for (int d = 0; d < (*pathsNetwork)[s].size();
                d++){ // For DST d
                if (s == d) continue; // IF SRC = DST, skip
                pathsOffline[b][s][d] = std::vector<AuxRoute *>();
                for (int r = 0; r < (*pathsNetwork)[s][d].size();
                    r++){ // For every ROUTE r between s and d

                    // We store the total length of the route r
                    double TotalLength = 0;
                    for (int l = 0; l < (*pathsNetwork)[s][d][r].size(); l++)
                            TotalLength += (*pathsNetwork)[s][d][r][l]->getLength();
                    
                    for (int m = 0; m < bitRates[b].getNumberOfModulations();
                        m++){ // For every modulation m in bitrate b
                        if (bitRates[b].getReach(m) >= TotalLength){ // Check if the reach of modulation m meets requirements of total length of route

                            // if does, we create Auxroute object of route r, bitrate b and modulation m
                            // and we add it to pathsOffline in [bitrate][source][destination] indexes

                            // but first we separate modulation from band strings and store it accordingly
                            std::string modulation_and_band = bitRates[b].getModulation(m);

                            // previously was 'BPSK/C'
                            //std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('/'));
                            //modulation_and_band.erase(0, modulation_and_band.find('/') + 1);
                            //std::string band = modulation_and_band.substr(0, modulation_and_band.find('/'));

                            // now is 'BPSK-C' ... depends of json format
                            std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            modulation_and_band.erase(0, modulation_and_band.find('-') + 1);
                            std::string band = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            
                            // Select the bands we want to work..in this case only C:
                            // if (bands[band] != 0) continue;

                            pathsOffline[b][s][d].push_back(new AuxRoute((*pathsNetwork)[s][d][r],
                                                    bitRates[b].getNumberOfSlots(m),
                                                    modulation,
                                                    modulations[modulation],
                                                    bitRates[b].getReach(m),
                                                    bands_v2_set4[band],
                                                    band,
                                                    bitRates[b].getBitRate(),
                                                    TotalLength,
                                                    r));
                        }
                    }
                }
                // Done with the routes between [s][d] in bitrate b we sort routes:
                std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), RBM_V);
            }
        }
    }
    return pathsOffline;
}

//  For Variant 2 proposal
std::map<std::string, int> bands_v3_set1 { { "E", 0 }, { "L", 1 }, { "S", 2 }, { "C", 3 }};
std::map<std::string, int> bands_v3_set2 { { "C", 0 }, { "S", 1 }, { "L", 2 }, { "E", 3 }};

// SET 1
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> offlineApproachOrder_V3_Set1(std::vector<BitRate> bitRates,
                                                                std::vector<std::vector<std::vector<std::vector<Link *>>>> *pathsNetwork)
{
    std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline = std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>>(bitRates.size());
    for (int b = 0; b < bitRates.size(); b++){ // For bitRate
        pathsOffline[b] = std::vector<std::vector<std::vector<AuxRoute *>>>((*pathsNetwork).size());
        for (int s = 0; s < pathsNetwork->size(); 
            s++){ // For SRC s
            pathsOffline[b][s] = std::vector<std::vector<AuxRoute *>>((*pathsNetwork)[s].size());
            for (int d = 0; d < (*pathsNetwork)[s].size();
                d++){ // For DST d
                if (s == d) continue; // IF SRC = DST, skip
                pathsOffline[b][s][d] = std::vector<AuxRoute *>();
                for (int r = 0; r < (*pathsNetwork)[s][d].size();
                    r++){ // For every ROUTE r between s and d

                    // We store the total length of the route r
                    double TotalLength = 0;
                    for (int l = 0; l < (*pathsNetwork)[s][d][r].size(); l++)
                            TotalLength += (*pathsNetwork)[s][d][r][l]->getLength();
                    
                    for (int m = 0; m < bitRates[b].getNumberOfModulations();
                        m++){ // For every modulation m in bitrate b
                        if (bitRates[b].getReach(m) >= TotalLength){ // Check if the reach of modulation m meets requirements of total length of route

                            // if does, we create Auxroute object of route r, bitrate b and modulation m
                            // and we add it to pathsOffline in [bitrate][source][destination] indexes

                            // but first we separate modulation from band strings and store it accordingly
                            std::string modulation_and_band = bitRates[b].getModulation(m);

                            // previously was 'BPSK/C'
                            //std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('/'));
                            //modulation_and_band.erase(0, modulation_and_band.find('/') + 1);
                            //std::string band = modulation_and_band.substr(0, modulation_and_band.find('/'));

                            // now is 'BPSK-C' ... depends of json format
                            std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            modulation_and_band.erase(0, modulation_and_band.find('-') + 1);
                            std::string band = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            
                            // Select the bands we want to work..in this case only C:
                            // if (bands[band] != 0) continue;

                            pathsOffline[b][s][d].push_back(new AuxRoute((*pathsNetwork)[s][d][r],
                                                    bitRates[b].getNumberOfSlots(m),
                                                    modulation,
                                                    modulations[modulation],
                                                    bitRates[b].getReach(m),
                                                    bands_v3_set1[band],
                                                    band,
                                                    bitRates[b].getBitRate(),
                                                    TotalLength,
                                                    r));
                        }
                    }
                }
                // Done with the routes between [s][d] in bitrate b we sort routes:
                std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), RBM_V);
            }
        }
    }
    return pathsOffline;
}

// SET 2
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> offlineApproachOrder_V3_Set2(std::vector<BitRate> bitRates,
                                                                std::vector<std::vector<std::vector<std::vector<Link *>>>> *pathsNetwork)
{
    std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline = std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>>(bitRates.size());
    for (int b = 0; b < bitRates.size(); b++){ // For bitRate
        pathsOffline[b] = std::vector<std::vector<std::vector<AuxRoute *>>>((*pathsNetwork).size());
        for (int s = 0; s < pathsNetwork->size(); 
            s++){ // For SRC s
            pathsOffline[b][s] = std::vector<std::vector<AuxRoute *>>((*pathsNetwork)[s].size());
            for (int d = 0; d < (*pathsNetwork)[s].size();
                d++){ // For DST d
                if (s == d) continue; // IF SRC = DST, skip
                pathsOffline[b][s][d] = std::vector<AuxRoute *>();
                for (int r = 0; r < (*pathsNetwork)[s][d].size();
                    r++){ // For every ROUTE r between s and d

                    // We store the total length of the route r
                    double TotalLength = 0;
                    for (int l = 0; l < (*pathsNetwork)[s][d][r].size(); l++)
                            TotalLength += (*pathsNetwork)[s][d][r][l]->getLength();
                    
                    for (int m = 0; m < bitRates[b].getNumberOfModulations();
                        m++){ // For every modulation m in bitrate b
                        if (bitRates[b].getReach(m) >= TotalLength){ // Check if the reach of modulation m meets requirements of total length of route

                            // if does, we create Auxroute object of route r, bitrate b and modulation m
                            // and we add it to pathsOffline in [bitrate][source][destination] indexes

                            // but first we separate modulation from band strings and store it accordingly
                            std::string modulation_and_band = bitRates[b].getModulation(m);

                            // previously was 'BPSK/C'
                            //std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('/'));
                            //modulation_and_band.erase(0, modulation_and_band.find('/') + 1);
                            //std::string band = modulation_and_band.substr(0, modulation_and_band.find('/'));

                            // now is 'BPSK-C' ... depends of json format
                            std::string modulation = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            modulation_and_band.erase(0, modulation_and_band.find('-') + 1);
                            std::string band = modulation_and_band.substr(0, modulation_and_band.find('-'));
                            
                            // Select the bands we want to work..in this case only C:
                            // if (bands[band] != 0) continue;

                            pathsOffline[b][s][d].push_back(new AuxRoute((*pathsNetwork)[s][d][r],
                                                    bitRates[b].getNumberOfSlots(m),
                                                    modulation,
                                                    modulations[modulation],
                                                    bitRates[b].getReach(m),
                                                    bands_v3_set2[band],
                                                    band,
                                                    bitRates[b].getBitRate(),
                                                    TotalLength,
                                                    r));
                        }
                    }
                }
                // Done with the routes between [s][d] in bitrate b we sort routes:
                std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), RBM_V);
            }
        }
    }
    return pathsOffline;
}