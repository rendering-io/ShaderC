#include <shaderc/Lexer/Lexer.h>
#include <cassert>

using namespace shaderc;

void Lexer::reset(const char *input, size_t size) {
  head_ = input;
  end_ = input + size;
  parse_state_ = ParseState::INITIAL;
}

bool Lexer::consume() {
  ParseState next_state;
  switch (parse_state_) {
  case ParseState::INITIAL:
    next_state = consumeInitial();
    break;
  
  default:
    assert(false);
  }

  parse_state_ = next_state;
  return !done();
}

// Consume the next character, assuming we are in the initial parse state.
Lexer::ParseState Lexer::consumeInitial() {
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
    advance(1);
    return ParseState::IDENTIFIER;  
  }

  assert(false);
}

bool Lexer::isAlphabetic(char c) const {
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
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
  while (consume()) { }
      
  return tokens;
}
