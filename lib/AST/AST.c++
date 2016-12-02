#include <shaderc/AST/AST.h>

using namespace shaderc;

void FunctionDecl::accept(ASTVisitor& visitor) {
  visitor.visit(*this); 
}

void TranslationUnit::accept(ASTVisitor& visitor) {
  // Traverse the translation unit itself first.
  visitor.visit(*this);

  // Then traverse all of its global declarations.
  for (auto& decl: decls_)
    decl->accept(visitor);
}

void TranslationUnit::append(GlobalDeclPtr decl) {
  decls_.emplace_back(std::move(decl));
}

