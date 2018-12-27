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

#ifndef NINX_TOKENREADER_H
#define NINX_TOKENREADER_H

#include <vector>
#include <memory>
#include "../lexer/token/Token.h"

using namespace ninx::lexer::token;

namespace ninx {
    namespace parser {
        class TokenReader {
        private:
            std::vector<std::unique_ptr<Token>> &tokens;

            int current_token = 0;
        public:
            explicit TokenReader(std::vector<std::unique_ptr<Token>> &tokens);

            Token * get_token();
            Token * peek_token();

            /*
             * Get the next token and verify that is a limiter equal to the given char.
             */
            bool check_limiter(char limiter);

            void seek_previous();
        };
    }
}
#endif //NINX_TOKENREADER_H
