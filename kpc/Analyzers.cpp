// Analyzers.cpp
// ~~~~~~~~~~~~~~~~~~~~
// Implementations of the analysis methods.
#include "KeyPointsCollector.h"

KeyPointsCollector::BranchPointInfo
KeyPointsCollector::analyzeIfCursor(CXCursor C) {}

KeyPointsCollector::BranchPointInfo
KeyPointsCollector::analyzeForCursor(CXCursor C) {}

KeyPointsCollector::BranchPointInfo
KeyPointsCollector::analyzeDoCursor(CXCursor C) {}

KeyPointsCollector::BranchPointInfo
KeyPointsCollector::analyzeWhileCursor(CXCursor C) {}

KeyPointsCollector::BranchPointInfo
KeyPointsCollector::analyzeBreakCursor(CXCursor C) {}

KeyPointsCollector::BranchPointInfo
KeyPointsCollector::analyzeContinueCursor(CXCursor C) {}

KeyPointsCollector::BranchPointInfo
KeyPointsCollector::analyzeGotoCursor(CXCursor C) {}

KeyPointsCollector::BranchPointInfo
KeyPointsCollector::analyzeSwitchCursor(CXCursor C) {}

KeyPointsCollector::FuncPtrInfo
KeyPointsCollector::analyzeFuncPtrCursor(CXCursor C) {}
