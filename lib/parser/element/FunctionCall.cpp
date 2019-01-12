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

#include "FunctionCall.h"
#include "FunctionCallArgument.h"
#include "Block.h"

ninx::parser::element::FunctionCall::FunctionCall(const std::string &name, std::vector<std::unique_ptr<FunctionCallArgument>> arguments,
                                                  std::unique_ptr<FunctionCallArgument> outer_argument)
        : name(name), arguments(std::move(arguments)), outer_argument(std::move(outer_argument)) {}

std::string ninx::parser::element::FunctionCall::dump(int level) const {
    return std::string(level, '\t')+ "FunctionCall: "+this->name;
}

void ninx::parser::element::FunctionCall::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

const std::string &ninx::parser::element::FunctionCall::get_name() const {
    return name;
}

const std::vector<std::unique_ptr<ninx::parser::element::FunctionCallArgument>> &
ninx::parser::element::FunctionCall::get_arguments() const {
    return arguments;
}

const std::unique_ptr<ninx::parser::element::FunctionCallArgument> &
ninx::parser::element::FunctionCall::get_outer_argument() const {
    return outer_argument;
}

int ninx::parser::element::FunctionCall::get_argument_count() const {
    int count {static_cast<int>(arguments.size())};

    // The outer argument is optional, so check if it is present
    if (this->outer_argument) {
        count++;
    }

    return count;
}

void ninx::parser::element::FunctionCall::set_parent(ninx::parser::element::Block *parent) {
    ASTElement::set_parent(parent);

    for (auto &argument : arguments) {
        argument->set_parent(parent);
    }
}
