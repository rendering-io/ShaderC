#ifndef SHADERC_PARSER_ASTPRETTYPRINTER_H
#define SHADERC_PARSER_ASTPRETTYPRINTER_H

#include "AST.h"
#include <ostream>

namespace shaderc {

class ASTPrettyPrinter : public ASTVisitor {
public:
  ASTPrettyPrinter(std::ostream&);

  void visit(TranslationUnit&) override;
  void visit(FunctionDecl&) override;

  void print(TranslationUnit&);

private:
  std::ostream& out_;
};

}

#endif // SHADERC_PARSER_ASTPRETTYPRINTER_H
