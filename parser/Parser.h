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

using namespace ninx::parser::element;

namespace ninx {
    namespace parser {
        class Parser {
        private:
            std::string origin;
            TokenReader reader;

            std::unique_ptr<Statement> parse_statement();
            std::unique_ptr<Block> parse_function_call();
        public:
            explicit Parser(std::vector<std::unique_ptr<ninx::lexer::token::Token>> &tokens, const std::string &origin);

            void parse();
        };
    }
}


#endif //NINX_PARSER_H
