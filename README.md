# CSC512 Course Project Part 1 - Key Points Detection
## Overview
This part of the course project aims to analyze a C program and determine its key points, branch points and function pointers. To achieve this, we have leveraged [LibClang](https://clang.llvm.org/docs/LibClang.html), a sub-tool from the Clang C-Family frontend under the larger [LLVM Compiler Infrastructure Project](llvm.org). Heres a brief overview of how this program works:<br>
1. Takes in a C file as input and parses it.
2. Traverses the AST and collects information on the branching points and function pointers.
3. Creates a branch point dictionary file.
4. Transforms the program, inserting log statments at branch points and function calls/pointers.
5. Optionally invokes Valgrind to retrieve the total amount of machine instructions executed for the program.
6. Optionally executes the transformed program to output a branch-pointer trace for the program.
## Example

## Build and Usage
