#include <iostream>
#include <fstream>
#include <memory>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

using namespace ninx::lexer;
using namespace ninx::lexer::token;
using namespace ninx::parser;
using namespace std;

int main() {
    std::ifstream source{R"(test/data/test.txt)", std::ios_base::binary};

    Lexer lexer {source, "test"};

    auto tokens = lexer.generate();

    Parser parser{tokens, "test"};

    parser.parse();

    return 0;
}