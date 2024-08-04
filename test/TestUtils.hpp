#ifndef AST_TBLGEN_TEST_UTIL_HPP
#define AST_TBLGEN_TEST_UTIL_HPP

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/FormatVariadic.h"
#include <format>

namespace astgen::test {

inline bool TrimEq(llvm::StringRef L, llvm::StringRef R) {
  return L.trim() == R.trim();
}

} // namespace astgen::test

#define TRIM_EQ(l, r)                                                          \
  do {                                                                         \
    if (!astgen::test::TrimEq(l, r))                                           \
      FAIL(llvm::formatv("Left:\n{0}\n\nRight:\n{}", l, r).str());             \
  } while (false)

#endif //  AST_TBLGEN_TEST_UTIL_HPP
