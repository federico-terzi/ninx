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

#include "VariableRead.h"

std::string ninx::parser::element::VariableRead::dump(int level) const {
    return std::string(level, '\t')+"VariableRead: "+name;
}

const std::string &ninx::parser::element::VariableRead::get_name() const {
    return name;
}

void ninx::parser::element::VariableRead::set_name(const std::string &name) {
    VariableRead::name = name;
}

ninx::parser::element::VariableRead::VariableRead(const std::string &name, int suffix_spaces)
        : name(name), trailing_spaces(suffix_spaces) {}

void ninx::parser::element::VariableRead::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

ninx::parser::element::VariableRead * ninx::parser::element::VariableRead::clone_impl() {
    return new VariableRead(this->name, 0);
}

int ninx::parser::element::VariableRead::get_trailing_spaces() const {
    return trailing_spaces;
}
