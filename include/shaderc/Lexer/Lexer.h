#ifndef SHADERC_LEXER_LEXER_H
#define SHADERC_LEXER_LEXER_H

#include <cstddef>
#include <cstring>
#include <list>

namespace shaderc {

// Represents a single token in our shader language.
class Token {
private:
};

// A list of tokens.
using TokenList = std::list<Token>;

class Lexer {
public:
  // Parses an input string into a a sequence of words/tokens.
  TokenList parse(const char *in) { return parse(in, std::strlen(in)); }
  TokenList parse(const char *in, size_t size);

private:
  void advance(size_t n) { head_ += n; }
  bool done() { return head_ >= end_; };
  bool isLineEnding(char c) { return c == '\n' || c == '\r'; }
  void reset(const char *input, size_t size);
  char lookahead(size_t n = 1) { 
    return head_ + n < end_ ? *(head_ + n) : 0;
  }

  // The current head of the current parse string.
  const char *head_;

  // Marks the end of the current parse string.
  const char *end_;
};

}

#endif // SHADERC_LEXER_LEXER_H
