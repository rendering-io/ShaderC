#include <shaderc/AST/ASTPrettyPrinter.h>

using namespace shaderc;

ASTPrettyPrinter::ASTPrettyPrinter(std::ostream& out) : out_{out} {

}

void ASTPrettyPrinter::print(TranslationUnit& module) {
  visitAll(module);
}

void ASTPrettyPrinter::visit(TranslationUnit& module) {
  out_ << "TU\n";
}

void ASTPrettyPrinter::visit(FunctionDecl& fdecl) {
  out_ << " fn\n";
}
