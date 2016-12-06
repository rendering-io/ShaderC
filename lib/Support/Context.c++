#include <shaderc/Support/Context.h>
#include <llvm/Support/ManagedStatic.h>
#include <atomic>

using namespace shaderc;

class Context::State {
public:
  State() { ++refCount_; }
  
  State(const State&) = delete;
  
  ~State() {
    // We track a global reference count. If it ever hits zero, then it's time 
    // to clean up LLVM.
    if (0 >= --refCount_) 
      llvm::llvm_shutdown();
  }
private:
  static std::atomic<int> refCount_;
};

std::atomic<int> Context::State::refCount_{0};