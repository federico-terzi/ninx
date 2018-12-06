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
#include "token/Text.h"
#include "token/Limiter.h"
#include "token/Keyword.h"
#include "token/Variable.h"

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
                {
                    auto keywordToken = std::make_unique<Keyword>(reader.read_identifier());
                    this->tokens.push_back(std::move(keywordToken));
                    break;
                }
                case '$':  // Variable beginning
                {
                    auto variableToken = std::make_unique<Variable>(reader.read_identifier());
                    this->tokens.push_back(std::move(variableToken));
                    break;
                }
                default:   // Other limiters, mark them as generic limiters
                {
                    auto limiterToken = std::make_unique<Limiter>(static_cast<char>(next_limiter));
                    this->tokens.push_back(std::move(limiterToken));
                }
            }
        }else{  // A limiter was not found, it is a simple text
            auto token = std::make_unique<Text>(reader.read_until_limiter());
            this->tokens.push_back(std::move(token));
        }
    }

    for (auto& token : this->tokens ) {
        std::cout << *token << std::endl;
    }
}