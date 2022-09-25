#include "./buffer.cpp"

std::map<std::string, int> bands { { "C", 0 }, { "L", 1 }, { "S", 2 }, { "E", 3 }};

std::map<std::string, int> modulations { { "64QAM", 0 }, { "32QAM", 1 }, { "16QAM", 2 }, { "8QAM", 3 }, { "QPSK", 4}, { "BPSK",5 } };
// std::map<std::string, int> modulations { { "64-QAM", 0 }, { "32-QAM", 1 }, { "16-QAM", 2 }, { "8-QAM", 3 }, { "QPSK", 4}, { "BPSK",5 } };

std::map<float, int> bitRates_map { { 10.0 , 0 }, { 40.0 , 1 }, { 100.0 , 2 }, { 400.0 , 3 }, {1000.0, 4} };

// Creates vector that contains all the routes between all nodes by the order specified (o)
std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> offlineApproachOrder(std::vector<BitRate> bitRates,
                                                                std::vector<std::vector<std::vector<std::vector<Link *>>>> *pathsNetwork, int o)
{
    bool print = true;
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
                            if (bands[band] != 0) continue;

                            pathsOffline[b][s][d].push_back(new AuxRoute((*pathsNetwork)[s][d][r],
                                                    bitRates[b].getNumberOfSlots(m),
                                                    modulation,
                                                    modulations[modulation],
                                                    bitRates[b].getReach(m),
                                                    bands[band],
                                                    band,
                                                    bitRates[b].getBitRate(),
                                                    TotalLength,
                                                    r));
                        }
                    }
                }
                // Done with the routes between [s][d] in bitrate b, we sort it by:
                switch (o)
                {
                case 0:
                    std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), RMB);
                    break;
                case 1:
                    std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), BMR);
                    break;
                case 2:
                    std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), BRM);
                    break;
                case 3:
                    std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), RBM);
                    break;
                case 4:
                    std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), MRB);
                    break;
                case 5:
                    std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), comByResource);
                    break;
                case 6:
                    std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), MBR);
                    break;
                case 7:
                    std::sort(pathsOffline[b][s][d].begin(), pathsOffline[b][s][d].end(), comByResourceModulationBand);
                    break;
                }
            }
        }
    }
    return pathsOffline;
}

// CSV out
void offlineApproachCSV(std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline, int o)
{
    // File tittle
    std::fstream fout;
    switch (o)
    {
    case 0:
        remove( "RMB_table.csv");
        fout.open("RMB_table.csv", std::ios::out | std::ios::app);
        break;
    case 1:
        remove( "BMR_table.csv");
        fout.open("BMR_table.csv", std::ios::out | std::ios::app);
        break;
    case 2:
        remove( "BRM_table.csv");
        fout.open("BRM_table.csv", std::ios::out | std::ios::app);
        break;
    case 3:
        remove( "RBM_table.csv");
        fout.open("RBM_table.csv", std::ios::out | std::ios::app);
        break;
    case 4:
        remove( "MRB_table.csv");
        fout.open("MRB_table.csv", std::ios::out | std::ios::app);
        break;
    case 5:
        remove( "RBMSA_table.csv");
        fout.open("RBMSA_table.csv", std::ios::out | std::ios::app);
        break;
    case 6:
        remove( "MBR_table.csv");
        fout.open("MBR_table.csv", std::ios::out | std::ios::app);
        break;
    case 7:
        remove( "RBMSA20_table.csv");
        fout.open("RBMSA20_table.csv", std::ios::out | std::ios::app);
        break;
    }
    // File header
    fout << "src,dst,route_id,bitrate,band,modulation,total_link_length,mean_link_length,reach,n_links,resources,";

    for (int nodes = 0; nodes < pathsOffline[0].size()-1; nodes++) fout << "node_" << nodes << ',';
    fout << "node_" << pathsOffline[0].size()-1;
    fout << '\n';

    // File data
        for (int s = 0; s < pathsOffline[0].size(); s++)
            for (int d = 0; d < pathsOffline[0][s].size(); d++){
                if (s == d) continue;
                for (int b = 0; b < pathsOffline.size(); b++){
                    // Node count
                    for (int r = 0; r < pathsOffline[b][s][d].size(); r++){
                        std::vector<int> node_count(pathsOffline[0][s].size(), 0);
                        float mean_link_length = 0;
                        mean_link_length = pathsOffline[b][s][d][r]->getTotalLength()/pathsOffline[b][s][d][r]->getLinks().size();
                    
                        fout << s << ','
                            << d << ','
                            << pathsOffline[b][s][d][r]->getRouteId() << ','
                            << pathsOffline[b][s][d][r]->getBitrate() << ','
                            << pathsOffline[b][s][d][r]->getBand() << ","
                            << pathsOffline[b][s][d][r]->getModulation() << ','
                            << pathsOffline[b][s][d][r]->getTotalLength() << ','
                            << mean_link_length << ','
                            << pathsOffline[b][s][d][r]->getReach() << ','
                            << pathsOffline[b][s][d][r]->getLinks().size() << ','
                            << pathsOffline[b][s][d][r]->getResources() << ',';
                        for (int l = 0; l < node_count.size()-1; l++) fout << node_count[l] << ',';
                        fout << node_count[node_count.size()-1];
                        fout << '\n';
                    }
                }
            }
    return;
}

// Band slice function aux
void bandSliceParameters(int linkSize, int band, int **arr) {

    *arr = new int[3];
    switch(band) {
        case 0:
            (*arr)[0] = 0;
            (*arr)[1] = 319;
            (*arr)[2] = 320;
            break;
        case 1:
            (*arr)[0] = 344;
            (*arr)[1] = 823;
            (*arr)[2] = 480;
            break;
        case 2:
            (*arr)[0] = 824;
            (*arr)[1] = 1583;
            (*arr)[2] = 760;
            break;
        case 3:
            (*arr)[0] = 1584;
            (*arr)[1] = 2719;
            (*arr)[2] = 1136;
            break;
    }
    return;
}

// Free allocated memory funcion
void offlineApproachFree(std::vector<std::vector<std::vector<std::vector<AuxRoute *>>>> pathsOffline){
    for (int b = 0; b < pathsOffline.size(); b++)
        for (int s = 0; s < pathsOffline[b].size(); s++)
            for (int d = 0; d < pathsOffline[b][s].size(); d++)
                for (int r = 0; r < pathsOffline[b][s][d].size(); r++)
                    delete(pathsOffline[b][s][d][r]);
}


// Calculate BBP
double bandwidthBlockingProbability(double bitrate_count_total[5], 
                                   double bitrate_count_blocked[5],
                                   double mean_weight_bitrate[5])
    {
    double BBP = 0;
    double BP = 0;
    double total_weight = 0;

    for (int b = 0; b < 5; b++){
        BP = bitrate_count_blocked[b] / bitrate_count_total[b];
        BBP += mean_weight_bitrate[b] * BP;
        total_weight += mean_weight_bitrate[b];
    }

    return (BBP/total_weight);
}

// Calculate BBP w/buffer
double bandwidthBlockingProbabilityWBuffer(double bitrate_count_total[5], 
                                           std::deque<buffer_element> buffer,
                                           double mean_weight_bitrate[5])
    {
    double BBP = 0;
    double BP = 0;
    double total_weight = 0;

    double count_blocked[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

    for (int i = 0; i < buffer.size(); i++){
        count_blocked[bitRates_map[buffer[i].bitRate->getBitRate()]] += 1;
    }

    for (int b = 0; b < 5; b++){
        BP = count_blocked[b] / bitrate_count_total[b];
        BBP += mean_weight_bitrate[b] * BP;
        total_weight += mean_weight_bitrate[b];
    }

    return (BBP/total_weight);
}

double bandwidthBlockingProbabilityWBufferV3(double bitrate_count_total[5], 
                                           std::priority_queue<buffer_element> buffer,
                                           double mean_weight_bitrate[5])
    {
    double BBP = 0;
    double BP = 0;
    double total_weight = 0;

    double count_blocked[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

    for (int i = 0; i < buffer.size(); i++){
        count_blocked[bitRates_map[buffer.top().bitRate->getBitRate()]] += 1;
        buffer.pop();
    }

    for (int b = 0; b < 5; b++){
        BP = count_blocked[b] / bitrate_count_total[b];
        BBP += mean_weight_bitrate[b] * BP;
        total_weight += mean_weight_bitrate[b];
    }

    return (BBP/total_weight);
}

// Result to TXT
void resultsToFile(bool buffer_state, std::fstream &output, double BBP, double BP,
                   int order, int earlang, double bitrate_count_blocked[5], Buffer buffer)
{
    double avgAttempts = buffer.mean_attempts/buffer.poped;
    double avgService = buffer.mean_service_time/buffer.poped;
    double avgSize = buffer.mean_size_time/buffer.last_time;

    switch (buffer_state){
        case false:
            // output info to txt:
            output << "N/Buffer orden: " << order 
                    << ", earlang: " << earlang 
                    << ", BBP: " << BBP 
                    << ", general blocking: " << BP 
                    << '\n';
            break;
        case true:
            if (buffer.size() == 0) std::cout << "\nNo elements in buffer! :P\n";
            // output info to txt:
            output << "W/Buffer orden: " << order 
                    << ", earlang: " << earlang << ", BBP: " << BBP 
                    << ", general blocking: " << (buffer.size()/(1e6)) 
                    << ", general blocking (original): " << BP
                    << ", buffer size: " << buffer.size() 
                    << ", reallocated: " << buffer.poped 
                    << ", Average try per allocated element: " << avgAttempts
                    << ", Average service time: " << avgService
                    << ", Average buffer size: " << avgSize
                    << '\n';
            break;
        }
}

void resultsToFileV3(bool buffer_state, std::fstream &output, double BBP, double BP,
                   int order, int earlang, int poped, std::priority_queue<buffer_element> buffer, 
                   float tried_times, double bitrate_count_blocked[5])
{
    switch (buffer_state){
        case false:
            // output info to txt:
            output << "N/Buffer orden: " << order 
                    << ", earlang: " << earlang 
                    << ", BBP: " << BBP 
                    << ", general blocking: " << BP 
                    << '\n';
            break;
        case true:
            if (buffer.size() == 0) std::cout << "\nNo elements in buffer! :P\n";
            // output info to txt:
            output << "W/Buffer orden: " << order 
                    << ", earlang: " << earlang << ", BBP: " << BBP 
                    << ", general blocking: " << (buffer.size()/(1e6)) 
                    << ", general blocking (original): " << BP
                    << ", buffer size: " << buffer.size() 
                    << ", reallocated: " << poped 
                    << ", Average try per allocated element: " << tried_times 
                    << '\n';
            break;
        }
}