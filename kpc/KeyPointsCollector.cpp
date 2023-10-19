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
    rootCursor = clang_getTranslationUnitCursor(translationUnit);
  } else {

    std::cerr << "File with name: " << filename
              << ", does not exist! Exiting..." << std::endl;
    exit(1);
  }
}

bool KeyPointsCollector::isBranchPointOrFunctionPtr(CXCursorKind K) {
  // First check if cursor type is a possible branch point
  switch (K) {
  case CXCursor_IfStmt:
  case CXCursor_ForStmt:
  case CXCursor_DoStmt:
  case CXCursor_WhileStmt:
  case CXCursor_BreakStmt:
  case CXCursor_ContinueStmt:
  case CXCursor_GotoStmt:
  case CXCursor_SwitchStmt:
  case CXCursor_CallExpr:
    return true;
  default:
    return false;
  }
}

CXChildVisitResult KeyPointsCollector::VisitorFunction(CXCursor current,
                                                       CXCursor parent,
                                                       CXClientData kpc)
    KeyPointsCollector *instance = static_cast<KeyPointsCollector *>(kpc);

CXCursorKind kind = clang_getCursorKind(current);
if (instance->isBranchPointOrFunctionPtr(kind)) {
  std::cout << "Adding Cursor type: "
            << clang_getCString(clang_getCursorKindSpelling(kind)) << std::endl;
  ;
  instance->addCursor(current);
}

return CXChildVisit_Recurse;
}

void KeyPointsCollector::collectCursors() {
  clang_visitChildren(rootCursor, this->VisitorFunction, this);
}
