#include <iostream>
#include <memory>
#include "lexer/Lexer.h"
#include "lexer/token/BaseToken.h"
#include "lexer/token/TextToken.h"

using namespace ninx::lexer;
using namespace ninx::lexer::token;
using namespace std;

int main() {
    Lexer lexer;

    unique_ptr<token::BaseToken> token { make_unique<token::TextToken>("Bella raga") };

    auto t { dynamic_cast<TextToken*>(token.get()) };

    cout << t->get_text();

    return 0;
}