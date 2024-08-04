#ifndef AST_TBLGEN_COMPONENTS_HPP
#define AST_TBLGEN_COMPONENTS_HPP

#include "ast-tblgen/Printer.hpp"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include <memory>
#include <string>
#include <variant>

namespace astgen {
using namespace llvm;

class Type {
public:
  Type(StringRef name, ArrayRef<Type *> typeArgs, bool isPtr = false)
      : name(name.str()), typeArgs(typeArgs), isPtr(isPtr) {}

  StringRef getName() const { return name; }
  ArrayRef<Type *> getTypeArgs() const { return typeArgs; }
  bool getIsPtr() const { return isPtr; }

  void print(Printer &printer) const;

private:
  std::string name;
  SmallVector<Type *> typeArgs;
  bool isPtr;
};

class Var {
public:
  Var(StringRef name, Type *type) : name(name.str()), type(type) {}
  StringRef getName() const { return name; }
  Type *getType() const { return type; }
  void print(Printer &printer) const;

private:
  std::string name;
  Type *type;
};

class Method {
public:
  enum Kind {
    Normal,
    Static,
    Const,
  };

  Method(StringRef name, Type *retType, ArrayRef<Var *> params, Kind methodKind)
      : name(name.str()), retType(retType), params(params),
        methodKind(methodKind) {}
  StringRef getName() const { return name; }
  Type *getRetType() const { return retType; }
  ArrayRef<Var *> getParams() const { return params; }
  Kind getMethodKind() const { return methodKind; }
  void print(Printer &printer) const;

private:
  std::string name;
  Type *retType;
  SmallVector<Var *> params;
  Kind methodKind;
};

class Region {
public:
  Region(bool isPublic, ArrayRef<std::variant<Method *, Var *>> members)
      : isPublic(isPublic), members(members) {}
  bool getIsPublic() const { return isPublic; }
  ArrayRef<std::variant<Method *, Var *>> getMembers() const { return members; }
  void print(Printer &printer) const;

private:
  bool isPublic;
  SmallVector<std::variant<Method *, Var *>> members;
};

class Class {
public:
  Class(StringRef className, Type *inherit, ArrayRef<Region *> regions)
      : className(className.str()), inherit(inherit), regions(regions) {}

  StringRef getClassName() const { return className; }
  Type *getInheritClass() const { return inherit; }
  ArrayRef<Region *> getRegions() const { return regions; }
  void print(Printer &printer) const;

private:
  std::string className;
  Type *inherit;
  llvm::SmallVector<Region *> regions;
};

} // namespace astgen

#endif /// AST_TBLGEN_COMPONENTS_HPP
