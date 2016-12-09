#include <shaderc/AST/AST.h>
#include <shaderc/CodeGen/CodeGenerator.h>
#include <shaderc/CodeGen/IRModule.h>
#include <shaderc/Support/Context.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>

namespace shaderc {

class LLVMIRModule : public IRModule {
public:
  LLVMIRModule(Context&, const char*);

  operator llvm::Module*();

  void print(std::ostream&) override;

  llvm::Module module_;
};

class CodeGeneratorImpl : public ASTVisitor {
public:
  CodeGeneratorImpl(Context &ctx);
  void visit(TranslationUnit&) override;
  void visit(FunctionDecl&) override;
private:
  Context &ctx_;
  std::unique_ptr<LLVMIRModule> module_;

  friend class CodeGenerator;
};

}

using namespace shaderc;

IRModule::~IRModule(){}

LLVMIRModule::LLVMIRModule(Context& ctx, const char* name)
: module_{name, ctx.getLLVMContext()} {

}

void LLVMIRModule::print(std::ostream& out) {
  std::string storage;
  llvm::raw_string_ostream os{storage};
  module_.print(os, nullptr);
  out << os.str();
}

LLVMIRModule::operator llvm::Module*() {
  return &module_;
}

CodeGenerator::CodeGenerator(Context &ctx)
: impl_{std::make_unique<CodeGeneratorImpl>(ctx)} {
}

CodeGenerator::~CodeGenerator() {

}

std::unique_ptr<IRModule> CodeGenerator::buildIRModule(TranslationUnit &module) {
  impl_->visitAll(module);
  return std::move(impl_->module_);
}

CodeGeneratorImpl::CodeGeneratorImpl(Context &ctx) : ctx_{ctx} { }

void CodeGeneratorImpl::visit(TranslationUnit& module) {
  module_ = std::make_unique<LLVMIRModule>(ctx_, "unamed"/*module.name()*/);
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
                                decl.name(), *module_);

  // Set the calling convention based on the shader type.
  func->setCallingConv(1/*CallingConv::MaxID - 1*/);

  // Add a first basic block.
  BasicBlock* block = BasicBlock::Create(ctx, "entry", func);
  IRBuilder<> builder(block);
}
