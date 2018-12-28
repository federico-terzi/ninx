#include <iostream>
#include <fstream>
#include <memory>
#include "lexer/Lexer.h"
#include "parser/Parser.h"
#include "evaluator/DefaultEvaluator.h"

using namespace ninx::lexer;
using namespace ninx::lexer::token;
using namespace ninx::parser;
using namespace ninx::evaluator;
using namespace std;

int main() {
    std::ifstream source{R"(test/data/test2.txt)", std::ios_base::binary};

    Lexer lexer {source, "test"};

    auto tokens = lexer.generate();

    Parser parser{tokens, "test"};

    auto ast = parser.parse();

    std::cout << *ast << std::endl;

    DefaultEvaluator eval;

    ast->accept(&eval);

    return 0;
}