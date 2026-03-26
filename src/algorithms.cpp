#include "algorithms.h"
#include <omp.h>
#include <chrono>
#include <fstream>  
#include <iostream>
#include <cmath>

using namespace std;
using namespace std::chrono;

const int MATCH_SCORE = 1;
const int MISMATCH_PENALTY = -1;
const int GAP_PENALTY = -1;

MatchResult runSerialDotMatrix(const string& s1, const string& s2) {
    auto start = high_resolution_clock::now();
    long long matches = 0;
    long long mismatches = 0;

    for (int i = 0; i < s1.length(); i++) {
        for (int j = 0; j < s2.length(); j++) {
            if (s1[i] == s2[j]) {
                matches++;
            } else {
                mismatches++;
            }
        }
    }
    
    long long gaps = abs((long long)s1.length() - (long long)s2.length());
    long long score = (matches * MATCH_SCORE) + (mismatches * MISMATCH_PENALTY) + (gaps * GAP_PENALTY);
    

    auto end = high_resolution_clock::now();
    return {matches, mismatches, gaps, score, duration<double>(end - start).count()};
}

MatchResult runParallelDotMatrix(const string& s1, const string& s2) {
    auto start = high_resolution_clock::now();
    long long matches = 0;
    long long mismatches = 0;

    #pragma omp parallel for collapse(2) reduction(+:matches, mismatches)
    for (int i = 0; i < s1.length(); i++) {
        for (int j = 0; j < s2.length(); j++) {
            if (s1[i] == s2[j]) {
                matches++;
            } else {
                mismatches++;
            }
        }
    }
    long long gaps = abs((long long)s1.length() - (long long)s2.length());
    long long score = (matches * MATCH_SCORE) + (mismatches * MISMATCH_PENALTY) + (gaps * GAP_PENALTY);

    auto end = high_resolution_clock::now();
    return {matches, mismatches, gaps, score, duration<double>(end - start).count()};
}

void generateCSV(const string& s1, const string& s2, const string& filename) {
    ofstream outFile(filename);
    
    if (!outFile.is_open()) {
        cerr << "Error: Unable to create CSV file!" << endl;
        return;
    }

    outFile << "X,Y,Type,Score\n";

    for (int i = 0; i < s1.length(); i++) {
        for (int j = 0; j < s2.length(); j++) {
            if (s1[i] == s2[j]) {
                outFile << i << "," << j << ",Match," << MATCH_SCORE << "\n";
            } else if (i == j) {
                outFile << i << "," << j << ",Mismatch," << MISMATCH_PENALTY << "\n";
            }
        }
    }

    outFile.close();
}