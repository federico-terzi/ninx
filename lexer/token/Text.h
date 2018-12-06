//
// Created by Federico on 06-Dec-18.
//

#ifndef NINX_TEXT_H
#define NINX_TEXT_H

#include "Token.h"

namespace ninx {
    namespace lexer {
        namespace token {
            class Text : public Token {
            private:
                std::string text;
            public:
                explicit Text(const std::string &text);

                Type get_type() override;

                std::string dump() const override;

                const std::string &get_text() const;
            };
        }
    }
}



#endif //NINX_TEXT_H
