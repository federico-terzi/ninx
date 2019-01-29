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

#include <vector>
#include <memory>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "Block.h"
#include "FunctionDefinition.h"
#include "FunctionArgument.h"
#include "TextElement.h"

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
    s << std::string(level + 1, '\t') + "echoing " << this->is_echoing() << std::endl;
    for (auto &statement : statements) {
        s << statement->dump(level + 1) << std::endl;
    }
    s << std::string(level, '\t') + "}" << std::endl;

    return s.str();
}

void ninx::parser::element::Block::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

const std::vector<std::unique_ptr<ninx::parser::element::Statement>> &
ninx::parser::element::Block::get_children() const {
    return statements;
}

ninx::parser::element::Block *ninx::parser::element::Block::get_variable(const std::string &name) const {
    return this->get_variable(name, false);
}


ninx::parser::element::Block *
ninx::parser::element::Block::get_variable(const std::string &name, bool only_local) const {
    if (variables.find(name) != variables.end()) {
        return this->variables.at(name).get();
    }

    // Check if the variable is declared in the parent block
    if (this->parent && !only_local) {
        return this->parent->get_variable(name);
    }

    return nullptr;
}

void
ninx::parser::element::Block::set_variable(const std::string &name, std::unique_ptr<Block> value, bool force_local) {
    if (force_local) {
        value->set_parent(this);
        this->variables[name] = std::move(value);
        return;
    }

    // Navigate the parent structure to determine if the variable is already defined in an outer block.
    // If so, the assignment will take effect on that entity.
    Block *current_block{this};
    while (current_block != nullptr) {
        if (current_block->get_variable(name, true) != nullptr) {
            break;
        } else {
            current_block = current_block->parent;
        }
    }

    if (current_block == nullptr) {
        current_block = this;
    }

    current_block->set_variable(name, std::move(value), true);
}

ninx::parser::element::FunctionDefinition *ninx::parser::element::Block::get_function(const std::string &name) const {
    if (functions.find(name) != functions.end()) {
        return this->functions.at(name).get();
    }

    // Check if the function is declared in the parent block
    if (this->parent) {
        return this->parent->get_function(name);
    }

    return nullptr;
}

void ninx::parser::element::Block::set_function(const std::string &name,
                                                std::unique_ptr<ninx::parser::element::FunctionDefinition> func) {
    this->functions[name] = std::move(func);
}

void ninx::parser::element::Block::clear_variables() {
    this->variables.clear();
}

ninx::parser::element::Block *ninx::parser::element::Block::clone_impl() {
    // Clone all the statements
    std::vector<std::unique_ptr<Statement>> statements_copy;
    for (auto &statement : this->statements) {
        auto statement_copy{statement->clone<Statement>()};
        statements_copy.push_back(std::move(statement_copy));
    }

    Block *obj{new Block(std::move(statements_copy))};

    // Copy function references
    std::unordered_map<std::string, std::unique_ptr<FunctionDefinition>> functions_copy;
    for (auto &v: this->functions) {
        auto function_copy{v.second->clone<FunctionDefinition>()};
        function_copy->set_parent(obj);  // Make the new object the new parent
        functions_copy[v.first] = std::move(function_copy);
    }
    obj->functions = std::move(functions_copy);

    // Copy variables
    std::unordered_map<std::string, std::unique_ptr<Block>> variables_copy;
    for (auto &v: this->variables) {
        auto variable_copy{v.second->clone<Block>()};
        variable_copy->set_parent(obj);  // Make the new object the new parent
        variables_copy[v.first] = std::move(variable_copy);
    }
    obj->variables = std::move(variables_copy);

    // Copy late call internals
    obj->__set_output_block(this->__output_block);
    obj->__output_segments = this->__output_segments;
    obj->__current_output_segment_position = this->__current_output_segment_position;
    std::map<size_t, std::unique_ptr<ninx::parser::util::LateCallDescriptor>> late_calls_copy;
    for (auto &l: this->__late_calls) {
        auto late_call_copy{l.second->clone()};
        late_calls_copy[l.first] = std::move(late_call_copy);
    }
    obj->__late_calls = std::move(late_calls_copy);

    return obj;
}

std::unique_ptr<ninx::parser::element::Block>
ninx::parser::element::Block::make_text_block(Block *parent, const std::string &text) {
    auto element{std::make_unique<TextElement>(text)};
    std::vector<std::unique_ptr<Statement>> block_statements;
    block_statements.push_back(std::move(element));

    auto block{std::make_unique<Block>(std::move(block_statements))};
    block->set_parent(parent);

    return block;
}

std::unique_ptr<ninx::parser::element::Block>
ninx::parser::element::Block::make_empty(ninx::parser::element::Block *parent) {
    std::vector<std::unique_ptr<Statement>> block_statements;
    auto block{std::make_unique<Block>(std::move(block_statements))};
    block->set_parent(parent);

    return block;
}

bool ninx::parser::element::Block::is_echoing() const {
    return echoing;
}

void ninx::parser::element::Block::set_echoing(bool echoing) {
    this->echoing = echoing;
}

int ninx::parser::element::Block::get_children_count() const {
    return static_cast<int>(statements.size());
}

void ninx::parser::element::Block::add_child(std::unique_ptr<ninx::parser::element::Statement> statement) {
    statements.push_back(std::move(statement));
}

// BUILT-IN METHODS

std::unique_ptr<ninx::parser::element::FunctionDefinition> build_size_definition() {
    std::vector<std::unique_ptr<ninx::parser::element::FunctionArgument>> arguments;
    auto body{ninx::parser::element::Block::make_empty(nullptr)};
    auto definition = std::make_unique<ninx::parser::element::FunctionDefinition>("size", std::move(arguments),
                                                                                  std::move(body));
    definition->set_evaluator(
            [](ninx::parser::element::Block *target, std::unique_ptr<ninx::parser::element::Block> args) {
                return ninx::parser::element::Block::make_text_block(target, boost::lexical_cast<std::string>(
                        target->get_children_count()));
            });
    return std::move(definition);
}

std::unique_ptr<ninx::parser::element::FunctionDefinition> build_add_definition() {
    std::vector<std::unique_ptr<ninx::parser::element::FunctionArgument>> arguments;
    auto argument{std::make_unique<ninx::parser::element::FunctionArgument>("item", nullptr)};
    arguments.push_back(std::move(argument));
    auto body{ninx::parser::element::Block::make_empty(nullptr)};
    auto definition = std::make_unique<ninx::parser::element::FunctionDefinition>("add", std::move(arguments),
                                                                                  std::move(body));
    definition->set_evaluator(
            [](ninx::parser::element::Block *target, std::unique_ptr<ninx::parser::element::Block> args) {
                target->add_child(args->get_variable("item")->clone<ninx::parser::element::Statement>());
                return nullptr;
            });
    return std::move(definition);
}

std::unordered_map<std::string, std::unique_ptr<ninx::parser::element::FunctionDefinition>>
initialize_builtin_methods() {
    std::unordered_map<std::string, std::unique_ptr<ninx::parser::element::FunctionDefinition>> methods;
    methods["size"] = build_size_definition();
    methods["add"] = build_add_definition();
    return std::move(methods);
}

std::unordered_map<std::string, std::unique_ptr<ninx::parser::element::FunctionDefinition>> ninx::parser::element::Block::builtin_functions = initialize_builtin_methods();

ninx::parser::element::FunctionDefinition *ninx::parser::element::Block::get_builtin(const std::string &name) const {
    if (builtin_functions.find(name) != builtin_functions.end()) {
        return builtin_functions.at(name).get();
    }

    return nullptr;
}

std::string ninx::parser::element::Block::__render_output() {
    std::stringstream output;
    for (auto &segment : __output_segments) {
        output << segment;
    }
    __output_segments.clear();
    return std::move(output.str());
}

size_t ninx::parser::element::Block::__add_late_call(std::unique_ptr<ninx::parser::util::LateCallDescriptor> descriptor) {
    size_t segment_position = __output_segments.size();

    // Add an empty string
    __output_segments.push_back(std::move(std::string()));

    // Add the late call reference
    __late_calls[segment_position] = std::move(descriptor);

    return segment_position;
}

size_t ninx::parser::element::Block::__add_output_segment(const std::string &text) {
    size_t segment_position = __output_segments.size();

    // Check whether to add the segment to the bottom or to a previous segment ( the latter used for late calls )
    if (__current_output_segment_position < 0) {
        __output_segments.push_back(text);
    }else{
        __output_segments[__current_output_segment_position] = text;
    }

    return segment_position;
}

const std::map<size_t, std::unique_ptr<ninx::parser::util::LateCallDescriptor>> &
ninx::parser::element::Block::__get_late_calls() const {
    return __late_calls;
}

void ninx::parser::element::Block::__set_output_segment_position(int position) {
    __current_output_segment_position = position;
}


