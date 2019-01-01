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
#include "element/VariableRead.h"
#include "../lexer/token/Variable.h"
#include "element/Assignment.h"

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
                // TODO: move this part to a separate function

                // Extract the function definition identifier
                auto id_token {reader.get_token()};
                if (!id_token) {
                    throw ParserException(id_token, this->origin, "Expected function name, but could not find one.");
                }
                if (id_token->get_type() != Type::TEXT) {
                    throw ParserException(id_token, this->origin, "Function name must be alphanumeric, and begin with a letter.");
                }
                auto id {dynamic_cast<ninx::lexer::token::Text *>(id_token)->get_identifier()};
                if (!id) {
                    throw ParserException(id_token, this->origin, "Invalid function name. It must be alphanumeric, and begin with a letter.");
                }

                // Extract the parameters
                if (reader.check_limiter('(') != 1) {
                    throw ParserException(id_token, this->origin, "Expected '(' but could not find one.");
                }
                reader.get_token();

                // Cycle until a closing parenthesis is found or EOF is reached
                int result;
                while((result = reader.check_limiter(')')) == 0) {
                    auto argument_token {reader.get_token()};

                    if (argument_token->get_type() != Type::VARIABLE) {
                        throw ParserException(argument_token, this->origin, "Argument expected but not found. Maybe "
                                                                            "you forgot to prepend $ to the argument"
                                                                            " name?");
                    }

                    std::unique_ptr<Block> default_value {nullptr};

                    // Check if the argument does have a default value
                    if (reader.check_limiter('=')) {
                        reader.get_token();

                        default_value = parse_block();
                    }

                    if (reader.check_limiter(',') == 1) {
                        // Skip the period limiter
                        reader.get_token();
                    }else if(reader.check_limiter(')') == 1) {
                        break;
                    }else{
                        throw ParserException(argument_token, this->origin, "Another argument or end of arguments expected.");
                    }
                }
                reader.get_token(); // Skip the closing parenthesis
                if (result == -1) {
                    throw ParserException(nullptr, this->origin, "Expected ')' but EOF has been reached.");
                }

                // Parse the function block
                auto function_block {parse_block()};
            }
            case Type::OPDEF: {
                // TODO: parse_operator_definition
                break;
            }
            case Type::VARIABLE: {
                // Get the variable name
                auto name {dynamic_cast<ninx::lexer::token::Variable *>(token)->get_name()};

                // Determine if the variable is used in an assignment by peeking the next
                // token and checking if it is a limiter equal to =
                if (reader.check_limiter('=') == 1) {  // Assignment
                    // Skip the = limiter
                    reader.get_token();

                    // Parse the block containing the assignment value
                    auto block = parse_block();

                    auto element = std::make_unique<ninx::parser::element::Assignment>(name, std::move(block));
                    return element;
                }else{  // Variable used as value
                    auto element = std::make_unique<ninx::parser::element::VariableRead>(name);
                    return element;
                }
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

std::unique_ptr<Block> ninx::parser::Parser::parse() {
    return parse_implicit_block();
}

std::unique_ptr<Block> ninx::parser::Parser::parse_implicit_block() {
    std::vector<std::unique_ptr<Statement>> statements;

    std::unique_ptr<Statement> current;
    while ((current = parse_statement())) {
        statements.push_back(std::move(current));


        // Check if the block will be closed
        if (reader.check_limiter('}') > 0) {
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