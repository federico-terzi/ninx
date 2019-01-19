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

#include "TokenReader.h"
#include "../lexer/token/Limiter.h"

ninx::parser::TokenReader::TokenReader(std::vector<std::unique_ptr<Token>> &tokens) : tokens(tokens) {}

Token *ninx::parser::TokenReader::get_token() {
    if (current_token < tokens.size()) {
        Token * token = tokens[current_token].get();
        current_token++;
        return token;
    }

    return nullptr;
}

Token *ninx::parser::TokenReader::peek_token() {
    if (current_token < tokens.size()) {
        Token * token = tokens[current_token].get();
        return token;
    }

    return nullptr;
}

void ninx::parser::TokenReader::seek_previous() {
    if (current_token > 0) {
        current_token--;
    }
}

int ninx::parser::TokenReader::check_limiter(char limiter) {
    auto token = peek_token();
    if (!token) {
        return -1;
    }

    if (token->get_type() != Type::LIMITER) {
        return 0;
    }

    if (dynamic_cast<ninx::lexer::token::Limiter*>(token)->get_limiter() == limiter) {
        return 1;
    }

    return 0;
}

int ninx::parser::TokenReader::check_limiter_sequence(const std::string &sequence) {
    int result = 1;

    int count = 0;

    for(char c : sequence) {
        auto token = get_token();
        count++;

        if (!token) {
            result = -1;
            break;
        }

        if (token->get_type() != Type::LIMITER) {
            result = 0;
            break;
        }

        if (dynamic_cast<ninx::lexer::token::Limiter*>(token)->get_limiter() != c) {
            result = 0;
            break;
        }
    }

    // Rewind
    for (int i = 0; i<count; i++) {
        seek_previous();
    }

    return result;
}

int ninx::parser::TokenReader::check_type(Type type) {
    auto token = peek_token();
    if (!token) {
        return -1;
    }

    return token->get_type() == type;
}
