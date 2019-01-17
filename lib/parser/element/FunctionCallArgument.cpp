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

#include <memory>
#include "FunctionCallArgument.h"
#include "Block.h"

ninx::parser::element::FunctionCallArgument::FunctionCallArgument(std::unique_ptr<std::string> name,
                                                                  std::unique_ptr<Expression> value)
        : name(std::move(name)), value(std::move(value)) {

    // Arguments are not echoing
    if (this->value) {
        this->value->set_echoing(false);
    }
}

void ninx::parser::element::FunctionCallArgument::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

std::string ninx::parser::element::FunctionCallArgument::dump(int level) const {
    std::string _name {"<implicit>"};
    if (this->name) {
        _name = *(this->name);
    }
    return std::string(level, '\t')+ "FunctionCallArgument: "+_name + " = "+ this->value->dump(level+1);
}

const std::unique_ptr<std::string> &ninx::parser::element::FunctionCallArgument::get_name() const {
    return name;
}

const std::unique_ptr<ninx::parser::element::Expression> &ninx::parser::element::FunctionCallArgument::get_value() const {
    return value;
}

void ninx::parser::element::FunctionCallArgument::set_parent(ninx::parser::element::Block *parent) {
    ASTElement::set_parent(parent);

    this->value->set_parent(parent);
}

ninx::parser::element::FunctionCallArgument *ninx::parser::element::FunctionCallArgument::clone_impl() {
    std::unique_ptr<std::string> name_copy {nullptr};
    if (this->name) {
        name_copy = std::make_unique<std::string>(*(this->name));
    }

    return new FunctionCallArgument(std::move(name_copy), this->value->clone<Expression>());
}
