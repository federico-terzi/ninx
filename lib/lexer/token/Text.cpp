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

#include "Text.h"
#include <boost/regex.hpp>
#include <boost/algorithm/string/trim.hpp>

ninx::lexer::token::Type ninx::lexer::token::Text::get_type() {
    return Type::TEXT;
}

ninx::lexer::token::Text::Text(int line_number, const std::string &text) : Token(line_number), text(text) {
    // Check if there are multiple spaces
    static const boost::regex multiple_newline_validation("\\s*[\\n]{2,}\\s*");
    if (regex_match(this->text, multiple_newline_validation)) {
        this->text = "\n";
        valid_newline = true;
    }
}

const std::string &ninx::lexer::token::Text::get_text() const {
    return text;
}

std::string ninx::lexer::token::Text::dump() const {
    return "TEXT: '" + this->get_text() + "'";
}

std::unique_ptr<std::string> ninx::lexer::token::Text::get_identifier() {
    // Check if the text is alphanumeric, and do not contain spaces
    static const boost::regex identifierValidation("^\\s*[a-zA-Z][a-zA-Z0-9]*\\s*$");
    if (!regex_match(this->get_text(), identifierValidation)) {
        return nullptr;
    }

    // Trim the text to extract the identifier
    std::string identifier {this->get_text()};
    boost::algorithm::trim(identifier);

    return std::make_unique<std::string>(identifier);
}

bool ninx::lexer::token::Text::is_empty() {
    // Check if the text is empty or contain only spaces
    if (this->text.length() == 0) {
        return true;
    }

    if (valid_newline) {
        return false;
    }

    static const boost::regex emptyValidation("^[\\n\\t\\r]$");
    return regex_match(this->get_text(), emptyValidation);
}
