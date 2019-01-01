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

#include <iostream>
#include <string>
#include "DefaultEvaluator.h"
#include "../parser/element/TextElement.h"
#include "../parser/element/Block.h"
#include "../parser/element/Assignment.h"
#include "../parser/element/VariableRead.h"
#include "exception/VariableNotFoundException.h"

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::TextElement *e) {
    this->output << e->get_text();
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::Assignment *e) {
    e->get_parent()->set_variable(e->get_name(), e->get_block());
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::Block *e) {
    for (auto &statement : e->get_statements()) {
        statement->accept(this);
    }
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionCall *e) {

}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::VariableRead *e) {
    auto variable {e->get_parent()->get_variable(e->get_name())};

    if (!variable) {
        // TODO: add information of line number and origin
        throw ninx::evaluator::exception::VariableNotFoundException(0, "TODO", "Variable \""+e->get_name()+"\" has not been declared!");
    }

    variable->accept(this);
}

ninx::evaluator::DefaultEvaluator::DefaultEvaluator(std::ostream &output) : output(output) {}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionDefinition *e) {

}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionArgument *e) {

}
