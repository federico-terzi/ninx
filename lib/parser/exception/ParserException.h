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

#ifndef NINX_PARSEEXCEPTION_H
#define NINX_PARSEEXCEPTION_H

#include <stdexcept>
#include "../../lexer/token/Token.h"

namespace ninx {
    namespace parser {
        namespace exception {
            class ParserException : public std::runtime_error {
            private:
                int line;
                std::string &origin;

                static std::string make_message(int line, const std::string &origin, const std::string &message);
            public:
                explicit ParserException(int line, std::string &origin, const std::string &message);
                explicit ParserException(ninx::lexer::token::Token *token, std::string &origin, const std::string &message);
            };
        }
    }
}

#endif //NINX_PARSEEXCEPTION_H
