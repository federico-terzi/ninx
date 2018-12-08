//
// Created by federico.terzi2 on 08/12/2018.
//

#ifndef NINX_PARSER_H
#define NINX_PARSER_H

#include <string>
#include <vector>
#include <memory>
#include "../lexer/token/Token.h"

using namespace ninx::lexer::token;

namespace ninx {
    namespace parser {
        class Parser {
        private:
            std::string origin;
            std::vector<std::unique_ptr<Token>> &tokens;

        public:
            explicit Parser(std::vector<std::unique_ptr<Token>> &tokens, const std::string &origin);
        };
    }
}


#endif //NINX_PARSER_H
