// Common.h
// ~~~~~~~~
// Common macros and other globals.
#include <clang-c/Index.h>

#define CXSTR(X) clang_getCString(X)

// File system output
#define EXE_OUT "program.out"
#define OUT_DIR "out/"

// Transforms
#define TRANSFORM_HEADER                                                       \
  "#include <stdio.h>\n#define LOG(BP) printf(\"%s\\n\", BP);\n"
