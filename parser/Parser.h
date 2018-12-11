//
// Created by federico.terzi2 on 08/12/2018.
//

#ifndef NINX_PARSER_H
#define NINX_PARSER_H

#include <string>
#include <vector>
#include <memory>
#include "../lexer/token/Token.h"
#include "TokenReader.h"

using namespace ninx::lexer::token;

namespace ninx {
    namespace parser {
        class Parser {
        private:
            std::string origin;
            TokenReader reader;

        public:
            explicit Parser(std::vector<std::unique_ptr<Token>> &tokens, const std::string &origin);

            void parse();
        };
    }
}


#endif //NINX_PARSER_H
