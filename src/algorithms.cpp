#include "algorithms.h"
#include <omp.h>
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;
using namespace std::chrono;

const int WINDOW_SIZE = 10;
const int THRESHOLD = 8; 

MatchResult runSerialDotMatrix(const string& s1, const string& s2) {
    auto start = high_resolution_clock::now();
    long long significant_matches = 0;

    int len1 = s1.length() - WINDOW_SIZE;
    int len2 = s2.length() - WINDOW_SIZE;

    // Standard serial loop (No OpenMP pragma)
    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            
            int local_matches = 0;
            for (int k = 0; k < WINDOW_SIZE; k++) {
                if (s1[i + k] == s2[j + k]) {
                    local_matches++;
                }
            }

            if (local_matches >= THRESHOLD) {
                significant_matches++;
            }
        }
    }

    auto end = high_resolution_clock::now();
    return {significant_matches, duration<double>(end - start).count()};
}

MatchResult runParallelDotMatrix(const string& s1, const string& s2) {
    auto start = high_resolution_clock::now();
    long long significant_matches = 0;

    int len1 = s1.length() - WINDOW_SIZE;
    int len2 = s2.length() - WINDOW_SIZE;

    #pragma omp parallel for collapse(2) reduction(+:significant_matches) schedule(static)
    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            
            int local_matches = 0;
            for (int k = 0; k < WINDOW_SIZE; k++) {
                if (s1[i + k] == s2[j + k]) {
                    local_matches++;
                }
            }

            if (local_matches >= THRESHOLD) {
                significant_matches++;
            }
        }
    }

    auto end = high_resolution_clock::now();
    return {significant_matches, duration<double>(end - start).count()};
}

void generateCSV(const string& s1, const string& s2, const string& filename) {
    ofstream outFile(filename);

    if (!outFile.is_open()) {
        cerr << "Error: Unable to create CSV file!" << endl;
        return;
    }

    outFile << "X,Y\n";

    int len1 = s1.length() - WINDOW_SIZE;
    int len2 = s2.length() - WINDOW_SIZE;

    for (int i = 0; i <= len1; i++) {
        for (int j = 0; j <= len2; j++) {
            
            int local_matches = 0;
            for (int k = 0; k < WINDOW_SIZE; k++) {
                if (s1[i + k] == s2[j + k]) {
                    local_matches++;
                }
            }

            if (local_matches >= THRESHOLD) {
                outFile << (i + WINDOW_SIZE/2) << "," << (j + WINDOW_SIZE/2) << endl;
            }
        }
    }

    outFile.close();
}