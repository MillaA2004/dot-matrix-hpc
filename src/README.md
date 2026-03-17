# HPC DNA Dot-Matrix Alignment

## Overview
This project implements a High-Performance Computing (HPC) C++ application that calculates a Dot-Matrix alignment between two DNA sequences (e.g., from FASTA files). The algorithm is designed to identify regions of similarity between genomic sequences.

To handle massive datasets (50,000+ base pairs) efficiently, the core alignment algorithm has been heavily optimized and parallelized using **OpenMP**. The application also includes an automated benchmarking system to compare the execution time of the sequential (serial) algorithm against the parallelized version.

## Project Structure
```text
dot-matrix-hpc/
├── data/                      # Directory for input FASTA files (ignored by Git)
│   └── small-debug/
│       ├── influenza_A_1.fasta
│       └── influenza_A_2.fasta
├── src/                       # Source code directory
│   ├── main.cpp               # Driver program (I/O, benchmarking, execution)
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
Open your terminal in the root directory of the project (dot-matrix-hpc) and execute the following command:

Windows (PowerShell):

PowerShell
g++ src/main.cpp src/algorithms.cpp -o main -fopenmp -O3 ; .\main.exe
Linux / macOS:

Bash
g++ src/main.cpp src/algorithms.cpp -o main -fopenmp -O3
./main
Parallel Optimizations Implemented
The algorithms.cpp file contains highly optimized parallel logic designed for deployment on supercomputing clusters. Key optimizations include:

OpenMP Multi-threading: Workload distribution across available CPU cores.

Loop Collapsing (collapse(2)): Merges the nested 2D matrix loops into a single, massive 1D iteration space for better thread utilization.

Thread-Safe Reductions (reduction(+:matches)): Prevents race conditions and safely aggregates the final match count without utilizing slow atomic locks.

SIMD Vectorization (simd): Forces the compiler to utilize hardware vector registers to process multiple DNA characters in a single clock cycle.

Static Scheduling (schedule(static)): Pre-divides the uniform matrix workload into equal chunks at compile-time, eliminating dynamic assignment overhead.

Compiler Optimization (-O3): Aggressive hardware-level optimization applied during the build process.

Interpreting the Benchmark Results
When running the program on small-to-medium datasets (e.g., 1,700 bp Influenza sequences), the Serial execution may appear faster than the Parallel execution when compiled with -O3.

This is expected behavior in HPC known as Parallel Overhead. The -O3 flag makes the mathematical operations nearly instantaneous (sub-millisecond), meaning the program spends comparatively more time waking up and assigning OpenMP threads than it does calculating matches. As the dataset scales up to 50,000+ characters (yielding billions of comparisons), the computation time will drastically outpace the thread overhead, and the parallel architecture will demonstrate massive speedups.

Generating the Dot-Matrix Graph
If the application successfully generates an output.csv file, you can visualize the DNA alignment by running the included Python script from the root directory:

PowerShell
python plot.py
This will generate an influenza_dot_matrix.png image file showing the biological sequence alignments.