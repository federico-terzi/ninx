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

#ifndef NINX_TEXT_H
#define NINX_TEXT_H

#include <memory>
#include "Token.h"

namespace ninx {
    namespace lexer {
        namespace token {
            class Text : public Token {
            private:
                std::string text;
            public:
                explicit Text(int line_number, const std::string &text);

                Type get_type() override;

                std::string dump() const override;

                const std::string &get_text() const;

                /**
                 * Try to convert the current Text token into an identifier ( only alphanumeric characters,
                 * not beginning with a number ) removing trailing and ending spaces.
                 * If cannot be converted ( contain spaces in between or contain non-alphanumeric chars )
                 * return nullptr.
                 * @return the text converted to a valid identifier, or nullptr if not valid
                 */
                std::unique_ptr<std::string> get_identifier();

                /**
                 * Check if the text token is only made of text characters.
                 */
                bool is_empty();
            };
        }
    }
}



#endif //NINX_TEXT_H
