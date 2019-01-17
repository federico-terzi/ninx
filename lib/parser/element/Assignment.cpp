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

#include "Assignment.h"


ninx::parser::element::Assignment::Assignment(const std::string &name,
                                              std::unique_ptr<ninx::parser::element::Expression> value)
                                              : name(name), value(std::move(value)) {

    // Assignment blocks are not echoing
    if (value) {
        this->value->set_echoing(false);
    }
}

ninx::parser::element::Expression *ninx::parser::element::Assignment::get_value() {
    return this->value.get();
}

std::string ninx::parser::element::Assignment::dump(int level) const {
    return std::string(level, '\t')+"Assignment: "+name+" = "+value->dump(level+1);
}

void ninx::parser::element::Assignment::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

const std::string &ninx::parser::element::Assignment::get_name() const {
    return name;
}

void ninx::parser::element::Assignment::set_parent(ninx::parser::element::Block *parent) {
    ASTElement::set_parent(parent);

    // Also set the value value parent to the given one, to enable it to access outer scope variables
    this->value->set_parent(parent);
}

ninx::parser::element::ASTElement *ninx::parser::element::Assignment::clone_impl() {
    return new Assignment(this->name, this->value->clone<Expression>());
}
