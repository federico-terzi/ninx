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
#include "IfCase.h"

ninx::parser::element::IfCase::IfCase(std::unique_ptr<ninx::parser::element::Expression> condition,
                                      std::unique_ptr<ninx::parser::element::Block> body)
                                      : condition(std::move(condition)), body(std::move(body)) {}

std::string ninx::parser::element::IfCase::dump(int level) const {
    std::stringstream s;

    s << std::string(level, '\t') + "IfCase " << this->condition->dump(level+1) << " {" << std::endl;
    s << body->dump(level+1) << std::endl;
    s << std::string(level, '\t') + "}" << std::endl;

    return s.str();
}

void ninx::parser::element::IfCase::set_parent(ninx::parser::element::Block *parent) {
    ASTElement::set_parent(parent);

    condition->set_parent(parent);
    body->set_parent(parent);
}

void ninx::parser::element::IfCase::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

ninx::parser::element::IfCase *ninx::parser::element::IfCase::clone_impl() {
    return new IfCase(this->condition->clone<Expression>(), this->body->clone<Block>());
}

const std::unique_ptr<ninx::parser::element::Expression> &ninx::parser::element::IfCase::get_condition() const {
    return condition;
}

const std::unique_ptr<ninx::parser::element::Block> &ninx::parser::element::IfCase::get_body() const {
    return body;
}
