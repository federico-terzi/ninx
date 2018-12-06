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

#include "Lexer.h"
#include "token/TextToken.h"

using namespace ninx::lexer::token;

ninx::lexer::Lexer::Lexer(std::istream &stream) : Lexer(stream, "unknown_origin") {}

ninx::lexer::Lexer::Lexer(std::istream &stream, std::string origin) : stream{stream}, origin{std::move(origin)},
                                                                      reader{Reader{stream, origin}} {

    while(true) {
        int next_limiter = reader.get_next_limiter();
        if (next_limiter == -1) {  // EOF
            break;
        }

        if (next_limiter > 0) {  // A limiter was found
            switch (next_limiter) {
                case '@':  // Keyword beginning
                    std::cout << "Keyword found: '" << reader.read_identifier() << "'" << std::endl;
                    break;
                default:   // Other limiters, mark them as generic limiters
                    std::cout << "Limiter found: '" << static_cast<char>(next_limiter) << "'" << std::endl;
            }
        }else{  // A limiter was not found, it is a simple text
            std::cout << "Text found: '" << reader.read_until_limiter() << "'" << std::endl;
        }
    }
}