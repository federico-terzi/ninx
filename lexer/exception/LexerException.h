//
// Created by federico.terzi2 on 07/12/2018.
//

#ifndef NINX_PARSEEXCEPTION_H
#define NINX_PARSEEXCEPTION_H

#include <stdexcept>

namespace ninx {
    namespace lexer {
        namespace exception {
            class LexerException : public std::runtime_error {
            private:
                int line;
                std::string &origin;

                static std::string make_message(int line, const std::string &origin, const std::string &message);
            public:
                explicit LexerException(int line, std::string &origin, const std::string &message);
            };
        }
    }
}

#endif //NINX_PARSEEXCEPTION_H
