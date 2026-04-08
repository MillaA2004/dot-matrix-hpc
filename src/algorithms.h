#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <string>
#include <vector>

struct MatchResult {
    long long count;
    double time;
};

MatchResult runSerialDotMatrix(const std::string& s1, const std::string& s2);

MatchResult runParallelDotMatrix(const std::string& s1, const std::string& s2);

void generateCSV(const std::string& s1, const std::string& s2, const std::string& filename);
#endif