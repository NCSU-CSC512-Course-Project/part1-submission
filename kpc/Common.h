// Common.h
// ~~~~~~~~
// Common macros and other globals.
#include <clang-c/Index.h>

#define CXSTR(X) clang_getCString(X)
#define QKDBG(OUT) std::cout << OUT << '\n';

// File system output
#define OUT_DIR "out/"
#define EXE_OUT std::string(OUT_DIR + filename + ".out")
#define MODIFIED_PROGAM_OUT std::string(OUT_DIR + filename + ".modified.c")

#define MAP_FIND(MAP, KEY) MAP.find(KEY) != MAP.end()

// Transforms
#define TRANSFORM_HEADER                                                       \
  "#include <stdio.h>\n#define LOG(BP) printf(\"%s\\n\", BP);\n"

#define DECLARE_BRANCH(BRANCH) "int BRANCH_" << BRANCH << " = 0;\n"
#define SET_BRANCH(BRANCH) "BRANCH_" << BRANCH << " = 1;\n"
#define WRITE_LINE(LINE) LINE << '\n';
