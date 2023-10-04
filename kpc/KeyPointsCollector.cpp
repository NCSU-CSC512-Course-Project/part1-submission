// KeyPointsCollector.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
// Implementation of KeyPointsCollector interface.
#include "KeyPointsCollector.h"
#include <fstream>
#include <iostream>

// Ctor Implementation
KeyPointsCollector::KeyPointsCollector(const std::string &filename)
    : filename(std::move(filename)) {

  // Check if file exists
  std::ifstream file(filename);
  if (file.good()) {
    file.close();

    // If good, try to parse the translation unit.
    translationUnit =
        clang_parseTranslationUnit(KPCIndex, filename.c_str(), nullptr, 0,
                                   nullptr, 0, CXTranslationUnit_None);
    // Check if parsed properly
    if (translationUnit == nullptr) {
      std::cerr << "There was an error parsing the translation unit! Exiting..."
                << std::endl;
      exit(1);
    }
    std::cout << "Translation unit for file: " << filename
              << " successfully parsed." << std::endl;
  } else {

    std::cerr << "File with name: " << filename
              << ", does not exist! Exiting..." << std::endl;
    exit(1);
  }
}
