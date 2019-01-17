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

#include "BinaryExpression.h"

ninx::parser::element::BinaryExpression::BinaryExpression(std::unique_ptr<ninx::parser::element::Expression> first,
                                                    std::unique_ptr<ninx::parser::element::Expression> second)
        : first(std::move(first)), second(std::move(second)) {


}

std::string ninx::parser::element::BinaryExpression::dump(int level) const {
    return std::string(level, '\t')+ get_dump_name() + ": "+first->dump(level)+" + "+second->dump(level);
}

ninx::parser::element::Expression * ninx::parser::element::BinaryExpression::get_first() const {
    return first.get();
}

ninx::parser::element::Expression * ninx::parser::element::BinaryExpression::get_second() const {
    return second.get();
}

void ninx::parser::element::BinaryExpression::set_parent(ninx::parser::element::Block *parent) {
    ASTElement::set_parent(parent);

    this->first->set_parent(parent);
    this->second->set_parent(parent);
}
