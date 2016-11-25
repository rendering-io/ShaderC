#ifndef SHADERC_LEXER_LEXER_H
#define SHADERC_LEXER_LEXER_H

#include <cstddef>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <string>

namespace shaderc {

// Represents a single token in our shader language.
class Token {
private:
  std::unique_ptr<char[]> lexme_;

  friend std::ostream & operator<<(std::ostream &os, const Token& t);
};

inline std::ostream & operator<<(std::ostream &os, const Token& t) {
  return os << "<,>";
}

// A list of tokens.
using TokenList = std::list<Token>;

class Lexer {
private:
  enum class ParseState {
    // The parser is in it's initial state, or has fully consumed the last token
    // and is ready for another.
    INITIAL,
    // Parsing an identifier or keyword.
    IDENTIFIER,
    // The parser has transitioned to the error state and we should abandon 
    // parsing
    INVALID,
  };
public:
  // Parses an input string into a a sequence of words/tokens.
  TokenList parse(const std::string& in) { return parse(in.c_str(), in.size()); }
  TokenList parse(const char *in) { return parse(in, std::strlen(in)); }
  TokenList parse(const char *in, size_t size);

private:
  void advance(size_t n) { head_ += n; }
  bool consume(TokenList&);

  // Consume the next character, assuming that it is a brace, bracket or 
  // parentheses.
  ParseState consumeBracket(TokenList&);

  // Consume the next character, assuming we are in the initial parse state.
  ParseState consumeInitial(TokenList&);

  // Consume the next character, assuming we are in the identifier parse state.
  ParseState consumeIdentifier(TokenList&);

  bool done() { return head_ >= end_; };
  bool isLineEnding(char c) { return c == '\n' || c == '\r'; }
  bool isAlphabetic(char c) const;
  bool isBracket(char c) const;
  bool isDigit(char c) const;
  bool isWhitespace(char c) const;
  void reset(const char *input, size_t size);
  char head() { return *head_; }
  char lookahead(size_t n = 1) { 
    return head_ + n < end_ ? *(head_ + n) : 0;
  }

  // The current head of the current parse string.
  const char *head_;

  // Marks the end of the current parse string.
  const char *end_;

  // The current parse state,
  ParseState parse_state_;
};

}

#endif // SHADERC_LEXER_LEXER_H
