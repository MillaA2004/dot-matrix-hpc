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
    cout << "================================================" << endl;
    cout << "   DNA DOT-MATRIX PERFORMANCE BENCHMARK         " << endl;
    cout << "================================================" << endl;
    cout << "Select dataset to analyze:" << endl;
    cout << "1. Test Data (20x20 Fast Debug)" << endl;
    cout << "2. Influenza A (Highly Similar Strains)" << endl;
    cout << "3. p53 Genes (Human vs Mouse)" << endl;
    cout << "4. Swine Flu (Evolutionary Comparison)" << endl;
    cout << "5. Bacteriophages (Large HPC Datasets)" << endl;
    cout << "Choice (1-5): ";
    
    int choice;
    cin >> choice;

    string seq1, seq2;

    switch (choice) {
        case 1:
            cout << "\nLoading 20x20 test data..." << endl;
            seq1 = "CCTAGATCGATCGACTAAGC";
            seq2 = "GGTTGATCGTTCGACTTCGG";
            break;
        case 2:
            cout << "\nLoading highly similar datasets (Influenza A)..." << endl;
            seq1 = readFASTA("src/data/small-debug-influenza-a/influenza_A_1.fasta");
            seq2 = readFASTA("src/data/small-debug-influenza-a/influenza_A_2.fasta");
            break;
        case 3:
            cout << "\nLoading orthologous datasets (p53 Genes)..." << endl;
            seq1 = readFASTA("src/data/small-debug-p53-genes/p53_human.fasta");
            seq2 = readFASTA("src/data/small-debug-p53-genes/p53_mouse.fasta");
            break;
        case 4:
            cout << "\nLoading evolutionary dataset (Swine Flu)..." << endl;
            seq1 = readFASTA("src/data/small-debug-swine-flu/swine_flu_human.fasta"); 
            seq2 = readFASTA("src/data/small-debug-swine-flu/swine_flu_pig.fasta");
            break;
        case 5:
            cout << "\nLoading large datasets for HPC..." << endl;
            seq1 = readFASTA("src/data/large-hpc/phage_lambda_48k.fasta");
            seq2 = readFASTA("src/data/large-hpc/phage_T7_39k.fasta");
            break;
        default:
            cout << "\nInvalid input. Defaulting to 10x10 test data." << endl;
            seq1 = "ACTGACGCAG";
            seq2 = "TCGACGTCGT";
            break;

    if (seq1.empty() || seq2.empty()) {
        cout << "Warning: Sequences empty or files not found. Using 10x10 test data." << endl;
        seq1 = "ACTGACGCAG"; seq2 = "TCGACGTCGT";
    }

    cout << "\n================================================" << endl;
    cout << "Length of Sequence 1: " << seq1.length() << " bp" << endl;
    cout << "Length of Sequence 2: " << seq2.length() << " bp" << endl;

    cout << "\nRunning Serial Algorithm..." << endl;
    MatchResult serial = runSerialDotMatrix(seq1, seq2);
    
    cout << "Running Parallel Algorithm..." << endl;
    MatchResult parallel = runParallelDotMatrix(seq1, seq2);

    cout << fixed << setprecision(6); 
    cout << "------------------------------------------------" << endl;
    cout << "RESULTS:" << endl;
    cout << "Serial   | Significant Matches: " << serial.count << " | Time: " << serial.time << " s" << endl;
    cout << "Parallel | Significant Matches: " << parallel.count << " | Time: " << parallel.time << " s" << endl;
    cout << "------------------------------------------------" << endl;

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

    if (seq1.length() < 5000) {
        cout << "Generating output.csv for Python visualization..." << endl;
        generateCSV(seq1, seq2, "output.csv"); 
        cout << "Done! Saved as output.csv" << endl;
    } else {
        cout << "[Notice] Sequences too large (>5000). CSV generation skipped." << endl;
    }

    return 0;
}