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

#ifndef NINX_PARSER_H
#define NINX_PARSER_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "../lexer/token/Token.h"
#include "exception/ParserException.h"
#include "element/Statement.h"
#include "TokenReader.h"
#include "element/Block.h"
#include "element/FunctionArgument.h"
#include "element/FunctionDefinition.h"
#include "element/FunctionCallArgument.h"
#include "element/FunctionCall.h"
#include "parser/element/Expression.h"

using namespace ninx::parser::element;

namespace ninx {
    namespace parser {
        struct OperatorCaseDefinition {
            std::string op;
            std::function<std::unique_ptr<Expression>(std::unique_ptr<Expression>, std::unique_ptr<Expression>)> builder;
        };

        class Parser {
        private:
            std::string origin;
            TokenReader reader;

            std::unique_ptr<Statement> parse_statement();
            std::unique_ptr<Block> parse_implicit_block();
            std::unique_ptr<Block> parse_block();
            std::unique_ptr<FunctionCall> parse_function_call();
            std::unique_ptr<FunctionCallArgument> parse_function_call_argument();
            std::unique_ptr<FunctionArgument> parse_function_argument();
            std::unique_ptr<FunctionDefinition> parse_function_definition();

            // Expression parsing
            std::unique_ptr<Expression> parse_value();
            std::unique_ptr<Expression> parse_sub_expression(std::function<std::unique_ptr<Expression>()> term_parser,
                    std::vector<OperatorCaseDefinition> operators);
            std::unique_ptr<Expression> parse_level_1_expression();
            std::unique_ptr<Expression> parse_level_2_expression();
            std::unique_ptr<Expression> parse_level_3_expression();
            std::unique_ptr<Expression> parse_expression();


        public:
            explicit Parser(std::vector<std::unique_ptr<ninx::lexer::token::Token>> &tokens, const std::string &origin);

            std::unique_ptr<Block> parse();
        };
    }
}


#endif //NINX_PARSER_H
