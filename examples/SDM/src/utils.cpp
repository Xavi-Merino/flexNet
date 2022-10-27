#include <fstream>
#include "./simulator.hpp"
#include <bits/stdc++.h>

// Bitrate map
std::map<float, int> bitRates_map { { 10.0 , 0 }, { 40.0 , 1 }, { 100.0 , 2 }, { 400.0 , 3 }, {1000.0, 4} };

// Calculate BBP
double bandwidthBlockingProbability(double bitrate_count_total[5], 
                                    double bitrate_count_blocked[5],
                                    double mean_weight_bitrate[5],
                                    bool RSA)
    {
    double BBP = 0;
    double BP = 0;
    double total_weight = 0;
    int end = 5;
    if (RSA) end = 4;

    for (int b = 0; b < end; b++){
        total_weight += mean_weight_bitrate[b];
        if (bitrate_count_total[b] == 0) continue;
        BP = bitrate_count_blocked[b] / bitrate_count_total[b];
        BBP += mean_weight_bitrate[b] * BP;
    }

    return (BBP/total_weight);
}

// Result to TXT
void resultsToFile(std::fstream &output, double BBP, double BP, int number_connections,
                   int lambda_index, double earlang, double bitrate_count_blocked[5])
{
  output << "N/Buffer earlang index: " << lambda_index
          << ", earlang: " << earlang
          << ", general blocking: " << BP
          << ", BBP: " << BBP                    
          << '\n';
}