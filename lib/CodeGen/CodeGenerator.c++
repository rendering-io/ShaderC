#include <shaderc/AST/AST.h>
#include <shaderc/CodeGen/CodeGenerator.h>
#include <shaderc/CodeGen/IRModule.h>
#include <shaderc/Support/Context.h>
#include <llvm/IR/Module.h>

namespace shaderc {

class LLVMIRModule : public IRModule {
public:
  llvm::Module module_;
};

class CodeGeneratorImpl : public ASTVisitor {
public:
  CodeGeneratorImpl(Context &ctx);
  void visit(TranslationUnit&) override;
  void visit(FunctionDecl&) override;
private:
  Context &ctx_;
  std::unique_ptr<llvm::Module> module_;
};

}

using namespace shaderc;

CodeGenerator::CodeGenerator(Context &ctx)
: impl_{std::make_unique<CodeGeneratorImpl>(ctx)} {
}

CodeGenerator::~CodeGenerator() {

}

std::unique_ptr<IRModule> CodeGenerator::buildIRModule(TranslationUnit &module) {
  impl_->visit(module);
  return std::make_unique<IRModule>();
}

CodeGeneratorImpl::CodeGeneratorImpl(Context &ctx) : ctx_{ctx} { }

void CodeGeneratorImpl::visit(TranslationUnit& module) {
  module_ = std::make_unique<llvm::Module>("unamed"/*module.name()*/,
                                           ctx_.getLLVMContext());
}

void CodeGeneratorImpl::visit(FunctionDecl& decl) {
  using namespace llvm;

  auto &ctx = ctx_.getLLVMContext();

  // Get the LLVM type representing void.
  auto *voidType = Type::getVoidTy(ctx);

  // Construct a type for a void function with no arguments.
  auto *funcType = FunctionType::get(voidType, false);

  // Now add a function to our module.
  auto *func = Function::Create(funcType, GlobalValue::ExternalLinkage,
                                "name", module_.get());

}
