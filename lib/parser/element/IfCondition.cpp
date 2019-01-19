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
#include "IfCondition.h"

ninx::parser::element::IfCondition::IfCondition(std::vector<std::unique_ptr<ninx::parser::element::IfCase>> cases)
: cases(std::move(cases)){}

std::string ninx::parser::element::IfCondition::dump(int level) const {
    std::stringstream s;

    s << std::string(level, '\t') + "IfCondition {" << std::endl;
    for (auto& c : cases) {
        s << c->dump(level+1) << std::endl;
    }
    s << std::string(level, '\t') + "}" << std::endl;

    return s.str();
}

void ninx::parser::element::IfCondition::set_parent(ninx::parser::element::Block *parent) {
    ASTElement::set_parent(parent);

    for (auto &c : cases) {
        c->set_parent(parent);
    }
}

void ninx::parser::element::IfCondition::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

ninx::parser::element::IfCondition *ninx::parser::element::IfCondition::clone_impl() {
    // Clone all the cases
    std::vector<std::unique_ptr<IfCase>> cases_copy;
    for (auto &c : cases) {
        auto case_copy {c->clone<IfCase>()};
        cases_copy.push_back(std::move(case_copy));
    }

    return new IfCondition(std::move(cases_copy));
}

const std::vector<std::unique_ptr<ninx::parser::element::IfCase>> &
ninx::parser::element::IfCondition::get_cases() const {
    return cases;
}
