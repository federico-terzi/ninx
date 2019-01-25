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

#ifndef NINX_KEYWORD_H
#define NINX_KEYWORD_H

#include "Token.h"
#include <map>

namespace ninx {
    namespace lexer {
        namespace token {
            // Reserved keywords, associated with their Type
            const std::map<std::string, Type> RESERVED_KEYWORDS = {{"func",     Type::FUNCDEF},
                                                                   {"operator", Type::OPDEF},
                                                                   {"if",       Type::IF},
                                                                   {"elseif",   Type::ELSEIF},
                                                                   {"else",     Type::ELSE},
                                                                   {"for",     Type::FOR}
            };

            class Keyword : public Token {
            private:
                std::string keyword;
                Type type;

            public:
                explicit Keyword(int line_number, const std::string &keyword);

                Type get_type() override;

                std::string dump() const override;

                const std::string &get_keyword() const;
            };
        }
    }
}


#endif //NINX_KEYWORD_H
