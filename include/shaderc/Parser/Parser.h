#ifndef SHADERC_PARSER_PARSER_H
#define SHADERC_PARSER_PARSER_H

#include <shaderc/Lexer/Lexer.h>
#include <memory>
#include <vector>

namespace shaderc {

class ParseError : std::exception { };

class ParseNode;
using ParseNodePtr = std::unique_ptr<ParseNode>;

class ParseNode {
public:
  ParseNode();
  ParseNode(Token token);
  void append(ParseNodePtr node);
private:
  std::vector<ParseNodePtr> children_;
  Token token_;
};

class Parser {
public:
  Parser();
  void parse(TokenList& tokens);
  
private:
  bool tokensRemaining() const;
  bool isKeyword(const Token& token, const char* name) const;
  ParseNodePtr makeNode();
  ParseNodePtr makeNode(Token token);

  ParseNodePtr parseModule();
  ParseNodePtr parseGlobalValue();
  ParseNodePtr parseFunction();
  ParseNodePtr parseParameterList();
  ParseNodePtr parseFunctionBody();

  Token consumeToken(Token::Type, const char* lexme);
  Token consumeKeyword(const char* lexme);
  Token consumeIdentifier();
  Token consumeLParen();
  Token consumeRParen();
  Token consumeLBrace();
  Token consumeRBrace();
  Token& head();

  TokenList* tokens_;
};

}

#endif // SHADERC_PARSER_PARSER_H
