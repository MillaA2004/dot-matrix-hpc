# HPC DNA Dot-Matrix Alignment

## Overview
This project implements a High-Performance Computing (HPC) C++ application that calculates a Dot-Matrix alignment between two DNA sequences (e.g., from FASTA files). The algorithm is designed to identify regions of similarity between genomic sequences.

To handle massive datasets (50,000+ base pairs) efficiently, the core alignment algorithm has been heavily optimized and parallelized using **OpenMP**. The application features an interactive console menu and an automated benchmarking system to compare the execution time of the sequential (serial) algorithm against the parallelized version.

## Project Structure
```text
dot-matrix-hpc/
├── data/                      # Directory for input FASTA files (ignored by Git)
│   ├── small-debug/           # ~1.7k bp sequences (Influenza A)
│   └── large-hpc/             # ~48k bp sequences for stress-testing
├── src/                       # Source code directory
│   ├── main.cpp               # Driver program (Menu, I/O, benchmarking)
│   ├── algorithms.h           # Function declarations and data structures
│   └── algorithms.cpp         # Core mathematical logic (Serial and Parallel)
├── plot.py                    # Python script to visualize the output.csv
├── output.csv                 # Generated coordinate data (Ignored by Git)
└── README.md                  # Project documentation
Prerequisites
To compile and run this project locally, you need:

C++ Compiler: A compiler that supports OpenMP (e.g., g++ via MSYS2/MinGW on Windows, or GCC on Linux).

Python 3 (Optional): Required only if you want to generate the 2D visual scatter plot.

Install the plotting library via terminal: pip install matplotlib

How to Compile and Run
Open your terminal in the root directory of the project (dot-matrix-hpc) and execute the following compilation command:

Windows (PowerShell):

PowerShell
g++ src/main.cpp src/algorithms.cpp -o main -fopenmp -O3 ; .\main.exe
Linux / macOS (Bash):

Bash
g++ src/main.cpp src/algorithms.cpp -o main -fopenmp -O3
./main
Interactive Execution Modes
Upon running the executable, the program will prompt you to select a dataset size. Type one of the following commands and press Enter:

test: Runs a rapid 10x10 hardcoded string comparison to verify basic compiler logic.

small: Loads ~1,700 bp sequences. This mode processes roughly 3 million comparisons and successfully generates the output.csv file required for Python visualization.

large: Loads ~48,000 bp sequences. This mode computes nearly 1.9 billion comparisons to stress-test the CPU and demonstrate true parallel scaling. (Note: CSV file generation is safely disabled in this mode to prevent severe I/O bottlenecks and memory crashes).

Parallel Optimizations Implemented
The algorithms.cpp file contains highly optimized parallel logic designed for deployment on supercomputing clusters. Key optimizations include:

OpenMP Multi-threading: Workload distribution across available CPU cores.

Loop Collapsing (collapse(2)): Merges the nested 2D matrix loops into a single, massive 1D iteration space for better thread utilization.

Thread-Safe Reductions (reduction(+:matches)): Prevents race conditions and safely aggregates the final match count without utilizing slow atomic locks.

SIMD Vectorization (simd): Forces the compiler to utilize hardware vector registers to process multiple DNA characters in a single clock cycle.

Static Scheduling (schedule(static)): Pre-divides the uniform matrix workload into equal chunks at compile-time, eliminating dynamic assignment overhead.

Compiler Optimization (-O3): Aggressive hardware-level optimization applied during the build process.

Interpreting the Benchmark Results
Small Datasets (Parallel Overhead): When running the small dataset, the Serial execution may appear faster than the Parallel execution. The -O3 flag makes the mathematical operations nearly instantaneous, meaning the program spends comparatively more time waking up and assigning OpenMP threads than it does calculating matches.

Large Datasets (True HPC Scaling): When running the large dataset (1.9+ billion operations), the computation time drastically outpaces the thread overhead. On standard local hardware, the parallel architecture demonstrates a 6.7x speedup, dropping execution time from 1.29 seconds down to 0.19 seconds.

Generating the Dot-Matrix Graph
Run the application using the small mode to safely generate the coordinate data. Once the output.csv file is created, you can visualize the DNA alignment by running the Python script from the root directory:

PowerShell
python plot.py
This will generate an influenza_dot_matrix.png image file showing the biological sequence alignments.