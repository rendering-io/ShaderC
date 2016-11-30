#include <shaderc/Lexer/Lexer.h>
#include <cassert>

namespace shaderc {

std::ostream & operator<<(std::ostream &os, const Token& t) {
  std::string typeStr;
  switch (t.type_) {
  case Token::Type::IDENTIFIER: typeStr = "IDENTIFIER"; break;
  case Token::Type::KEYWORD:    typeStr = "KEYWORD";    break;
  case Token::Type::LBRACKET:   typeStr = "LBRACKET";   break;
  case Token::Type::RBRACKET:   typeStr = "RBRACKET";   break;
  case Token::Type::LBRACE:     typeStr = "LBRACE";     break;
  case Token::Type::RBRACE:     typeStr = "RBRACE";     break;
  case Token::Type::LPAREN:     typeStr = "LPAREN";     break;
  case Token::Type::RPAREN:     typeStr = "RPAREN";     break;
  default:
    assert(false && "Unsupported token type.");
  }

  return os << "<" << typeStr << "," << t.lexme() << ">";
}

}

using namespace shaderc;

void Lexer::reset(const char *input, size_t size) {
  head_ = input;
  end_ = input + size;
  parse_state_ = ParseState::INITIAL;
}

bool Lexer::consume(TokenList& tokens) {
  ParseState next_state;
  switch (parse_state_) {
  case ParseState::INITIAL:
    next_state = consumeInitial(tokens);
    break;
  
  case ParseState::IDENTIFIER:
    next_state = consumeIdentifier(tokens);
    break;
  
  default:
    assert(false);
  }

  parse_state_ = next_state;
  return !done();
}

Lexer::ParseState Lexer::consumeBracket(TokenList& tokens) {
  assert(ParseState::INITIAL == parse_state_);

  // Get the current character.
  char c = head();
  assert(isBracket(c));

  Token::Type type;
  switch (c) {
  case '[':
    type = Token::Type::LBRACKET; 
    break;
  case ']':
    type = Token::Type::RBRACKET; 
    break;
  case '{':
    type = Token::Type::LBRACE; 
    break;
  case '}':
    type = Token::Type::RBRACE; 
    break;
  case '(':
    type = Token::Type::LPAREN; 
    break;
  case ')':
    type = Token::Type::RPAREN; 
    break;

  default:
    assert(false);
    return ParseState::INVALID;
  }

  lexme_.push_back(c);
  lexme_.push_back(0);
  tokens.emplace_back(type, lexme_.data());
  lexme_.clear();
  advance(1);
  return ParseState::INITIAL;
}

// Consume the next character, assuming we are in the initial parse state.
Lexer::ParseState Lexer::consumeInitial(TokenList& tokens) {
  assert(ParseState::INITIAL == parse_state_);

  // Get the current character.
  char c = head();

  // If the character is whitespace then consume it, and return in initial 
  // state again.
  if (isWhitespace(c)) {
    advance(1);
    return ParseState::INITIAL;
  }
  
  // If the character is alphabetic, then we are starting a keyword or 
  // identifier.
  if (isAlphabetic(c)) {
    lexme_.push_back(c);
    advance(1);
    return ParseState::IDENTIFIER;  
  }

  if (isBracket(c)) {
    return consumeBracket(tokens);
  }
  assert(false);
}

// Consume the next character, assuming we are in the identifier parse state.
Lexer::ParseState Lexer::consumeIdentifier(TokenList& tokens) {
  assert(ParseState::IDENTIFIER == parse_state_);

  // Get the current character.
  char c = head();

  // If the character is alphabetic, then we are extending a keyword or 
  // identifier.
  if (isAlphabetic(c)) {
    lexme_.push_back(c);
    advance(1);
    return ParseState::IDENTIFIER;  
  }

  // If we reach this point, then we have a character that isn't valid in an
  // identifier. We should push the identifier to the token list and then 
  // return. We don't advance, because we need the invalid character to be 
  // consumed by a state that can handle it.
  lexme_.push_back(0);
  tokens.emplace_back(Token::Type::IDENTIFIER, lexme_.data());
  lexme_.clear();
  
  return ParseState::INITIAL;
}

bool Lexer::isAlphabetic(char c) const {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool Lexer::isBracket(char c) const {
  switch(c) {
  case '[':
  case ']':
  case '{':
  case '}':
  case '(':
  case ')':
    return true;

  default:
    return false;
  }
}

bool Lexer::isDigit(char c) const {
  return ('0' <= c && c <= '9');
}

bool Lexer::isWhitespace(char c) const {
  switch (c) {
  case ' ':
  case '\t':
  case '\n':
  case '\r':
    return true;
  }

  return false;
}

TokenList Lexer::parse(const char *input, size_t size) {
  TokenList tokens;

  // If the input is null, or zero length, then return an empty token list.
  if (0 == size || nullptr == input)
    return tokens;

  // Reset the parse state.
  reset(input, size);

  // Loop until all input has been consumed.
  while (consume(tokens)) { }
      
  return tokens;
}
