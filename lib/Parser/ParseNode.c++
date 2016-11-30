#include <shaderc/Parser/Parser.h>

using namespace shaderc;

ParseNode::ParseNode() { }

ParseNode::ParseNode(Token token) : token_{std::move(token)} { }

void ParseNode::append(ParseNodePtr child) {
  children_.emplace_back(std::move(child));
}
