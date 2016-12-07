#ifndef SHADERC_PARSER_AST_H
#define SHADERC_PARSER_AST_H

#include <list>
#include <memory>
#include <string>

namespace shaderc {

class ASTVisitor;

class ASTNode {
public:
  virtual ~ASTNode() {}

  virtual void accept(ASTVisitor&) = 0;
};

// Base class for global declarations such as functions and types.
class GlobalDecl : public ASTNode {
public:
};

using GlobalDeclPtr = std::unique_ptr<GlobalDecl>;

class FunctionDecl : public GlobalDecl {
public:
  FunctionDecl(std::string name) :name_{name} { }
  void accept(ASTVisitor& visitor) override;

  const char* name() const { return name_.c_str(); }
private:
  std::string name_;
};

using FunctionDeclPtr = std::unique_ptr<FunctionDecl>;

// Root AST node for a whole translation unit.
class TranslationUnit : public ASTNode {
public:
  void accept(ASTVisitor& visitor) override;

  void append(GlobalDeclPtr decl);
private:
  std::list<GlobalDeclPtr> decls_;
};

using TranslationUnitPtr = std::unique_ptr<TranslationUnit>;

class ASTVisitor {
public:
  void visitAll(ASTNode&);
  
  virtual void visit(TranslationUnit&) = 0;
  virtual void visit(FunctionDecl&) = 0;
};

}

#endif // SHADERC_PARSER_AST_H
