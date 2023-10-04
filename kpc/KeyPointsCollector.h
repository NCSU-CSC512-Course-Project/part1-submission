// KeyPointsCollector.h
// ~~~~~~~~~~~~~~~~~~~~
// Defines the KeyPointsCollector interface.
#ifndef KEY_POINTS_COLLECTOR__H
#define KEY_POINTS_COLLECTOR__H

#include <clang-c/Index.h>

#include <string>
#include <vector>

class KeyPointsCollector {

  // Name of file we are analyzing
  const std::string filename;

  // Vector of CXCursor objs pointing to node of interest
  std::vector<CXCursor> cursorObjs;

  // Index - set of translation units that would be linked together as an exe
  // Ref ^ https://clang.llvm.org/docs/LibClang.html
  // Marked as static as there could be multiple KPC objects for files that need
  // to be grouped together.
  inline static CXIndex KPCIndex = clang_createIndex(0, 0);

  // Top level translation unit of the source file.
  CXTranslationUnit translationUnit;


public:
  
  // KPC ctor, takes file name in, ownership is transfered to KPC.
  // Inits the translation unit, invoking the clang parser.
  KeyPointsCollector(const std::string &fileName);

  // Nothing special needs to be done for dtor at the moment, leaving as
  // default.
  ~KeyPointsCollector() = default;

  // Returns a reference to collected cursor objects.
  std::vector<CXCursor> &getCursorObjs() { return cursorObjs; }

  // Core AST traversal function, once the translation unit has been parsed,
  // recursively visit nodes and add to cursorObjs if they are of interest.
  void collectCursors();

  // Iterates over cursorObjs and constructs the branch ptr trace.
  // Once traversal and parsing have finished.
  void outputBranchPtrTrace();

  // Invokes Valgrind through system calls and constructs output.
  void invokeValgrind();
};

#endif // KEY_POINTS_COLLECTOR__H
