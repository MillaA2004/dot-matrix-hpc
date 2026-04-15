# 🧬 HPC DNA Dot-Matrix Alignment

## 📌 Overview
This project implements a High-Performance Computing (HPC) C++ application that calculates a Dot-Matrix alignment between two DNA sequences (FASTA format). The algorithm is designed to identify regions of evolutionary homology and similarity between genomic sequences.

To handle massive datasets efficiently (up to 50,000+ base pairs), the core alignment algorithm is heavily optimized and parallelized using **OpenMP**. The application features an interactive Command Line Interface (CLI) menu and an automated benchmarking system to compare the execution time of the sequential (serial) algorithm against the parallelized version.

To filter out random biological noise and provide a clean visualization, the algorithm implements a **Sliding Window** technique (Window Size: 10, Threshold: 8).

## 📁 Project Structure
```text
dot-matrix-hpc/
├── src/                       # Source code directory
│   ├── main.cpp               # Driver program (Interactive CLI Menu, benchmarking)
│   ├── algorithms.h           # Function declarations and data structures
│   ├── algorithms.cpp         # Core mathematical logic (Serial and Parallel OpenMP)
│   └── data/                  # Directory for input FASTA files (ignored by Git)
│       ├── large-hpc/                 # ~48k bp sequences (Bacteriophages)
│       ├── small-debug-influenza-a/   # ~1.7k bp sequences (Similar Influenza Strains)
│       ├── small-debug-p53-genes/     # ~2.5k bp sequences (Human vs Mouse p53)
│       └── small-debug-swine-flu/     # ~1.7k bp sequences (Human vs Swine H1N1)
├── .gitignore                 # Excludes binaries, output.csv, and large FASTA datasets
├── Dot_matrix_hpc.ipynb       # Colab/Python script to visualize the output.csv
└── README.md                  # Project documentation
```
## ⚙️ Prerequisites
To compile and run this project, you need:
1. **C++ Compiler:** A compiler that supports OpenMP (e.g., `g++` via MSYS2/MinGW on Windows, or GCC on Linux).
2. **Python 3 (Optional):** Required only to generate the 2D visual scatter plot.
   * *Install plotting libraries:* `pip install pandas matplotlib`

## 🚀 How to Compile and Run
Open your terminal in the root directory of the project and execute the following commands:

**Windows (PowerShell/CMD):**
```bash
g++ -fopenmp src/main.cpp src/algorithms.cpp -o main.exe
.\main.exe
```
**Linux / macOS:**
```bash
g++ -fopenmp src/main.cpp src/algorithms.cpp -o main
./main
```
## 📊 Execution Modes (Interactive Menu)
Upon running the executable, the program presents a `switch-case` menu. Select the dataset to analyze:
1. **Test Data (20x20 Fast Debug):** Rapid string comparison to verify basic compiler logic.
2. **Influenza A:** Compares two closely related strains (~1,700 bp) showing near-perfect alignment.
3. **p53 Genes:** Analyzes orthologous sequences (Human vs Mouse) to find conserved genetic regions.
4. **Swine Flu:** Compares the Hemagglutinin (HA) gene of Human vs Swine H1N1.
5. **Bacteriophages (HPC Mode):** Loads massive ~48,000 bp sequences. Computes nearly 2 billion comparisons to stress-test the CPU and demonstrate true parallel scaling (CSV output is disabled here to prevent memory crashes).

## 📈 Generating the Dot-Matrix Graph
Run the application using options **2, 3, or 4** to generate the coordinate data (`output.csv`). Once created, visualize the DNA alignment by running the .ipynb file in the Colab environment
