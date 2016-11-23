#include <shaderc/Lexer/Lexer.h>

using namespace shaderc;

void Lexer::reset(const char *input, size_t size) {
  head_ = input;
  end_ = input + size;
}

TokenList Lexer::parse(const char *input, size_t size) {
  TokenList tokens;

  // If the input is null, or zero length, then return an empty token list.
  if (0 == size)
    return tokens;

  // Reset the parse state.
  reset(input, size);

  // Loop until all input has been consumed.
  while (!done()) {
    //if (isLineEnding())
    
    advance(1);
  }
      
  return tokens;
}
