#include <shaderc/Lexer/Lexer.h>
#include <gtest/gtest.h>

using namespace shaderc;

TEST(Lexer, Parse_ZeroLengthString_ReturnsEmptyTokenList) {
  Lexer lexer;
  auto tok = lexer.parse("XXX", 0);
  EXPECT_TRUE(tok.empty());
}

TEST(Lexer, Parse_NullString_ReturnsEmptyTokenList) {
  Lexer lexer;
  auto tok = lexer.parse(nullptr, 99);
  EXPECT_TRUE(tok.empty());
}

TEST(Lexer, Parse_Whitespace_ReturnsEmptyTokenList) {
  Lexer lexer;
  auto tok = lexer.parse(" \t\n\r", 4);
  EXPECT_TRUE(tok.empty());
}

TEST(Lexer, Parse_SingleIdentifier_ReturnsSingleIdentiferTokenList) {
  Lexer lexer;
  auto tok = lexer.parse(" x ");
  EXPECT_EQ(1, tok.size());
}

TEST(Lexer, Parse_TwoIdentifiers_ReturnsTwoIdentiferTokenList) {
  Lexer lexer;
  auto tok = lexer.parse(" x y ");
  EXPECT_EQ(2, tok.size());
}

TEST(Lexer, Parse_SingleCharacterIdentifierWithoutWhitespace_ReturnsSingleIdentiferTokenList) {
  Lexer lexer;
  auto tok = lexer.parse("x");
  EXPECT_EQ(1, tok.size());
}

TEST(Lexer, Parse_SingleIdentifierWithoutWhitespace_ReturnsSingleIdentiferTokenList) {
  Lexer lexer;
  auto tok = lexer.parse("xx");
  EXPECT_EQ(1, tok.size());
}

TEST(Lexer, Parse_Integer_ReturnsSingleIntegerLiteral) {
  Lexer lexer;
  auto tok = lexer.parse("99");
  ASSERT_EQ(1, tok.size());
  EXPECT_EQ(Token::Type::LITERAL_INTEGER, tok.front().type());
}

TEST(Lexer, Parse_IntegerSemiColon_ReturnsIntegerLiteralAndTerminator) {
  Lexer lexer;
  auto tok = lexer.parse("88;");
  ASSERT_EQ(2, tok.size());
  EXPECT_EQ(Token::Type::LITERAL_INTEGER, tok.front().type());
  tok.pop_front();
  EXPECT_EQ(Token::Type::TERMINATOR, tok.front().type());
}
