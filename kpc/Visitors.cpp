// Analyzers.cpp
// ~~~~~~~~~~~~~~~~~~~~
// Implementations of the analysis methods.
#include "Common.h"
#include "KeyPointsCollector.h"
#include <cassert>
#include <iostream>

CXChildVisitResult KeyPointsCollector::ifStmtVisitor(CXCursor current,
                                                     CXCursor parent,
                                                     CXClientData kpc) {
  // Get KPC instance and the current branch
  KeyPointsCollector *kpcInstance = static_cast<KeyPointsCollector *>(kpc);
  KeyPointsCollector::BranchPointInfo *currentBranch =
      kpcInstance->getCurrentBranch();

  // Get initial branch point if start
  if (clang_getCursorKind(parent) == CXCursor_IfStmt &&
      currentBranch->branchPoint == 0) {
    clang_getSpellingLocation(clang_getCursorLocation(parent), nullptr,
                              currentBranch->getBranchPointOut(), nullptr,
                              nullptr);
    std::cout << "Found if statment branch point at line #"
              << currentBranch->branchPoint << std::endl;
  }

  // If a compound statement, get the first child and add that to a branch
  // target
  if (current.kind == CXCursor_CompoundStmt) {
    clang_visitChildren(current, &KeyPointsCollector::getFirstChildInCmpnd,
                        kpc);
  }

  // Check if alternate branch of interest
  if (kpcInstance->isBranchPointOrFunctionPtr(current.kind)) {

  }

  return CXChildVisit_Recurse;
}

CXChildVisitResult KeyPointsCollector::getFirstChildInCmpnd(CXCursor current,
                                                            CXCursor parent,
                                                            CXClientData kpc) {
  unsigned line;
  KeyPointsCollector *kpcInstance = static_cast<KeyPointsCollector *>(kpc);
  KeyPointsCollector::BranchPointInfo *currentBranch =
      kpcInstance->getCurrentBranch();

  assert(parent.kind == CXCursor_CompoundStmt);

  // Get line number and add as a target of the current branch;
  clang_getSpellingLocation(clang_getCursorLocation(current), nullptr, &line,
                            nullptr, nullptr);
  currentBranch->addTarget(line);
  std::cout << "Found branching target point at line #"
            << line << std::endl;
  // Just break after visiting the first child
  return CXChildVisit_Break;
}

CXChildVisitResult KeyPointsCollector::forStmtVisitor(CXCursor current,
                                                      CXCursor parent,
                                                      CXClientData kpc) {}

CXChildVisitResult KeyPointsCollector::doStmtVisitor(CXCursor current,
                                                     CXCursor parent,
                                                     CXClientData kpc) {}

CXChildVisitResult KeyPointsCollector::whileStmtVisitor(CXCursor current,
                                                        CXCursor parent,
                                                        CXClientData kpc) {}

CXChildVisitResult KeyPointsCollector::gotoStmtVisitor(CXCursor current,
                                                       CXCursor parent,
                                                       CXClientData kpc) {}

CXChildVisitResult KeyPointsCollector::switchVisitor(CXCursor current,
                                                     CXCursor parent,
                                                     CXClientData kpc) {}

CXChildVisitResult KeyPointsCollector::callExprVisitor(CXCursor current,
                                                       CXCursor parent,
                                                       CXClientData kpc) {
  /* std::cout << "AFound cursor of type: " */
  /*           <<
   * CXSTR(clang_getCursorKindSpelling(clang_getCursorKind(current))) */
  /*           << std::endl; */
  return CXChildVisit_Recurse;
}
