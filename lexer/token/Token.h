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


#ifndef NINX_BASETOKEN_H
#define NINX_BASETOKEN_H

#include <iostream>
#include "../Reader.h"
#include "Type.h"

namespace ninx {
    namespace lexer {
        namespace token {
            class Token {
            protected:
                int line_number = 0;
            public:
                explicit Token(int line_number) : line_number(line_number) {}
                virtual ~Token() = default;

                virtual Type get_type() = 0;

                virtual std::string dump() const = 0;
                friend std::ostream& operator<<(std::ostream &strm, const Token &a) {
                    return strm << a.dump();
                }

                int get_line_number() const {
                    return line_number;
                }
            };
        }
    }
}

#endif //NINX_BASETOKEN_H
