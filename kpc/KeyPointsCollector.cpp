// KeyPointsCollector.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
// Implementation of KeyPointsCollector interface.
#include "KeyPointsCollector.h"
#include "Common.h"
#include <cassert>
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

    // Init cursor and branch count
    rootCursor = clang_getTranslationUnitCursor(translationUnit);
    branchCount = 0;
  } else {

    std::cerr << "File with name: " << filename
              << ", does not exist! Exiting..." << std::endl;
    exit(1);
  }
}

bool KeyPointsCollector::isBranchPointOrFunctionPtr(CXCursorKind K) {
  switch (K) {
  case CXCursor_IfStmt:
  case CXCursor_ForStmt:
  case CXCursor_DoStmt:
  case CXCursor_WhileStmt:
  case CXCursor_GotoStmt:
  case CXCursor_SwitchStmt:
  case CXCursor_CallExpr:
    return true;
  default:
    return false;
  }
}

CXChildVisitResult
KeyPointsCollector::VisitorFunctionInitial(CXCursor current, CXCursor parent,
                                           CXClientData kpc) {
  KeyPointsCollector *instance = static_cast<KeyPointsCollector *>(kpc);
  const CXCursorKind currKind = clang_getCursorKind(current);

  if (instance->isBranchPointOrFunctionPtr(currKind)) {
    unsigned line;
    clang_getSpellingLocation(clang_getCursorLocation(current), nullptr, &line,
                              nullptr, nullptr);

    std::cout << "Adding cursor of type: "
              << CXSTR(clang_getCursorKindSpelling(currKind))
              << " at line: " << line << std::endl;
    instance->addCursor(current);
    instance->analyzeChildren(current);
    return CXChildVisit_Continue;
  }

  return CXChildVisit_Recurse;
}

void KeyPointsCollector::collectCursors() {
  clang_visitChildren(rootCursor, this->VisitorFunctionInitial, this);
}

void KeyPointsCollector::analyzeChildren(CXCursor C) {
  CXCursorKind kind = clang_getCursorKind(C);
  if (!(kind == CXCursor_CallExpr)) {
    pushNewBranchPoint();
    switch (clang_getCursorKind(C)) {
    case CXCursor_IfStmt:
      clang_visitChildren(C, this->ifStmtVisitor, this);
      break;
    case CXCursor_ForStmt:
      clang_visitChildren(C, this->forStmtVisitor, this);
      break;
    case CXCursor_DoStmt:
      clang_visitChildren(C, this->doStmtVisitor, this);
      break;
    case CXCursor_WhileStmt:
      clang_visitChildren(C, this->whileStmtVisitor, this);
      break;
    case CXCursor_GotoStmt:
      clang_visitChildren(C, this->gotoStmtVisitor, this);
      break;
    case CXCursor_SwitchStmt:
      clang_visitChildren(C, this->switchVisitor, this);
      break;
    default:
      std::cerr << "Branch pt cursor encountered with no visitor funciton!";
    }
  } else {
    // handle function pointer / callexpr case ...
    assert(kind == CXCursor_CallExpr);
  }
}

void KeyPointsCollector::outputBranchPtrTrace() {}

void KeyPointsCollector::addBranchesToDictionary() {
  std::map<unsigned, std::string> targetsAndIds;
  const BranchPointInfo *currentBranch = getCurrentBranch();
  for (const unsigned &target : currentBranch->targets) {
    targetsAndIds[target] = "br_" + std::to_string(branchCount++);
  }
  branchDictionary[currentBranch->branchPoint] = targetsAndIds;
}
