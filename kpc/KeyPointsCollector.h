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

  // Root cursor of the translation unit.
  CXCursor rootCursor;

  // This is a weird one, since clang_visitChildren requires a function ptr
  // for its second argument without any signature, its not possible to capture
  // 'this' with a lambda. Consequently, we mark this function as static and
  // pass 'this' for the clientdata parameter in the clang_visitChildren call to
  // retrieve the KPC instance.
  static CXChildVisitResult VisitorFunction(CXCursor current, CXCursor parent,
                                            CXClientData clientData);

  // We can generalize the output of each node with three different
  // CXSourceLocations:
  //    1. The branch point itself. e.g  line 5: if (condition) ...
  //    2. The target or 'then' of the branch point. e.g line 6: { if stmt
  //    body... }
  //    3. The end or 'else' of the branch point. e.g line 7: else {...} ...
  struct BranchPointInfo {
    CXSourceLocation branchPoint;
    CXSourceLocation branchTarget;
    CXSourceLocation branchEnd;
  };

  // Define a separate information struct for function pointers.
  struct FuncPtrInfo {
    std::string name;
    CXSourceLocation funcLoc;
  };

  // Analysis Methods -
  // Once the cursors have been collected from the translation unit, some
  // analysis needs to happen on the cursors before the branch ptr trace can be
  // output. Depending on the type of cursor, different steps need to be taken
  // to gather information about that cursors children as each cursor or
  // ASTNode, has a differing children heirarchy.

  // The information associated with different nodes can be found in
  // the clang source code:
  // https://github.com/llvm/llvm-project/tree/main/clang/include/clang/AST

  // Noted before each function is the children that need to be analyzed
  // for the BP trace.

  // Children: then, else
  BranchPointInfo analyzeIfCursor(CXCursor C);

  // Children: body
  BranchPointInfo analyzeForCursor(CXCursor C);

  // Children: body
  BranchPointInfo analyzeDoCursor(CXCursor C);

  // Children: body
  BranchPointInfo analyzeWhileCursor(CXCursor C);

  // Children: TBD
  BranchPointInfo analyzeBreakCursor(CXCursor C);

  // Children: TBD
  BranchPointInfo analyzeContinueCursor(CXCursor C);

  // Children: TBD
  BranchPointInfo analyzeGotoCursor(CXCursor C);

  // Children: TBD, a bit more complex than the others
  BranchPointInfo analyzeSwitchCursor(CXCursor C);

public:
  // KPC ctor, takes file name in, ownership is transfered to KPC.
  // Inits the translation unit, invoking the clang parser.
  KeyPointsCollector(const std::string &fileName);

  // Nothing special needs to be done for dtor at the moment, leaving as
  // default.
  ~KeyPointsCollector() = default;

  // Returns a reference to collected cursor objects.
  const std::vector<CXCursor> &getCursorObjs() const { return cursorObjs; }

  // Core AST traversal function, once the translation unit has been parsed,
  // recursively visit nodes and add to cursorObjs if they are of interest.
  void collectCursors();

  // Checks to see if the current cursor is a point in the program
  // that could be a branch
  bool isBranchPointOrFunctionPtr(CXCursorKind K);

  // Add a cursor to cursorObjs
  void addCursor(CXCursor const &C) { cursorObjs.push_back(C); }

  // Iterates over cursorObjs and constructs the branch ptr trace.
  // Once traversal and parsing have finished.
  void outputBranchPtrTrace();

  // Invokes Valgrind through system calls and constructs output.
  void invokeValgrind();
};

#endif // KEY_POINTS_COLLECTOR__H
