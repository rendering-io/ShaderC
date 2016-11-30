#include <shaderc/Parser/Parser.h>

using namespace shaderc;

Parser::Parser() : tokens_{nullptr} {
	
}

void Parser::parse(TokenList& tokens) {
	tokens_ = &tokens;

	parseModule();
}

bool Parser::tokensRemaining() const {
	return !tokens_->empty();
}

Token& Parser::head() {
	return tokens_->front();
}

bool Parser::isKeyword(const Token& token, const char* name) const {
	return (token.type() == Token::Type::KEYWORD) && (name == token.lexme()); 
}

Token Parser::consumeToken(Token::Type type, const char* lexme) {
	if (tokens_->empty())
		throw ParseError{};
	
	Token token = std::move(tokens_->front());
	tokens_->pop_front();

	if (token.type() != type)
		throw ParseError{};

	if (lexme && lexme != token.lexme())
		throw ParseError{};

	return token;
}

Token Parser::consumeKeyword(const char* lexme) {
	return consumeToken(Token::Type::KEYWORD, lexme);
}

Token Parser::consumeIdentifier() {
	return consumeToken(Token::Type::IDENTIFIER, nullptr);
}

Token Parser::consumeLParen() {
	return consumeToken(Token::Type::LPAREN, nullptr);
}

Token Parser::consumeRParen() {
	return consumeToken(Token::Type::RPAREN, nullptr);
}

Token Parser::consumeLBrace() {
	return consumeToken(Token::Type::LBRACE, nullptr);
}

Token Parser::consumeRBrace() {
	return consumeToken(Token::Type::RBRACE, nullptr);
}

ParseNodePtr Parser::makeNode() {
  return std::make_unique<ParseNode>();
}

ParseNodePtr Parser::makeNode(Token token) {
  return std::make_unique<ParseNode>(std::move(token));
}

ParseNodePtr Parser::parseModule() {
  ParseNodePtr module = makeNode();
  while (tokensRemaining()) {
    ParseNodePtr value = parseGlobalValue();
    module->append(std::move(value));
  }

  return module;
}

ParseNodePtr Parser::parseGlobalValue() {
  Token& token = head();
  if (isKeyword(token, "fn")) {
    return parseFunction();
  } else {
    // We found an unexpected and unsupported token.
    throw ParseError{};
  }
}

ParseNodePtr Parser::parseFunction() {
  // Require that the first token is 'fn' keyword.
  consumeKeyword("fn");

  ParseNodePtr function = makeNode();

  // Get the function name.
  Token name = consumeIdentifier();
  ParseNodePtr function_name = makeNode(name);
  function->append(parseParameterList());
  function->append(parseFunctionBody());

  return function;
}

ParseNodePtr Parser::parseParameterList() {
  ParseNodePtr params = makeNode();
  consumeLParen();
  consumeRParen();
  return params;
}

ParseNodePtr Parser::parseFunctionBody() {
  ParseNodePtr body = makeNode();
  consumeLBrace();
  consumeRBrace();
  return body;
}
