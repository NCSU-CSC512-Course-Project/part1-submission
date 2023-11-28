// main.cpp
// ~~~~~~~~
// Main execution for the KPC
#include "KeyPointsCollector.h"

#include <cassert>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {

  // Get filename
  std::string filename;
  std::cout << "Enter a file name for analysis: ";
  std::cin >> filename;

  if (!static_cast<bool>(std::ifstream(filename).good())) {
    std::cerr << "There was an issue opening " << filename
              << " for analysis, exiting!\n";
    exit(EXIT_FAILURE);
  }

  // Init the KPC
  KeyPointsCollector kpc(
      filename, (argc > 2 && !std::string(argv[2]).compare("--debug")));
  kpc.executeToolchain();
}
