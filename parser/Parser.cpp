//
// Created by federico.terzi2 on 08/12/2018.
//

#include "Parser.h"
#include "element/TextElement.h"
#include "../lexer/token/Text.h"
#include "../lexer/token/Limiter.h"
#include "exception/ParserException.h"
#include "element/FunctionCall.h"
#include "../lexer/token/Keyword.h"

using namespace ninx::parser::exception;

ninx::parser::Parser::Parser(std::vector<std::unique_ptr<Token>> &tokens, const std::string &origin) : origin(origin),
                                                                                                       reader(tokens) {

}

std::unique_ptr<Block> ninx::parser::Parser::parse_block() {
    Token *open_bracket{reader.get_token()};
    if (!open_bracket || open_bracket->get_type() != Type::LIMITER ||
        dynamic_cast<ninx::lexer::token::Limiter *>(open_bracket)->get_limiter() != '{') {
        throw ParserException(open_bracket, this->origin, "Expected open bracket {, but could not find one.");
    }

    auto block {parse_implicit_block()};

    Token *close_bracket{reader.get_token()};
    if (!close_bracket || close_bracket->get_type() != Type::LIMITER ||
        dynamic_cast<ninx::lexer::token::Limiter *>(close_bracket)->get_limiter() != '}') {
        throw ParserException(close_bracket, this->origin, "Expected close bracket }, but could not find one.");
    }

    return block;
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
                auto limiter = dynamic_cast<ninx::lexer::token::Limiter *>(token);
                switch (limiter->get_limiter()) {
                    case '{': {
                        reader.seek_previous();
                        auto block{this->parse_block()};
                        return block;
                    }
                }

                // The given limiter does not trigger any built-in statement, rewind to avoid consuming it.
                reader.seek_previous();
                break;
            }
            case Type::FUNCNAME: {
                auto element = std::make_unique<FunctionCall>(dynamic_cast<ninx::lexer::token::Keyword *>(token)->get_keyword());
                return element;
            }
            case Type::TEXT: {
                auto element = std::make_unique<TextElement>(
                        dynamic_cast<ninx::lexer::token::Text *>(token)->get_text());
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
    std::cout << *parse_implicit_block() << std::endl;
}

std::unique_ptr<Block> ninx::parser::Parser::parse_implicit_block() {
    std::vector<std::unique_ptr<Statement>> statements;

    std::unique_ptr<Statement> current;
    while ((current = parse_statement())) {
        statements.push_back(std::move(current));


        // Check if the block will be closed
        Token *close_bracket{reader.peek_token()};
        if (close_bracket && close_bracket->get_type() == Type::LIMITER &&
            dynamic_cast<ninx::lexer::token::Limiter *>(close_bracket)->get_limiter() == '}') {
            // Exit the cycle, because the next token marks the end of a block
            break;
        }
    }

    auto block = std::make_unique<Block>(std::move(statements));
    return std::move(block);
}

/*
void ninx::parser::Parser::generate() {
Token * current_token;
while ((current_token = reader.get_token())) {
std::cout << *current_token << std::endl;
}
}
*/