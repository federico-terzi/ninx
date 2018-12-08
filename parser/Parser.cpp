//
// Created by federico.terzi2 on 08/12/2018.
//

#include "Parser.h"

ninx::parser::Parser::Parser(std::vector<std::unique_ptr<Token>> &tokens, const std::string &origin) : origin(
        origin), tokens(tokens) {}
