//
// Created by Federico on 06-Dec-18.
//

#include "Text.h"

ninx::lexer::token::Type ninx::lexer::token::Text::get_type() {
    return Type::TEXT;
}

ninx::lexer::token::Text::Text(const std::string &text) : text(text) {}

const std::string &ninx::lexer::token::Text::get_text() const {
    return text;
}

std::string ninx::lexer::token::Text::dump() const {
    return "TEXT: '" + this->get_text() + "'";
}
