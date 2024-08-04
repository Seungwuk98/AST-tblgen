#ifndef AST_TBLGEN_TABLE_GEN_EMITTER_HPP
#define AST_TBLGEN_TABLE_GEN_EMITTER_HPP

#include "ast-tblgen/Components.hpp"
#include "ast-tblgen/Context.hpp"
#include "llvm/TableGen/Record.h"
#include <llvm-17/llvm/ADT/StringRef.h>
#include <llvm-17/llvm/Support/raw_ostream.h>

namespace astgen {
using namespace llvm;

class TableGenEmitter {
public:
  TableGenEmitter(RecordKeeper &Records) : Records(Records) {}

  void Run(raw_ostream &OS);

  Class *CreateAST(Record *RC);

  Context *getCtx() { return &Ctx; }

  std::tuple<Type *, Type *, bool> getArgRetType(StringRef argType);

  struct DefineScope {
    DefineScope(raw_ostream &OS, StringRef symbol, bool isNDef = false)
        : OS(OS), symbol(symbol) {
      OS << (isNDef ? "#ifndef " : "#ifdef ") << symbol << '\n';
    }
    ~DefineScope() { OS << "#endif // " << symbol << '\n'; }

  private:
    raw_ostream &OS;
    StringRef symbol;
  };

  struct NamespaceScope {
    NamespaceScope(raw_ostream &OS, StringRef symbol) : OS(OS), symbol(symbol) {
      OS << "namespace " << symbol << " {\n";
    }
    ~NamespaceScope() { OS << "}\n"; }

  private:
    raw_ostream &OS;
    StringRef symbol;
  };

private:
  std::pair<SmallVector<std::string>, SmallVector<std::string>>
  resolveTreeData(DagInit *dag);

  RecordKeeper &Records;
  Context Ctx;
};

} // namespace astgen

#endif // AST_TBLGEN_TABLE_GEN_EMITTER_HPP
