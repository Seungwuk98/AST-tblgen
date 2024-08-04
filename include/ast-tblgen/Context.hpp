#ifndef AST_TBLGEN_CONTEXT_HPP
#define AST_TBLGEN_CONTEXT_HPP

#include "llvm/Support/Allocator.h"

namespace astgen {
using namespace llvm;

class Context {
public:
  ~Context() {
    for (const auto &[ptr, destFn] : destStorage)
      destFn(ptr);
  }

  template <typename T, typename... Args> T *alloc(Args &&...args) {
    auto *ptr = allocator.Allocate<T>(1);
    new (ptr) T(std::forward<Args>(args)...);
    destStorage.emplace_back(
        ptr, [](void *toDestruct) { static_cast<T *>(toDestruct)->~T(); });
    return ptr;
  }

private:
  BumpPtrAllocator allocator;
  SmallVector<std::pair<void *, std::function<void(void *)>>> destStorage;
};

} // namespace astgen

#endif // AST_TBLGEN_CONTEXT_HPP
