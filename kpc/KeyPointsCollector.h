// KeyPointsCollector.h
// ~~~~~~~~~~~~~~~~~~~~
// Defines the KeyPointsCollector interface.
#ifndef KEY_POINTS_COLLECTOR__H
#define KEY_POINTS_COLLECTOR__H

#include <clang-c/Index.h>

#include <map>
#include <stack>
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
  static CXChildVisitResult VisitorFunctionInitial(CXCursor current,
                                                   CXCursor parent,
                                                   CXClientData clientData);

  struct BranchPointInfo {
    unsigned branchPoint;
    std::vector<unsigned> targets;
    unsigned *getBranchPointOut() { return &branchPoint; }
    void addTarget(unsigned target) { targets.push_back(target); }
  };

  // Define a separate information struct for function pointers.
  struct FuncPtrInfo {
    std::string name;
    unsigned funcLoc;
  };

  // Analysis Methods -
  // Once the cursors have been collected from the translation unit, some
  // analysis needs to happen on the cursors before the branch ptr trace can be
  // output. Depending on the type of cursor, a corressponding visitor function
  // will be called to handle the analysis.

  // The information associated with different nodes can be found in
  // the clang source code:
  // https://github.com/llvm/llvm-project/tree/main/clang/include/clang/AST

  // Noted before each function is the children that need to be analyzed
  // for the BP trace.

  // Children: then, else
  static CXChildVisitResult ifStmtVisitor(CXCursor current, CXCursor parent,
                                          CXClientData kpc);

  // Children: body
  static CXChildVisitResult forStmtVisitor(CXCursor current, CXCursor parent,
                                           CXClientData kpc);

  // Children: body
  static CXChildVisitResult doStmtVisitor(CXCursor current, CXCursor parent,
                                          CXClientData kpc);

  // Children: body
  static CXChildVisitResult whileStmtVisitor(CXCursor current, CXCursor parent,
                                             CXClientData kpc);

  // Children: TBD
  static CXChildVisitResult gotoStmtVisitor(CXCursor current, CXCursor parent,
                                            CXClientData kpc);

  // Children: TBD, a bit more complex than the others
  static CXChildVisitResult switchVisitor(CXCursor current, CXCursor parent,
                                          CXClientData kpc);

  // This will be passed a call expression for now.
  static CXChildVisitResult callExprVisitor(CXCursor current, CXCursor parent,
                                            CXClientData kpc);

  static CXChildVisitResult getFirstChildInCmpnd(CXCursor current,
                                                 CXCursor parent, CXClientData);

  // Core analysis function, swtiches on cursor types and invokes
  // the correct visitor function.
  void analyzeChildren(CXCursor C);

  // Push a new BP onto the stack
  void pushNewBranchPoint() {
    branchPointStack.push({.branchPoint = 0});
  }

  // Amount of branches, initialized to 0 in ctor
  unsigned branchCount;

  // Stack of branch points being analyzed.
  std::stack<BranchPointInfo> branchPointStack;

  // After a branch analysis is done, add the branch point,
  // and a new branch for each target that was found.
  void addBranches();

  // Core branch dictionary
  // The outer map holds the initial branch point line number,
  // whilst the inner map holds the target line number for the branch,
  // mapped to the branch id e.g. 'br_2'
  std::map<unsigned, std::map<unsigned, std::string>> branchDictionary;

  // Called once branch analysis has completed.
  void addBranchesToDictionary();

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

  // Get pointer to current branch point info struct
  BranchPointInfo *getCurrentBranch() { return &(branchPointStack.top()); }

  // Iterates over cursorObjs and constructs the branch ptr trace.
  // Once traversal and parsing have finished.
  void outputBranchPtrTrace();

  // Invokes Valgrind through system calls and constructs output.
  void invokeValgrind();
};

#endif // KEY_POINTS_COLLECTOR__H
