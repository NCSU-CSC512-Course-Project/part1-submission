// main.cpp
// ~~~~~~~~
// Main execution for the KPC
#include "KeyPointsCollector.h"

#include <cassert>
#include <iostream>

int main(int argc, char *argv[]) {
  ;

  // Check a filename was passed
  assert(argc > 1 && "Must provide a file name as an argument!");

  // Get filename
  const std::string filename(argv[1]);

  // Init the KPC
  KeyPointsCollector kpc(
      filename, (argc > 2 && !std::string(argv[2]).compare("--debug")));

  kpc.invokeValgrind();
}
