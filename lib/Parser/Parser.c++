#include <shaderc/Parser/Parser.h>

using namespace shaderc;

Parser::Parser() : tokens_{nullptr} {
	
}

TranslationUnitPtr Parser::parse(TokenList& tokens) {
	tokens_ = &tokens;

	return parseModule();
}

bool Parser::tokensRemaining() const {
	return !tokens_->empty();
}

Token& Parser::head() {
	return tokens_->front();
}

bool Parser::isTokenType(const Token& token, Token::Type type) const {
  return (token.type() == type); 
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

TranslationUnitPtr Parser::parseModule() {
  TranslationUnitPtr module = std::make_unique<TranslationUnit>();
  while (tokensRemaining()) {
    auto value = parseGlobalDecl();
    module->append(std::move(value));
  }

  return module;
}

GlobalDeclPtr Parser::parseGlobalDecl() {
  Token& token = head();
  if (isTokenType(token, 
                  Token::Type::FUNCTION_DECL, 
                  Token::Type::COMPUTE_SHADER_DECL,
                  Token::Type::VERTEX_SHADER_DECL,
                  Token::Type::HULL_SHADER_DECL,
                  Token::Type::DOMAIN_SHADER_DECL,
                  Token::Type::GEOMETRY_SHADER_DECL,
                  Token::Type::FRAGMENT_SHADER_DECL)) {
    return parseFunctionDecl();
  } else {
    // We found an unexpected and unsupported token.
    throw ParseError{};
  }
}

FunctionDeclPtr Parser::parseFunctionDecl() {
  // Require that the first token is a function or shader decl.
  Token& decl = head();
  if (!isTokenType(decl, 
                   Token::Type::FUNCTION_DECL, 
                   Token::Type::COMPUTE_SHADER_DECL,
                   Token::Type::VERTEX_SHADER_DECL,
                   Token::Type::HULL_SHADER_DECL,
                   Token::Type::DOMAIN_SHADER_DECL,
                   Token::Type::GEOMETRY_SHADER_DECL,
                   Token::Type::FRAGMENT_SHADER_DECL))
    throw ParseError{};

  consumeToken(decl.type());

  // Get the function name.
  Token name = consumeIdentifier();

  auto function = std::make_unique<FunctionDecl>(name.lexme());

  //ParseNodePtr function_name = makeNode(name);
  parseParameterList();
  parseFunctionBody();

  return function;
}

void Parser::parseParameterList() {
  consumeLParen();
  consumeRParen();
}

void Parser::parseFunctionBody() {
  consumeLBrace();
  consumeRBrace();
}
