#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono> 
#include <omp.h> 

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

    // Disabled file saving for parallel execution to prevent file corruption
    bool save_to_file = false; 
    ofstream outFile;
    if (save_to_file) {
        outFile.open("output.csv");
        if (outFile.is_open()) outFile << "X,Y\n"; 
    }

    // Print the number of threads OpenMP will use
    int max_threads = omp_get_max_threads();
    cout << "Starting parallel execution using up to " << max_threads << " threads..." << endl;

    // start timer
    auto start_time = high_resolution_clock::now();

    long long match_count = 0; // match count

    // THE MAGIC HAPPENS HERE:
    // collapse(2) merges the two loops into one large parallel space.
    // reduction(+:match_count) creates a safe, private copy of the counter for each thread, 
    // and adds them all together securely at the very end.
    #pragma omp parallel for collapse(2) reduction(+:match_count)
    for (int i = 0; i < len1; i++) {
        for (int j = 0; j < len2; j++) {
            if (seq1[i] == seq2[j]) {
                match_count++;
                
                // Writing to file is skipped during parallel runs
                if (save_to_file && outFile.is_open()) {
                    // In a real HPC scenario, you would not do file I/O inside a parallel loop
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
    cout << "Total of matches found: " << match_count << endl;
    cout << "Execution time: " << execution_time.count() << " seconds." << endl;
    cout << "--------------------------------------" << endl;

    return 0;
}