#ifndef SHADERC_AST_TYPE_H
#define SHADERC_AST_TYPE_H

#include <string>

namespace shaderc {

class Type {
public:
  const std::string& name() const { return name_; }
private:
  std::string name_;
};

}

#endif // SHADERC_AST_TYPE_H
