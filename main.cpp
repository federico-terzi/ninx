#include <iostream>
#include <fstream>
#include <memory>
#include "lexer/Lexer.h"

using namespace ninx::lexer;
using namespace ninx::lexer::token;
using namespace std;

int main() {
    std::ifstream source{R"(test/data/test.txt)", std::ios_base::binary};

    Lexer lexer {source, "test"};

    auto tokens = lexer.generate();

    return 0;
}