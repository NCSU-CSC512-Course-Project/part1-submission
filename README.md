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
Here is a quick example for this C program:<br>
```C
int add(int a, int b) { return a + b; }

int main(void) {
  int (*add_ptr)(int, int) = &add;

  int result = (*add_ptr)(2, 2);
  result = 4;

  if (result == 4) {
    for (int acc = 1; result <= 100;) {
      result += add(result, result);
    }
    result += 1;
  }
  return result;
}
```
This KeyPointsCollector will produce this dictionary<br>
br_1: test_file.c, 9, 10<br>
br_2: test_file.c, 9, 13<br>
br_3: test_file.c, 9, 15<br>
br_4: test_file.c, 10, 11<br>
br_5: test_file.c, 10, 13<br>

and the transformed program looks like: <br>
```C
#include <stdio.h>
#define LOG(BP) printf("%s\n", BP);
#define LOG_PTR(PTR) printf("func_%p\n", PTR);
int add(int a, int b) { return a + b; }

int *add_PTR = &add;

int main(void) {
  int BRANCH_0 = 0;
  int BRANCH_1 = 0;

  int (*add_ptr)(int, int) = &add;

  LOG_PTR(add_PTR);
  int result = (*add_ptr)(2, 2);
  result = 4;

  if (result == 4) {
    BRANCH_0 = 1;
    if (!BRANCH_1)
      LOG("br_1");
    for (int acc = 1; result <= 100;) {
      BRANCH_1 = 1;
      LOG("br_4");
      LOG_PTR(add_PTR);
      result += add(result, result);
    }
    if (BRANCH_1) {
      LOG("br_5")
    } else {
      LOG("br_2")
    }
    result += 1;
  }
  if (!BRANCH_1)
    LOG("br_3");
  return result;
}
```
producing this trace:<br>
```bash
func_0x55c84b2d0140
br_1
br_4
func_0x55c84b2d0140
br_4
func_0x55c84b2d0140
br_4
func_0x55c84b2d0140
br_5
```


## Build and Usage
