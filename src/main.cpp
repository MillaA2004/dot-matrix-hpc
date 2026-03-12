#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono> 

using namespace std;
using namespace std::chrono;

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
    string file1 = "../data/seq1.fasta";
    string file2 = "../data/seq2.fasta";
    
    cout << "Reading sequences..." << endl;
    
    // test if no file is found, strings
    string seq1 = readFASTA(file1);
    string seq2 = readFASTA(file2);
    
    if (seq1.empty() || seq2.empty()) {
        cout << "Warning: Files not found. Using test data." << endl;
        seq1 = "ACTGACGCAG"; 
        seq2 = "TCGACGTCGT"; 
    }

    int len1 = seq1.length();
    int len2 = seq2.length();
    cout << "Length of Sequence 1: " << len1 << endl;
    cout << "Length of Sequence 2: " << len2 << endl;

    // Prepare file for saving results (for visualization)
    // IMPORTANT: For large files on supercomputers we will exclude this writing,
    // because writing to disk slows down the process!
    bool save_to_file = true; 
    ofstream outFile("output.csv");
    if (save_to_file && outFile.is_open()) {
        outFile << "X,Y\n"; // Header of the CSV file
    }

    // start timer
    auto start_time = high_resolution_clock::now();

    long long match_count = 0; // match count

    // Dot-Matrix algorithm (O(n*m))
    for (int i = 0; i < len1; i++) {
        for (int j = 0; j < len2; j++) {
            if (seq1[i] == seq2[j]) {
                match_count++;
                if (save_to_file && outFile.is_open()) {
                    outFile << i << "," << j << "\n";
                }
            }
        }
    }

    // stop timer
    auto end_time = high_resolution_clock::now();
    duration<double> execution_time = end_time - start_time;

    if (outFile.is_open()) outFile.close();

    // results
    cout << "--------------------------------------" << endl;
    cout << "Number of matches found: " << match_count << endl;
    cout << "Execution time: " << execution_time.count() << " seconds." << endl;
    cout << "--------------------------------------" << endl;

    return 0;
}