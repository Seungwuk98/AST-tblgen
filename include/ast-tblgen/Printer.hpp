#ifndef AST_TBLGEN_PRINTER_HPP
#define AST_TBLGEN_PRINTER_HPP

#include "llvm/Support/raw_ostream.h"

namespace astgen {
using namespace llvm;

class Printer {
public:
  Printer(raw_ostream &os) : indent(0), os(os) {}

  raw_ostream &OS() { return os; }
  raw_ostream &LN() { return os << '\n' << std::string(indent, ' '); }

  struct AddIndentScope {
    AddIndentScope(Printer &printer, int amount)
        : printer(printer), savedIndent(printer.indent) {
      printer.indent += amount;
    };
    ~AddIndentScope() { printer.indent = savedIndent; }

  private:
    Printer &printer;
    int savedIndent;
  };

private:
  int indent;
  raw_ostream &os;
};

} // namespace astgen

#endif // AST_TBLGEN_PRINTER_HPP
