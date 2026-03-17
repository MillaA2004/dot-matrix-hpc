#include "algorithms.h"
#include <omp.h>
#include <chrono>
#include <fstream>  
#include <iostream>

using namespace std;
using namespace std::chrono;

MatchResult runSerialDotMatrix(const string& s1, const string& s2) {
    auto start = high_resolution_clock::now();
    long long matches = 0;

    for (int i = 0; i < s1.length(); i++) {
        for (int j = 0; j < s2.length(); j++) {
            if (s1[i] == s2[j]) matches++;
        }
    }

    auto end = high_resolution_clock::now();
    return {matches, duration<double>(end - start).count()};
}

MatchResult runParallelDotMatrix(const string& s1, const string& s2) {
    auto start = high_resolution_clock::now();
    long long matches = 0;

    #pragma omp parallel for collapse(2) reduction(+:matches)
    for (int i = 0; i < s1.length(); i++) {
        for (int j = 0; j < s2.length(); j++) {
            if (s1[i] == s2[j]) matches++;
        }
    }

    auto end = high_resolution_clock::now();
    return {matches, duration<double>(end - start).count()};
}

void generateCSV(const string& s1, const string& s2, const string& filename) {
    ofstream outFile(filename);
    
    if (!outFile.is_open()) {
        cerr << "Грешка: Не може да се създаде CSV файлът!" << endl;
        return;
    }

    outFile << "X,Y\n"; 

    for (int i = 0; i < s1.length(); i++) {
        for (int j = 0; j < s2.length(); j++) {
            if (s1[i] == s2[j]) {
                outFile << i << "," << j << "\n";
            }
        }
    }

    outFile.close();
}