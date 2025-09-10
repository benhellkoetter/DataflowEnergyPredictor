# Overview
The DataflowEnergyPredictor is a static program analysis tool designed to predict the cumulative energy consumption of variables in C/C++ programs. Instead of executing the program, it analyzes the code at the LLVM Intermediate Representation (IR) level to estimate the energy cost required to compute each value.

The goal is to provide developers with insights into potentially energy-intensive computation paths early in the software development process, thereby enabling optimizations for energy efficiency.

The core of the tool is a dataflow-based approach implemented using the Phasar framework. It tracks the flow of energy values (EnergyFact) through the program's control-flow graph and calculates their accumulated energy costs based on a configurable energy model.

## Core Concepts
- Static Analysis: The program is analyzed without being executed. This allows for the detection of energy-intensive computation paths regardless of specific program inputs.

- LLVM Intermediate Representation (IR): The analysis is performed not on the C/C++ source code directly, but on the platform-independent LLVM IR. This allows the tool to work with any language that can be compiled to LLVM IR.

- Dataflow Analysis (IDE Framework): The tool uses the Interprocedural Distributive Environment (IDE) framework from Phasar. This not only allows tracking of dataflow facts (which variable influences which other?) but also computes an associated value (in this case, energy in Joules).

- Configurable Energy Model: The energy costs of individual LLVM instructions are not hard-coded but are loaded from an external JSON file. This allows the analysis to be adapted to different CPU architectures by creating a corresponding energy profile.

# How It Works
The analysis proceeds in several steps:

1. Compilation to LLVM-IR: C/C++ source code is compiled into an LLVM-IR (.ll) file using Clang. For the most accurate analysis, the optimization level -O0 is recommended to preserve the structure of the original code.

2. Loading the Energy Profile: On startup, the ProfileLoader loads a JSON profile containing the base energy costs for different instruction categories (e.g., MEMORY, DIVISION, PROGRAMFLOW, OTHER).

3. Setting up the Analysis: The main.cpp file initializes the Phasar environment, creates an Interprocedural Control-Flow Graph (ICFG), and instantiates the central analysis class, EnergyOfValueAnalysis.

4. Running the IDE Analysis: The Phasar solver traverses the program and applies the defined logic:

- Flow Functions (getNormalFlowFunction): Define how EnergyFacts (the variables to be tracked) are propagated from one instruction to the next or newly generated. For example, with an add instruction, a new EnergyFact is created for the result, which depends on the operands.

- Edge Functions (getNormalEdgeFunction): Calculate the transformation of the energy value on each edge of the data flow. If the value of a variable a is used to compute b, the energy of a is transferred to b, and the cost of the executing instruction is added.

5. Handling Control Flow:

- Branches (if/else): When data flows merge, the worst-case (the maximum energy value) is used for further calculation to ensure an over-approximation.

- Loops (for/while): Since the number of iterations is not always known statically, a fixed number can be set via the --iterations command-line parameter to prevent an infinite loop in the solver.

6. Outputting the Results: After the analysis is complete, the tool outputs the known EnergyFacts and their calculated cumulative energy values for each instruction in the program.

# Building the Project
The project uses CMake and is configured to be built with clang++-15. A helper script build.sh is provided to simplify the process.

## 1. Build the executable
Run the script from the root directory of the project. It will create a build directory, run CMake, and compile the source code.

./build.sh

The executable DataflowEnergyPredictor will be located in the build/ directory.

## 2. Clean the build directory
To remove the build directory and all compiled files, run the script with the clean argument.

./build.sh clean

# Usage
## 1. Compile C/C++ Source Code to LLVM-IR
Use clang++ to compile your C++ code into a .ll file. It's important to disable optimization with -O0 to avoid altering the analysis with compiler optimizations.

clang++ -S -emit-llvm -O0 tests/example.cpp -o tests/example.ll

## 2. Run the Energy Analysis
Execute the DataflowEnergyPredictor from the build directory.

./DataflowEnergyPredictor <LLVM-IR-File> -E <function-name> --profile <JSON-profile-file>

Examples:

./DataflowEnergyPredictor ../tests/test_simple_add.ll -E main --profile ../profile_i5-1335u.json

./DataflowEnergyPredictor ../tests/test_ifelse_add.ll -E main --profile ../profile_i5-1335u.json

## Command-Line Options
<LLVM-IR-File>: (Required) The path to the .ll file to be analyzed.

-E <function name>: (Required) The entry point of the analysis (e.g. main).

--profile=<profile.json>: (Optional) Path to the JSON file with the energy model. If not provided, internal default values will be used.

--iterations=<N>: (Optional) Sets a fixed number of iterations for all loops. The default value is 1.

## The Energy Profile Format
The JSON file for the energy profile should have the following format, which maps the energy costs (in Joules) per instruction category. Such a profile can be determined experimentally for a specific CPU architecture using the tool Spear.

{
  "cpu": {
    "name": "13th Gen Intel(R) Core(TM) i5-1335U",
    "architecture": "6",
    "cores": "12"
  },
  "profile": {
    "Division": 0.0050198,
    "Memory": 0.0050409,
    "Other": 0.0050799,
    "Programflow": 0.0049983
  }
}
