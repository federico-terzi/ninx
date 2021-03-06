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
#include "FunctionDefinition.h"
#include "Block.h"

ninx::parser::element::FunctionDefinition::FunctionDefinition(const std::string &name,
                                                              std::vector<std::unique_ptr<ninx::parser::element::FunctionArgument>> arguments,
                                                              std::unique_ptr<ninx::parser::element::Block> body)
        : name(name), arguments(std::move(arguments)), body(std::move(body)) {

    // Populate the caches
    for (auto &argument : this->arguments) {
        this->_argument_name_cache.insert(argument->get_name());

        if (!argument->get_default_value()) {
            this->mandatory_arguments.insert(argument->get_name());
        }
    }
}

std::string ninx::parser::element::FunctionDefinition::dump(int level) const {
    std::stringstream s;

    s << std::string(level, '\t') + "FunctionDefinition " << this->name << " {" << std::endl;
    for (auto& argument : arguments) {
        s << argument->dump(level+1) << std::endl;
    }
    s << body->dump(level+1) << std::endl;
    s << std::string(level, '\t') + "}" << std::endl;

    return s.str();
}

void ninx::parser::element::FunctionDefinition::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

const std::string &ninx::parser::element::FunctionDefinition::get_name() const {
    return name;
}

const std::vector<std::unique_ptr<ninx::parser::element::FunctionArgument>> &
ninx::parser::element::FunctionDefinition::get_arguments() const {
    return arguments;
}

const std::unique_ptr<ninx::parser::element::Block> &ninx::parser::element::FunctionDefinition::get_body() const {
    return body;
}

void ninx::parser::element::FunctionDefinition::set_parent(ninx::parser::element::Block *parent) {
    ASTElement::set_parent(parent);

    // Also set the body block parent to the given one, to enable it to access outer scope variables
    this->body->set_parent(parent);
}

bool ninx::parser::element::FunctionDefinition::check_argument(const std::string &name) {
    return this->_argument_name_cache.find(name) != this->_argument_name_cache.end();
}

bool ninx::parser::element::FunctionDefinition::check_mandatory(const std::string &name) {
    return this->mandatory_arguments.find(name) != this->mandatory_arguments.end();
}

const std::unordered_set<std::string> &ninx::parser::element::FunctionDefinition::get_mandatory_arguments() const {
    return mandatory_arguments;
}

ninx::parser::element::FunctionDefinition *ninx::parser::element::FunctionDefinition::clone_impl() {
    // Clone all the arguments
    std::vector<std::unique_ptr<FunctionArgument>> arguments_copy;
    for (auto &argument : arguments) {
        auto argument_copy {argument->clone<FunctionArgument>()};
        arguments_copy.push_back(std::move(argument_copy));
    }

    return new FunctionDefinition(this->name, std::move(arguments_copy), this->body->clone<Block>());
}

std::unique_ptr<ninx::parser::element::Block>
ninx::parser::element::FunctionDefinition::evaluate(ninx::parser::element::Block *target,
                                                    std::unique_ptr<ninx::parser::element::Block> body) {
    ninx::parser::element::Block * __output_block {nullptr};
    if (body) {
        __output_block = body->__get_output_block();
    }

    auto result_block {evaluator(target, std::move(body))};
    if (result_block) {
        result_block->__set_output_block(__output_block);
    }
    return result_block;
}

void ninx::parser::element::FunctionDefinition::set_evaluator(
        std::function<std::unique_ptr<ninx::parser::element::Block>(ninx::parser::element::Block *,
                                                                    std::unique_ptr<ninx::parser::element::Block>)> evaluator) {
    this->evaluator = evaluator;
}
