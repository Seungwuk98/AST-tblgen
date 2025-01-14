#include "ast-tblgen/TableGenEmitter.hpp"
#include "llvm/Support/CommandLine.h"
#include "llvm/TableGen/Main.h"
#include "llvm/TableGen/TableGenBackend.h"
#include <iostream>

using namespace llvm;

namespace astgen {
bool ASTGenMain(raw_ostream &os, RecordKeeper &Records) {
  TableGenEmitter emitter(Records);
  emitter.Run(os);
  return false;
}

} // namespace astgen

int main(int argc, const char **argv) {
  llvm::cl::ParseCommandLineOptions(argc, argv);
  return llvm::TableGenMain(argv[0], astgen::ASTGenMain);
}
