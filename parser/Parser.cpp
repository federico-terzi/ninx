//
// Created by federico.terzi2 on 08/12/2018.
//

#include "Parser.h"

ninx::parser::Parser::Parser(std::vector<std::unique_ptr<Token>> &tokens, const std::string &origin) : origin(
        origin), reader{tokens} {}

void ninx::parser::Parser::parse() {

}

/*
void ninx::parser::Parser::generate() {
Token * current_token;
while ((current_token = reader.get_token())) {
std::cout << *current_token << std::endl;
}
}
*/