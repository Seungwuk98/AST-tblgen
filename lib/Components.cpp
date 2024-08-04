#include "ast-tblgen/Components.hpp"

namespace astgen {

void Type::print(Printer &printer) const {
  printer.OS() << getName();
  if (!typeArgs.empty()) {
    printer.OS() << '<';
    for (auto I = getTypeArgs().begin(), E = getTypeArgs().end(); I != E; ++I) {
      if (I != getTypeArgs().begin())
        printer.OS() << ", ";
      (*I)->print(printer);
    }
    printer.OS() << '>';
  }
  if (isPtr)
    printer.OS() << " *";
}

void Var::print(Printer &printer) const {
  getType()->print(printer);
  printer.OS() << ' ' << getName();
}

void Method::print(Printer &printer) const {
  if (getMethodKind() == Method::Static)
    printer.OS() << "static ";

  retType->print(printer);
  printer.OS() << ' ' << getName() << '(';

  for (auto I = getParams().begin(), E = getParams().end(); I != E; ++I) {
    if (I != getParams().begin())
      printer.OS() << ", ";
    (*I)->print(printer);
  }

  printer.OS() << ")";

  if (getMethodKind() == Method::Const)
    printer.OS() << " const";
}

void Region::print(Printer &printer) const {
  printer.OS() << (getIsPublic() ? "public:" : "private:");
  Printer::AddIndentScope scope(printer, 2);
  for (const auto &var : getMembers()) {
    printer.LN();
    std::visit([&]<typename T>(const T *val) { val->print(printer); }, var);
    printer.OS() << ';';
  }
}

void Class::print(Printer &printer) const {
  printer.OS() << "class " << getClassName();
  if (getInheritClass()) {
    printer.OS() << " : public ";
    getInheritClass()->print(printer);
  }
  printer.OS() << " {";
  for (auto *region : getRegions()) {
    printer.LN();
    region->print(printer);
  }

  printer.LN() << "};";
  printer.LN();
}

} // namespace astgen
