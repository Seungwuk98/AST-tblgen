#include "ast-tblgen/TableGenEmitter.hpp"
#include "ast-tblgen/Printer.hpp"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/TableGen/Record.h"
#include <llvm-17/llvm/ADT/StringRef.h>

namespace astgen {

static std::string makeFirstUppercase(StringRef str) {
  return (std::string(1, str[0] - 32) + str.substr(1)).str();
}

void TableGenEmitter::Run(raw_ostream &OS) {
  Printer printer(OS);

  std::vector<Record *> ASTs = Records.getAllDerivedDefinitions("AST");

  SmallVector<Class *> asts;

  DefineScope scope(OS, "DECLARE_AST");

  for (auto *RC : ASTs) {
    auto *newAST = CreateAST(RC);
    if (!newAST)
      return;
    asts.emplace_back(newAST);
  }

  {
    NamespaceScope nameScope(OS, "ast");
    for (auto *C : asts)
      C->print(printer);
  }
}

Class *TableGenEmitter::CreateAST(Record *RC) {
  auto className = RC->getName();

  auto *treeData = RC->getValueAsDag("TreeData");
  assert(treeData && treeData->getOperator()->getAsString() == "ins");

  const auto &[argName, tagName] = resolveTreeData(treeData);

  SmallVector<Type *> memberTypes;
  SmallVector<Type *> methodRetTypes;
  memberTypes.reserve(argName.size());
  memberTypes.reserve(argName.size());

  for (const auto &tag : tagName) {
    const auto &[memberT, retT, success] = getArgRetType(tag);
    if (!success) {
      llvm::errs() << "Fails" << '\n';
      return nullptr;
    }
    memberTypes.emplace_back(memberT);
    methodRetTypes.emplace_back(retT);
  }

  auto *dataType = Ctx.alloc<Type>("std::tuple", memberTypes);
  auto *treeDataVar = Ctx.alloc<Var>("Data", dataType);

  SmallVector<Method *> getters;
  getters.reserve(argName.size());

  for (const auto &[arg, retT] : llvm::zip(argName, methodRetTypes)) {
    auto getterName = "get" + makeFirstUppercase(arg);
    auto *method =
        Ctx.alloc<Method>(getterName, retT, std::nullopt, Method::Const);
    getters.emplace_back(method);
  }

  SmallVector<std::variant<Method *, Var *>> regionData;
  regionData.reserve(argName.size() + 1);

  regionData.append(getters.begin(), getters.end());
  regionData.emplace_back(treeDataVar);

  auto *region = Ctx.alloc<Region>(true, regionData);

  return Ctx.alloc<Class>(className, nullptr, region);
}

std::tuple<Type *, Type *, bool>
TableGenEmitter::getArgRetType(StringRef argType) {
  if (argType == "string") {
    auto *paramType = Ctx.alloc<Type>("std::string", std::nullopt);
    auto *retType = Ctx.alloc<Type>("llvm::StringRef", std::nullopt);
    return {paramType, retType, true};
  } else {
    return {nullptr, nullptr, false};
  }
}

std::pair<SmallVector<std::string>, SmallVector<std::string>>
TableGenEmitter::resolveTreeData(DagInit *dag) {

  SmallVector<std::string> argName;
  SmallVector<std::string> tagName;

  for (auto idx = 0; idx < dag->arg_size(); ++idx) {
    auto arg = dag->getArg(idx)->getAsUnquotedString();
    auto tag = dag->getArgName(idx)->getAsUnquotedString();
    argName.emplace_back(arg);
    tagName.emplace_back(tag);
  }

  return {argName, tagName};
}

} // namespace astgen
