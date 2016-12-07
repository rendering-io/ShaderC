#ifndef SHADERC_CODEGEN_CODEGENERATOR_H
#define SHADERC_CODEGEN_CODEGENERATOR_H

#include <memory>

namespace shaderc {

class CodeGeneratorImpl;
class Context;
class IRModule;
class TranslationUnit;

class CodeGenerator {
public:
  CodeGenerator(Context &ctx);
  ~CodeGenerator();

  std::unique_ptr<IRModule> buildIRModule(TranslationUnit &);
private:
  std::unique_ptr<CodeGeneratorImpl> impl_;
};

}

#endif // SHADERC_CODEGEN_CODEGENERATOR_H
