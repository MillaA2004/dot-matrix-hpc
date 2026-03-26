#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "algorithms.h"

using namespace std;

string readFASTA(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return "";
    }

    string sequence = "";
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '>') {
            continue;
        }
        sequence += line;
    }
    
    file.close();
    return sequence;
}

int main() {
    //paths to FASTA files
    string file1 = "src/data/small-debug/influenza_A_1.fasta";
    string file2 = "src/data/small-debug/influenza_A_2.fasta";
    
    cout << "DNA Dot-Matrix Performance Comparison" << endl;
    
    // test if no file is found, strings
    string seq1 = readFASTA(file1);
    string seq2 = readFASTA(file2);
    
    if (seq1.empty() || seq2.empty()) {
        cout << "Warning: Files not found. Using 10X10 test data." << endl;
        seq1 = "ACTGACGCAG"; 
        seq2 = "TCGACGTCGT"; 
    }

    cout << "Length of Sequence 1: " << seq1.length() << endl;
    cout << "Length of Sequence 2: " << seq2.length() << endl;

    // 1. Run the Serial (Sequential) Walkthrough
    cout << "Running Serial Algorithm..." << endl;
    MatchResult serial = runSerialDotMatrix(seq1, seq2);
    
    // 2. Run the Parallel (OpenMP) Walkthrough
    cout << "Running Parallel Algorithm..." << endl;
    MatchResult parallel = runParallelDotMatrix(seq1, seq2);

    // 3. Display Results
   cout << fixed << setprecision(6); 
    cout << "------------------------------------------------" << endl;
    cout << "RESULTS (Math Formula Output):" << endl;
    cout << "Serial   | Matches: " << serial.matches << " | Mismatches: " << serial.mismatches << " | Gaps: " << serial.gaps << " | SCORE: " << serial.score << " | Time: " << serial.time << " s" << endl;
    cout << "Parallel | Matches: " << parallel.matches << " | Mismatches: " << parallel.mismatches << " | Gaps: " << parallel.gaps << " | SCORE: " << parallel.score << " | Time: " << parallel.time << " s" << endl;
    cout << "------------------------------------------------" << endl;

    // 4. Performance Analysis (Verification and Speedup)
    if (serial.score != parallel.score) {
        cout << "[ERROR] Score mismatch! Check parallel logic." << endl;
    } else {
        cout << "[Verified] Results are mathematically consistent." << endl;
    }

    if (parallel.time > 0) {
        double speedup = serial.time / parallel.time;
        cout << "Calculated Speedup: " << speedup << "x" << endl;
    }

    cout << "================================================" << endl;

    if (seq1.length() < 5000) {
        cout << "Generating output.csv for Python visualization..." << endl;
        generateCSV(seq1, seq2, "output.csv");
        cout << "Done! Saved as output.csv with Types and Scores." << endl;
    }
    return 0;
}