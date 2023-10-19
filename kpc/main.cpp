// main.cpp
// ~~~~~~~~
// Main execution for the KPC
#include "KeyPointsCollector.h"

#include <cassert>

int main(int argc, char *argv[]) {

  // Check a filename was passed
  assert(argc > 1 && "Must provide a file name as an argument!");

  // Get filename
  const std::string filename(argv[1]);

  // Init the KPC
  KeyPointsCollector kpc(filename);

  // Collect
  kpc.collectCursors();
}
