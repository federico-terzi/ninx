/*

MIT License

Copyright (c) 2018 Federico Terzi

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Parser.h"
#include "element/Block.h"
#include "element/TextElement.h"
#include "../lexer/token/Text.h"
#include "../lexer/token/Limiter.h"
#include "exception/ParserException.h"
#include "element/FunctionCall.h"
#include "../lexer/token/Keyword.h"
#include "element/VariableRead.h"
#include "../lexer/token/Variable.h"
#include "element/Assignment.h"
#include "parser/element/expression/Expression.h"
#include "parser/element/expression/AddExpression.h"
#include "parser/element/expression/MultiplicationExpression.h"
#include "parser/element/expression/DivisionExpression.h"
#include "parser/element/expression/SubtractExpression.h"
#include "parser/element/expression/EqualExpression.h"
#include "parser/element/expression/NotEqualExpression.h"
#include "element/FunctionCallArgument.h"
#include "element/IfCase.h"
#include "element/IfCondition.h"

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

    auto block{parse_implicit_block()};

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
                return parse_function_definition();
            }
            case Type::OPDEF: {
                // TODO: parse_operator_definition
                break;
            }
            case Type::VARIABLE: {
                // Get the variable name
                auto name{dynamic_cast<ninx::lexer::token::Variable *>(token)->get_name()};

                // Determine if the variable is used in an assignment by peeking the next
                // token and checking if it is a limiter equal to =
                if (reader.check_limiter('=') == 1) {  // Assignment
                    // Skip the = limiter
                    reader.get_token();

                    auto value = parse_expression();

                    auto element = std::make_unique<ninx::parser::element::Assignment>(name, std::move(value));
                    return element;
                } else {  // Variable used as value
                    auto element = std::make_unique<ninx::parser::element::VariableRead>(name);
                    return element;
                }
            }
            case Type::LIMITER: {
                auto limiter = dynamic_cast<ninx::lexer::token::Limiter *>(token);
                switch (limiter->get_limiter()) {
                    default:  // Not defined as an operator
                    {
                        auto element = std::make_unique<TextElement>(std::string(1, limiter->get_limiter()));
                        return element;
                    }
                }
            }
            case Type::FUNCNAME: {
                reader.seek_previous();  // Seek to the previous token to make the function able to read it again
                return parse_function_call();
            }
            case Type::IF: {
                reader.seek_previous();  // Rewind the if block
                return parse_if_condition();
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

std::unique_ptr<IfCondition> ninx::parser::Parser::parse_if_condition() {
    if (reader.check_type(Type::IF) != 1) {
        generate_exception("Expected If statement");
    }
    reader.get_token();

    std::vector<std::unique_ptr<IfCase>> cases;

    // Parse the main condition
    auto condition {parse_expression()};
    auto body {parse_block()};
    auto main_case = std::make_unique<IfCase>(std::move(condition), std::move(body));
    cases.push_back(std::move(main_case));

    // Parse optional else if statements
    while(reader.check_type(Type::ELSEIF) == 1) {
        reader.get_token();

        auto elseif_condition {parse_expression()};
        auto elseif_body {parse_block()};
        auto elseif_case = std::make_unique<IfCase>(std::move(elseif_condition), std::move(elseif_body));
        cases.push_back(std::move(elseif_case));
    }

    // Parse optional else statement
    std::unique_ptr<Block> else_body {nullptr};
    if (reader.check_type(Type::ELSE) == 1){
        reader.get_token();

        else_body = parse_block();
    }


    auto if_condition = std::make_unique<IfCondition>(std::move(cases), std::move(else_body));

    return if_condition;
}

std::unique_ptr<Expression> ninx::parser::Parser::parse_value() {
    std::unique_ptr<Expression> value{nullptr};

    // Check if is a block value or a variable read
    if (reader.check_limiter('{') == 1) {
        value = parse_block();
    } else if (reader.check_limiter('(') == 1) {  // Nested expression
        // Consume the open parenthesis
        reader.get_token();

        value = parse_expression();

        // Check and consume the closing parenthesis
        if (reader.check_limiter(')') != 1) {
            auto error_token{reader.get_token()};
            throw ParserException(error_token, this->origin, "Expected closing parethesis ')'");
        }
        reader.get_token();

    } else if (reader.peek_token() != nullptr && reader.peek_token()->get_type() == Type::VARIABLE) {
        // Get the variable name
        auto name{dynamic_cast<ninx::lexer::token::Variable *>(reader.get_token())->get_name()};
        value = std::make_unique<ninx::parser::element::VariableRead>(name);
    } else {
        auto error_token{reader.get_token()};
        throw ParserException(error_token, this->origin, "Expected Block or Variable.");
    }

    return std::move(value);
}

std::unique_ptr<Expression>
ninx::parser::Parser::parse_sub_expression(std::function<std::unique_ptr<Expression>()> term_parser,
                                           std::vector<ninx::parser::OperatorCaseDefinition> operators) {
    std::unique_ptr<Expression> expression{nullptr};

    auto first{term_parser()};
    expression = std::move(first);

    while (true) {
        bool found{false};

        for (auto &case_def : operators) {
            if (reader.check_limiter_sequence(case_def.op) == 1) {
                // Remove the tokens
                for (int i = 0; i<case_def.op.length(); i++) {
                    reader.get_token();
                }

                auto second{term_parser()};

                auto sub_expr = case_def.builder(std::move(expression), std::move(second));
                expression = std::move(sub_expr);

                found = true;
                break;
            }
        }

        if (!found) {
            break;
        }
    }

    return std::move(expression);
}

template <typename T>
ninx::parser::OperatorCaseDefinition build_operator_case(const std::string &op) {
    return  {
            op,
            [](std::unique_ptr<Expression> first,
               std::unique_ptr<Expression> second) -> std::unique_ptr<Expression> {
                return std::make_unique<T>(std::move(first), std::move(second));
            }
    };
}

std::unique_ptr<Expression> ninx::parser::Parser::parse_level_1_expression() {
    static std::vector<ninx::parser::OperatorCaseDefinition> level_1_operators = {
            build_operator_case<MultiplicationExpression>("*"),
            build_operator_case<DivisionExpression>("/")
    };

    return parse_sub_expression([this] {
        return parse_value();
    }, level_1_operators);
}

std::unique_ptr<Expression> ninx::parser::Parser::parse_level_2_expression() {
    static std::vector<ninx::parser::OperatorCaseDefinition> level_2_operators = {
            build_operator_case<AddExpression>("+"),
            build_operator_case<SubtractExpression>("-")
    };

    return parse_sub_expression([this] {
        return parse_level_1_expression();
    }, level_2_operators);
}

std::unique_ptr<Expression> ninx::parser::Parser::parse_level_3_expression() {
    static std::vector<ninx::parser::OperatorCaseDefinition> level_3_operators = {
            build_operator_case<EqualExpression>("=="),
            build_operator_case<NotEqualExpression>("!=")
    };

    return parse_sub_expression([this] {
        return parse_level_2_expression();
    }, level_3_operators);
}

std::unique_ptr<Expression> ninx::parser::Parser::parse_expression() {
    return parse_level_3_expression();
}

std::unique_ptr<FunctionArgument> ninx::parser::Parser::parse_function_argument() {
    auto raw_token{reader.get_token()};

    if (raw_token->get_type() != Type::VARIABLE) {
        throw ParserException(raw_token, this->origin, "Argument expected but not found. Maybe "
                                                       "you forgot to prepend $ to the argument"
                                                       " name?");
    }

    std::unique_ptr<Expression> default_value{nullptr};

    // Check if the argument does have a default value
    if (reader.check_limiter('=')) {
        reader.get_token();

        default_value = parse_expression();
    }

    auto argument_token{dynamic_cast<ninx::lexer::token::Variable *>(raw_token)};

    auto argument = std::make_unique<FunctionArgument>(argument_token->get_name(), std::move(default_value));

    return argument;
}


std::unique_ptr<FunctionDefinition> ninx::parser::Parser::parse_function_definition() {
    // Extract the function definition identifier
    auto id_token{reader.get_token()};
    if (!id_token) {
        throw ParserException(id_token, this->origin, "Expected function name, but could not find one.");
    }
    if (id_token->get_type() != Type::TEXT) {
        throw ParserException(id_token, this->origin, "Function name must be alphanumeric, and begin with a letter.");
    }
    auto id_ptr{dynamic_cast<ninx::lexer::token::Text *>(id_token)->get_identifier()};
    if (!id_ptr) {
        throw ParserException(id_token, this->origin,
                              "Invalid function name. It must be alphanumeric, and begin with a letter.");
    }
    std::string id{*id_ptr};

    // Extract the parameters
    if (reader.check_limiter('(') != 1) {
        throw ParserException(id_token, this->origin, "Expected '(' but could not find one.");
    }
    reader.get_token();

    std::vector<std::unique_ptr<FunctionArgument>> arguments;

    // Cycle until a closing parenthesis is found or EOF is reached
    int result;
    while ((result = reader.check_limiter(')')) == 0) {
        auto argument{parse_function_argument()};

        arguments.push_back(std::move(argument));

        if (reader.check_limiter(',') == 1) {
            // Skip the period limiter
            reader.get_token();
        } else if (reader.check_limiter(')') == 1) {
            break;
        } else {
            auto error_token{reader.get_token()};
            throw ParserException(error_token, this->origin, "Another argument or end of arguments expected.");
        }
    }
    reader.get_token(); // Skip the closing parenthesis
    if (result == -1) {
        throw ParserException(nullptr, this->origin, "Expected ')' but EOF has been reached.");
    }

    // Parse the function block
    auto function_body{parse_block()};

    auto function_definition = std::make_unique<FunctionDefinition>(id, std::move(arguments), std::move(function_body));

    return function_definition;
}

std::unique_ptr<FunctionCallArgument> ninx::parser::Parser::parse_function_call_argument() {
    // Check if the function call argument use implicit naming ( without the argument name )
    // to do that, check if there is the beginning of a block.

    std::unique_ptr<std::string> id{nullptr}; // Argument name

    if (reader.check_limiter('{') == 0) {
        auto raw_token{reader.get_token()};

        if (raw_token->get_type() != Type::TEXT) {
            throw ParserException(raw_token, this->origin,
                                  "Invalid function argument name. It must be alphanumeric, and begin with a letter.");
        }
        auto id_ptr{dynamic_cast<ninx::lexer::token::Text *>(raw_token)->get_identifier()};
        if (!id_ptr) {
            throw ParserException(raw_token, this->origin,
                                  "Invalid function argument name. It must be alphanumeric, and begin with a letter.");
        }
        id = std::move(id_ptr);

        if (reader.check_limiter('=') != 1) {
            auto error_token{reader.get_token()};
            throw ParserException(error_token, this->origin,
                                  "Expected '=' after argument name, but could not find one.");
        }
        reader.get_token();  // Skip the = limiter
    }

    auto value{parse_expression()};

    auto argument = std::make_unique<FunctionCallArgument>(std::move(id), std::move(value));

    return argument;
}

std::unique_ptr<FunctionCall> ninx::parser::Parser::parse_function_call() {
    auto token{reader.get_token()};
    auto call_token{dynamic_cast<ninx::lexer::token::Keyword *>(token)};

    std::vector<std::unique_ptr<FunctionCallArgument>> arguments;

    // Parse optional call parameters
    if (reader.check_limiter('(') == 1) {
        reader.get_token();

        // Cycle until a closing parenthesis is found or EOF is reached
        int result;
        while ((result = reader.check_limiter(')')) == 0) {
            auto argument{parse_function_call_argument()};

            arguments.push_back(std::move(argument));

            if (reader.check_limiter(',') == 1) {
                // Skip the period limiter
                reader.get_token();
            } else if (reader.check_limiter(')') == 1) {
                break;
            } else {
                auto error_token{reader.get_token()};
                throw ParserException(error_token, this->origin, "Another argument or end of arguments expected.");
            }
        }
        reader.get_token(); // Skip the closing parenthesis
        if (result == -1) {
            throw ParserException(nullptr, this->origin, "Expected ')' but EOF has been reached.");
        }
    }

    std::unique_ptr<FunctionCallArgument> outer_argument{nullptr};

    // Check if there is a block argument at the end
    if (reader.check_limiter('{') == 1) {
        auto value{parse_block()};

        outer_argument = std::make_unique<FunctionCallArgument>(nullptr, std::move(value));
    }

    auto function_call = std::make_unique<FunctionCall>(call_token->get_keyword(), std::move(arguments),
                                                        std::move(outer_argument));

    return function_call;
}

std::unique_ptr<Block> ninx::parser::Parser::parse() {
    auto main_block{parse_implicit_block()};
    return main_block;
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

void ninx::parser::Parser::generate_exception(const std::string &message) {
    auto error_token{reader.get_token()};
    throw ParserException(error_token, this->origin, message);
}


/*
void ninx::parser::Parser::generate() {
Token * current_token;
while ((current_token = reader.get_token())) {
std::cout << *current_token << std::endl;
}
}
*/