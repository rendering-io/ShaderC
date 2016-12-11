#ifndef SHADERC_LEXER_LEXER_H
#define SHADERC_LEXER_LEXER_H

#include <cstddef>
#include <cstring>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

namespace shaderc {

// Represents a single token in our shader language.
class Token {
public:
  enum class Type {
    INVALID,
    KEYWORD,
    IDENTIFIER,
    LBRACKET,
    RBRACKET,
    LBRACE,
    RBRACE,
    LPAREN,
    RPAREN,
    COLON,
    TERMINATOR,
    RARROW,
    LITERAL_INTEGER,
    FUNCTION_DECL,
    COMPUTE_SHADER_DECL,
    VERTEX_SHADER_DECL,
    HULL_SHADER_DECL,
    DOMAIN_SHADER_DECL,
    GEOMETRY_SHADER_DECL,
    FRAGMENT_SHADER_DECL,
    TYPE_DECL,
    RETURN,
  };
  Token() : lexme_{}, type_{Type::INVALID} { }
  Token(Type type, std::string lexme) : lexme_{lexme}, type_{type} { }
  
  const std::string& lexme() const { return lexme_; }
  Type type() const { return type_; }

private:
  std::string lexme_;
  Type type_;

  friend std::ostream & operator<<(std::ostream &os, const Token& t);
};

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
  Lexer();

  // Parses an input string into a a sequence of words/tokens.
  TokenList parse(const std::string& in) { return parse(in.c_str(), in.size()); }
  TokenList parse(const char *in) { return parse(in, std::strlen(in)); }
  TokenList parse(const char *in, size_t size);

private:
  void advance(size_t n) { head_ += n; }
  bool consume(TokenList&);
  void emitToken(TokenList& tokens, Token::Type type);
  void emitKeywordOrIdentifier(TokenList& tokens);

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

  // A map of keywords to tokens.
  std::unordered_map<std::string, Token::Type> keywords_;

  // A buffer to accumulate the lexme into.
  std::vector<char> lexme_;

  // The current head of the current parse string.
  const char *head_;

  // Marks the end of the current parse string.
  const char *end_;

  // The current parse state,
  ParseState parse_state_;
};

}

#endif // SHADERC_LEXER_LEXER_H
