//
// Created by federico.terzi2 on 08/12/2018.
//

#ifndef NINX_PARSER_H
#define NINX_PARSER_H

#include <string>
#include <vector>
#include <memory>
#include "../lexer/token/Token.h"
#include "exception/ParserException.h"
#include "element/Statement.h"
#include "TokenReader.h"
#include "element/Block.h"
#include "element/FunctionArgument.h"
#include "element/FunctionDefinition.h"
#include "element/FunctionCallArgument.h"
#include "element/FunctionCall.h"

using namespace ninx::parser::element;

namespace ninx {
    namespace parser {
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
        public:
            explicit Parser(std::vector<std::unique_ptr<ninx::lexer::token::Token>> &tokens, const std::string &origin);

            std::unique_ptr<Block> parse();
        };
    }
}


#endif //NINX_PARSER_H
