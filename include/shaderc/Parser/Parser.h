#ifndef SHADERC_PARSER_PARSER_H
#define SHADERC_PARSER_PARSER_H

#include <shaderc/Lexer/Lexer.h>

namespace shaderc {

class ParseError : std::exception { };

class Parser {
public:
  Parser();
  void parse(TokenList& tokens);
  
private:
  bool tokensRemaining() const;
  bool isKeyword(const Token& token, const char* name) const;

  void parseModule();
  void parseFunction();

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
