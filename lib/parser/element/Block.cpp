#include <utility>
#include <sstream>

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

#include "Block.h"

ninx::parser::element::Block::Block(std::vector<std::unique_ptr<Statement>> statements) : statements(std::move(
        statements)) {
    // Make all the statements parent reference point to this block
    for (auto &statement : this->statements) {
        statement->set_parent(this);
    }
}

std::string ninx::parser::element::Block::dump(int level) const {
    std::stringstream s;

    s << std::string(level, '\t') + "Block {" << std::endl;
    for (auto& statement : statements) {
        s << statement->dump(level+1) << std::endl;
    }
    s << std::string(level, '\t') + "}" << std::endl;

    return s.str();
}

void ninx::parser::element::Block::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

const std::vector<std::unique_ptr<ninx::parser::element::Statement>> &
ninx::parser::element::Block::get_statements() const {
    return statements;
}

ninx::parser::element::Block *ninx::parser::element::Block::get_variable(const std::string &name) const {
    if (variables.find(name) != variables.end()) {
        return this->variables.at(name);
    }

    // Check if the variable is declared in the parent block
    if (this->parent) {
        return this->parent->get_variable(name);
    }

    return nullptr;
}

void ninx::parser::element::Block::set_variable(const std::string &name, ninx::parser::element::Block *value) {
    this->variables[name] = value;
}

ninx::parser::element::FunctionDefinition *ninx::parser::element::Block::get_function(const std::string &name) const {
    if (functions.find(name) != functions.end()) {
        return this->functions.at(name);
    }

    // Check if the function is declared in the parent block
    if (this->parent) {
        return this->parent->get_function(name);
    }

    return nullptr;
}

void ninx::parser::element::Block::set_function(const std::string &name, ninx::parser::element::FunctionDefinition *func) {
    this->functions[name] = func;
}

void ninx::parser::element::Block::clear_variables() {
    this->variables.clear();
}
