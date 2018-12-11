//
// Created by federico.terzi2 on 08/12/2018.
//

#include "Parser.h"
#include "element/TextElement.h"
#include "../lexer/token/Text.h"

ninx::parser::Parser::Parser(std::vector<std::unique_ptr<Token>> &tokens, const std::string &origin) : origin(origin),
                                                                                                       reader(tokens) {

}

std::unique_ptr<Statement> ninx::parser::Parser::parse_statement() {
    Token *token{reader.get_token()};

    if (token) {
        switch (token->get_type()) {
            case Type::FUNCDEF: {
                // TODO: parse_function_definition
                break;
            }
            case Type::OPDEF: {
                // TODO: parse_operator_definition
                break;
            }
            case Type::VARIABLE: {
                // TODO: parse_variable
                break;
            }
            case Type::LIMITER: {
                // TODO: parse_operator_block
                break;
            }
            case Type::FUNCNAME: {
                // TODO: parse_function_call
                break;
            }
            case Type::TEXT: {
                auto element = std::make_unique<TextElement>(dynamic_cast<ninx::lexer::token::Text*>(token)->get_text());
                return element;
            }
        }

        // TODO: throw parse exception for unexpected token
    }

    // End of the document
    return nullptr;
}

std::unique_ptr<Block> ninx::parser::Parser::parse_function_call() {
    return std::unique_ptr<Block>();
}

void ninx::parser::Parser::parse() {
    std::cout << *parse_statement() << std::endl;
}

/*
void ninx::parser::Parser::generate() {
Token * current_token;
while ((current_token = reader.get_token())) {
std::cout << *current_token << std::endl;
}
}
*/