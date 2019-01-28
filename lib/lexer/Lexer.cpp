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

ninx::lexer::Lexer::Lexer(std::istream &stream, std::string origin) : stream{stream}, origin{origin},
                                                                      reader{Reader{stream, origin}} {
}

std::vector<std::unique_ptr<Token>> ninx::lexer::Lexer::generate() {
    std::vector<std::unique_ptr<Token>> tokens;

    while(true) {
        int next_limiter = reader.get_next_limiter();
        if (next_limiter == -1) {  // EOF
            break;
        }

        if (next_limiter > 0) {  // A limiter was found
            switch (next_limiter) {
                case '@':  // Keyword beginning
                {
                    bool lateSelectorFound = false;
                    int trailing_spaces;
                    auto keywordToken = std::make_unique<Keyword>(reader.get_line_number(), reader.read_identifier(trailing_spaces, '?', lateSelectorFound));

                    if (lateSelectorFound) {
                        keywordToken->set_late(true);
                    }

                    tokens.push_back(std::move(keywordToken));
                    break;
                }
                case '$':  // Variable beginning
                {
                    int trailing_spaces;
                    auto variableToken = std::make_unique<Variable>(reader.get_line_number(),
                                                                    reader.read_identifier(trailing_spaces));
                    variableToken->set_trailing_spaces(trailing_spaces);
                    tokens.push_back(std::move(variableToken));
                    break;
                }
                default:   // Other limiters, mark them as generic limiters
                {
                    auto limiterToken = std::make_unique<Limiter>(reader.get_line_number(), static_cast<char>(next_limiter));
                    tokens.push_back(std::move(limiterToken));
                }
            }
        }else{  // A limiter was not found, it is a simple text
            auto token = std::make_unique<Text>(reader.get_line_number(), reader.read_until_limiter());
            // Only add non-empty tokens
            if (!token->is_empty()) {
                tokens.push_back(std::move(token));
            }
        }
    }

    if (verbose) {
        for (auto &token : tokens) {
            std::cout << *token << std::endl;
        }
    }

    return std::move(tokens);
}

bool ninx::lexer::Lexer::is_verbose() const {
    return verbose;
}

void ninx::lexer::Lexer::set_verbose(bool verbose) {
    Lexer::verbose = verbose;
}
