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
        if (line.empty() || line[0] == '>') continue;
        sequence += line;
    }
    
    file.close();
    return sequence;
}

int main() {
    cout << "Select dataset size (type 'test', 'small', or 'large'): ";
    string type;
    cin >> type;

    string seq1, seq2;

    // 1. Set the sequences based on user input
    if (type == "test") {
        cout << "Loading 20x20 test data..." << endl;
        seq1 = "CCTAGATCGATCGACTAAGC";
        seq2 = "GGTTGATCGTTCGACTTCGG";
    } 
    else if(type == "diff"){
        cout<< "Loading different sequences for testing..." << endl;
        seq1 = readFASTA("src/data/diff-debug/p53_human.fasta");
        seq2 = readFASTA("src/data/diff-debug/p53_mouse.fasta");
    }
    else if (type == "small") {
        cout << "Loading small datasets..." << endl;
        seq1 = readFASTA("src/data/small-debug/influenza_A_1.fasta");
        seq2 = readFASTA("src/data/small-debug/influenza_A_2.fasta");
    } 
    else if (type == "large") {
        cout << "Loading large datasets..." << endl;
        seq1 = readFASTA("src/data/large-hpc/phage_lambda_48k.fasta");
        seq2 = readFASTA("src/data/large-hpc/phage_T7_39k.fasta");
    } 
    else {
        cout << "Invalid input. Defaulting to test data." << endl;
        seq1 = "ACTGACGCAG";
        seq2 = "TCGACGTCGT";
    }

    // Fallback if files failed to open
    if (seq1.empty() || seq2.empty()) {
        cout << "Warning: Sequences empty. Using 10x10 test data." << endl;
        seq1 = "ACTGACGCAG"; seq2 = "TCGACGTCGT";
    }

    cout << "\n================================================" << endl;
    cout << "   DNA DOT-MATRIX PERFORMANCE BENCHMARK        " << endl;
    cout << "================================================" << endl;
    cout << "Length of Sequence 1: " << seq1.length() << " bp" << endl;
    cout << "Length of Sequence 2: " << seq2.length() << " bp" << endl;

    // 2. Run the algorithms
    cout << "\nRunning Serial Algorithm..." << endl;
    MatchResult serial = runSerialDotMatrix(seq1, seq2);
    
    cout << "Running Parallel Algorithm..." << endl;
    MatchResult parallel = runParallelDotMatrix(seq1, seq2);

    // 3. Display Results
    cout << fixed << setprecision(6); 
    cout << "------------------------------------------------" << endl;
    cout << "RESULTS:" << endl;
    cout << "Serial   | Significant Matches: " << serial.count << " | Time: " << serial.time << " s" << endl;
    cout << "Parallel | Significant Matches: " << parallel.count << " | Time: " << parallel.time << " s" << endl;
    cout << "------------------------------------------------" << endl;

    // 4. Verification and Speedup
    if (serial.count != parallel.count) {
        cout << "[ERROR] Match count mismatch! Check parallel logic." << endl;
    } else {
        cout << "[Verified] Results are completely consistent." << endl;
    }

    if (parallel.time > 0) {
        double speedup = serial.time / parallel.time;
        cout << "Calculated Speedup: " << speedup << "x" << endl;
    }
    cout << "================================================" << endl;

    // 5. CSV Generation
    if (seq1.length() < 5000) {
        cout << "Generating output.csv for Python visualization..." << endl;
        generateCSV(seq1, seq2, "src/output.csv"); 
        cout << "Done! Saved as output.csv" << endl;
    } else {
        cout << "[Notice] Sequences too large (>5000). CSV generation skipped." << endl;
    }

    return 0;
}