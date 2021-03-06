#include <shaderc/Parser/Parser.h>
#include <gtest/gtest.h>

using namespace shaderc;

TEST(Parser, Parse_SingleEmptyFunction_DoesNotThrow) {
  // Build a token list for a valid function with no arguments.
  TokenList tokens;
  tokens.emplace_back(Token::Type::KEYWORD, "fn");
  tokens.emplace_back(Token::Type::IDENTIFIER, "name");
  tokens.emplace_back(Token::Type::LPAREN, "(");
  tokens.emplace_back(Token::Type::RPAREN, ")");
  tokens.emplace_back(Token::Type::LBRACE, "{");
  tokens.emplace_back(Token::Type::RBRACE, "}");

  Parser parser;
  
  EXPECT_NO_THROW(parser.parse(tokens));
}

TEST(Parser, Parse_TwoEmptyFunctions_DoesNotThrow) {
  // Build a token list for a pair of valid functions with no arguments.
  TokenList tokens;
  tokens.emplace_back(Token::Type::KEYWORD, "fn");
  tokens.emplace_back(Token::Type::IDENTIFIER, "f");
  tokens.emplace_back(Token::Type::LPAREN, "(");
  tokens.emplace_back(Token::Type::RPAREN, ")");
  tokens.emplace_back(Token::Type::LBRACE, "{");
  tokens.emplace_back(Token::Type::RBRACE, "}");

  tokens.emplace_back(Token::Type::KEYWORD, "fn");
  tokens.emplace_back(Token::Type::IDENTIFIER, "g");
  tokens.emplace_back(Token::Type::LPAREN, "(");
  tokens.emplace_back(Token::Type::RPAREN, ")");
  tokens.emplace_back(Token::Type::LBRACE, "{");
  tokens.emplace_back(Token::Type::RBRACE, "}");

  Parser parser;
  
  EXPECT_NO_THROW(parser.parse(tokens));
}

TEST(Parser, Parse_FunctionWithoutIdentifier_ThrowsParseError) {
  // Build a token list for a function with no name.
  TokenList tokens;
  tokens.emplace_back(Token::Type::KEYWORD, "fn");
  tokens.emplace_back(Token::Type::LPAREN, "(");
  tokens.emplace_back(Token::Type::RPAREN, ")");
  tokens.emplace_back(Token::Type::LBRACE, "{");
  tokens.emplace_back(Token::Type::RBRACE, "}");

  Parser parser;
  
  EXPECT_THROW(parser.parse(tokens), ParseError);
}
