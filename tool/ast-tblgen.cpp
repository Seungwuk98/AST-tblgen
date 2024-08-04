#include "llvm/Support/CommandLine.h"
#include "llvm/TableGen/Main.h"
#include "llvm/TableGen/TableGenBackend.h"
#include <iostream>

using namespace llvm;

bool ASTGenMain(raw_ostream &os, RecordKeeper &Records) {
  llvm::errs() << 1 << '\n';
  return false;
}

int main(int argc, const char **argv) {
  llvm::cl::ParseCommandLineOptions(argc, argv);
  return llvm::TableGenMain(argv[0], ASTGenMain);
}
