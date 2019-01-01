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

#include "FunctionArgument.h"
#include "Block.h"

std::string ninx::parser::element::FunctionArgument::dump(int level) const {
    std::string value {"null"};
    if (this->default_value) {
        value = this->default_value->dump(level+1);
    }
    return std::string(level, '\t')+ "FunctionArgument: "+this->name + " = "+ value;
}

void ninx::parser::element::FunctionArgument::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

ninx::parser::element::FunctionArgument::FunctionArgument(const std::string &name,
                                                          std::unique_ptr<ninx::parser::element::Block> default_value)
        : name(name), default_value(std::move(default_value)) {}

const std::string &ninx::parser::element::FunctionArgument::get_name() const {
    return name;
}

const std::unique_ptr<ninx::parser::element::Block> &ninx::parser::element::FunctionArgument::get_default_value() const {
    return default_value;
}