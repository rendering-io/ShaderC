#ifndef SHADERC_SUPPORT_CONTEXT_H
#define SHADERC_SUPPORT_CONTEXT_H

#include <memory>

namespace llvm { class LLVMContext; }

namespace shaderc {

// Compiler context object which manages global state.
class Context {
public:
  Context();
  ~Context();
  
  llvm::LLVMContext& getLLVMContext();
private:
  class State;
  std::unique_ptr<State> impl_;
};

}

#endif // SHADERC_SUPPORT_CONTEXT_H
