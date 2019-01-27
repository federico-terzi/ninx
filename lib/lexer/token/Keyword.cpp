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

#include <sstream>
#include "Keyword.h"

ninx::lexer::token::Keyword::Keyword(int line_number, const std::string &keyword) : Token(line_number), keyword(keyword) {
    if (RESERVED_KEYWORDS.count(keyword) > 0) {  // The given keyword is a reserved one, get the corresponding Type
        this->type = RESERVED_KEYWORDS.at(keyword);
    }else{
        this->type = Type::FUNCNAME;  // If a keyword is not reserved, then it is a function call
    }
}

ninx::lexer::token::Type ninx::lexer::token::Keyword::get_type() {
    return this->type;
}

std::string ninx::lexer::token::Keyword::dump() const {
    std::stringstream s;
    s << "KEYWORD ( type " << static_cast<int>(this-> type) << " late:" << this->late << " ): '" << this->keyword << "'";
    return s.str();
}

const std::string &ninx::lexer::token::Keyword::get_keyword() const {
    return keyword;
}

bool ninx::lexer::token::Keyword::is_late() const {
    return late;
}

void ninx::lexer::token::Keyword::set_late(bool late) {
    Keyword::late = late;
}
