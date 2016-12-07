#include <shaderc/AST/ASTPrettyPrinter.h>
#include <shaderc/CodeGen/CodeGenerator.h>
#include <shaderc/CodeGen/IRModule.h>
#include <shaderc/Lexer/Lexer.h>
#include <shaderc/Parser/Parser.h>
#include <shaderc/Support/Context.h>
#include <fstream>
#include <iostream>

using namespace shaderc;

std::string readSourceFile(const char* path) {
  // Try to open the source file and seek to the end.
  std::ifstream in;

  // Set excpetions to throw on failure.
  in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  in.open(path, std::ios::in | std::ios::ate);

  // Construct a string, sized to hold the full file.
  std::string contents(in.tellg(), 0);

  // Seek back to the start and then read the entire file.
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();

  return contents;
}

int main(int argc, char *argv[]) {
  // Disable syncing between stdio and iostreams.
  std::ios::sync_with_stdio(false);

  Context ctx;
  try {
    if (1 >= argc)
      return -1;

    // Read the input source file.
    std::string source = readSourceFile(argv[argc - 1]);

    // Scan and lex the input.
    Lexer lexer;
    auto tokens = lexer.parse(source);

    // Print the tokens.
    for (auto& token: tokens) {
      std::cout << token << '\n';
    }

    std::cout << '\n';

    // Parse our token stream.
    Parser parser;
    TranslationUnitPtr module = parser.parse(tokens);

    ASTPrettyPrinter printer{std::cout};
    printer.print(*module);

    std::cout << '\n';

    CodeGenerator generator{ctx};
    auto irModule = generator.buildIRModule(*module);
    irModule->print(std::cout);
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
