#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <lexer/Lexer.h>
#include <parser/Parser.h>
#include <evaluator/DefaultEvaluator.h>

using namespace ninx::lexer;
using namespace ninx::lexer::token;
using namespace ninx::parser;
using namespace ninx::evaluator;
using namespace std;

int main() {
    //std::ifstream source{R"(/Users/freddy/Documents/GitHub/ninx/examples/example6.txt)", std::ios_base::binary};
    std::ifstream source{R"(D:\Git\ninx\examples\example6.txt)", std::ios_base::binary};

    Lexer lexer {source, "tests"};

    auto tokens = lexer.generate();

    Parser parser{tokens, "tests"};

    auto ast = parser.parse();

    std::cout << *ast << std::endl;

    std::cout << "############ OUTPUT #############" << std::endl;

    std::stringstream output;
    DefaultEvaluator eval {output};
    eval.evaluate(ast.get());

    std::cout << output.str() << std::endl;

    return 0;
}