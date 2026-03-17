#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <string>
#include <vector>

// Structure to hold results for easier analysis
struct MatchResult {
    long long count;
    double time;
};

// Serial (Sequential) version
MatchResult runSerialDotMatrix(const std::string& s1, const std::string& s2);

// Parallel (OpenMP) version
MatchResult runParallelDotMatrix(const std::string& s1, const std::string& s2);

void generateCSV(const std::string& s1, const std::string& s2, const std::string& filename);
#endif