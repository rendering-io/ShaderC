#ifndef SHADERC_CODEGEN_IRMODULE_H
#define SHADERC_CODEGEN_IRMODULE_H

#include <ostream>

namespace shaderc {

class IRModule {
public:
  virtual ~IRModule();

  virtual void print(std::ostream&) = 0;
};

}

#endif // SHADERC_CODEGEN_IRMODULE_H
