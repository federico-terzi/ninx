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
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <set>
#include <limits>
#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/lexical_cast/bad_lexical_cast.hpp>
#include "DefaultEvaluator.h"
#include "../parser/element/TextElement.h"
#include "../parser/element/Block.h"
#include "../parser/element/Assignment.h"
#include "../parser/element/VariableRead.h"
#include "../parser/element/FunctionDefinition.h"
#include "../parser/element/FunctionCall.h"
#include "../parser/element/FunctionCallArgument.h"
#include "../parser/element/IfCase.h"
#include "../parser/element/IfCondition.h"
#include "parser/element/expression/BinaryExpression.h"
#include "parser/element/expression/AddExpression.h"
#include "parser/element/expression/MultiplicationExpression.h"
#include "parser/element/expression/DivisionExpression.h"
#include "parser/element/expression/SubtractExpression.h"
#include "parser/element/expression/EqualExpression.h"
#include "parser/element/expression/NotEqualExpression.h"
#include "exception/RuntimeException.h"

ninx::evaluator::DefaultEvaluator::DefaultEvaluator(std::ostream &output) : output(output) {}

// RETURN BLOCK MECHANISM

ninx::parser::element::Block * current_return_block;  // This is used to obtain the result block from an expression

std::unique_ptr<ninx::parser::element::Block> get_owned_return_block() {
    std::unique_ptr<ninx::parser::element::Block> new_obj(current_return_block);
    // Reset the current return block
    current_return_block = nullptr;

    return new_obj;
};

void reset_return_block() {
    // Free up the memory if a previous block was used
    delete current_return_block;
    current_return_block = nullptr;
}

void replace_return_block(std::unique_ptr<ninx::parser::element::Block> new_block) {
    reset_return_block();

    current_return_block = new_block.release();
}



// ECHOING MECHANISM

bool is_echoing_disabled = false;

bool disable_echoing() {
    bool changed = false;

    if (!is_echoing_disabled) {
        is_echoing_disabled = true;
        changed = true;
    }

    return changed;
}

void enable_echoing(bool changed) {
    if (changed) {
        is_echoing_disabled = false;
    }
}

void no_echo(const std::function<void()> &block) {
    bool changed = disable_echoing();

    block();

    enable_echoing(changed);
}



// FUNCTION RELATED VISITING

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionCall *call) {
    auto function {call->get_parent()->get_function(call->get_name())};

    if (!function) {
        // TODO: add information of line number and origin
        throw ninx::evaluator::exception::RuntimeException(0, "TODO", "Function \""+call->get_name()+"\" has not been declared!");
    }

    // Clone the function definition body, to generate a new instance
    auto body {function->get_body()->clone<ninx::parser::element::Block>()};

    // Load all the call arguments

    // Check if the call arguments are too many
    if (call->get_argument_count() > function->get_arguments().size()) {
        // TODO: add information of line number and origin
        throw ninx::evaluator::exception::RuntimeException(0, "TODO", "Function \""+call->get_name()+"\" is called with too many parameters!");
    }

    // Check if the named call arguments are present in the function definition and make sure that
    // after a named parameter, no ordinal parameters can be used.
    // Then populate the call arguments as function body block local variables
    bool named_started {false};
    int index {0};  // Current ordinal parameter index

    std::unordered_set<std::string> call_mandatory_arguments;

    for (auto &argument : call->get_arguments()) {
        if (argument->get_name()) {
            auto argument_name {*(argument->get_name())};
            named_started = true;

            if (!function->check_argument(argument_name)){
                // TODO: add information of line number and origin
                throw ninx::evaluator::exception::RuntimeException(0, "TODO", "Argument \"" + *(argument->get_name()) +
                "\" is not a valid for function \""+call->get_name()+"\"");
            }

            if (function->check_mandatory(argument_name)) {
                call_mandatory_arguments.insert(argument_name);
            }

            // Evaluate the argument value and get the result
            reset_return_block();
            argument->accept(this);
            auto result_value {get_owned_return_block()};

            // Setup the argument as a local variable
            body->set_variable(argument_name, std::move(result_value));
        }else{
            // Check if an ordinal parameter has been used after a named parameter
            if (named_started) {
                // TODO: add information of line number and origin
                throw ninx::evaluator::exception::RuntimeException(0, "TODO", "Cannot use ordinals arguments after a named argument");
            }

            auto definition_argument {function->get_arguments().at(static_cast<unsigned long long int>(index)).get()};

            if (function->check_mandatory(definition_argument->get_name())) {
                call_mandatory_arguments.insert(definition_argument->get_name());
            }

            // Evaluate the argument value and get the result
            reset_return_block();
            argument->accept(this);
            auto result_value {get_owned_return_block()};

            // Setup the argument as a local variable
            body->set_variable(definition_argument->get_name(), std::move(result_value));
        }
        index++;
    }

    // Add also the outer_argument
    if (call->get_outer_argument()) {
        auto last_definition_argument {function->get_arguments().at(function->get_arguments().size()-1).get()};

        if (function->check_mandatory(last_definition_argument->get_name())) {
            call_mandatory_arguments.insert(last_definition_argument->get_name());
        }

        // Evaluate the argument value and get the result
        reset_return_block();
        call->get_outer_argument()->accept(this);
        auto result_value {get_owned_return_block()};

        // Setup the argument as a local variable
        body->set_variable(last_definition_argument->get_name(), std::move(result_value));
    }

    // Check if all mandatory arguments are been used
    if (call_mandatory_arguments.size() != function->get_mandatory_arguments().size()) {
        std::set<std::string> missing;

        // Find the missing parameters by calculating the difference between the definition and the call
        std::set_difference(function->get_mandatory_arguments().begin(), function->get_mandatory_arguments().end(),
                call_mandatory_arguments.begin(), call_mandatory_arguments.end(),
                std::inserter(missing, missing.end()));

        auto joined_missing {boost::algorithm::join(missing, ", ")};
        // TODO: add information of line number and origin
        throw ninx::evaluator::exception::RuntimeException(0, "TODO", "Missing required arguments: \""+joined_missing+"\"");
    }

    body->accept(this);
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionDefinition *e) {
    e->get_parent()->set_function(e->get_name(), e);

    // Load all the function argument default as local variables in the function body block
    for (auto &argument : e->get_arguments()) {
        // Evaluate the argument
        reset_return_block();
        argument->accept(this);

        auto argument_value {get_owned_return_block()};
        // If the argument is non-null, copy the value
        if (argument_value) {
            e->get_body()->set_variable(argument->get_name(), std::move(argument_value));
        }
    }
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionArgument *e) {
    no_echo([&]{
        // Evaluate the default value if present
        if (e->get_default_value()) {
            e->get_default_value()->accept(this);
        }
    });
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionCallArgument *e) {
    no_echo([&]{
        // Evaluate the argument value
        e->get_value()->accept(this);
    });
}


// VARIABLES

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::VariableRead *e) {
    auto variable {e->get_parent()->get_variable(e->get_name())};

    if (!variable) {
        // TODO: add information of line number and origin
        throw ninx::evaluator::exception::RuntimeException(0, "TODO", "Variable \""+e->get_name()+"\" has not been declared!");
    }

    variable->accept(this);
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::Assignment *e) {
    no_echo([&]{
        // Evaluate the expression
        reset_return_block();
        e->get_value()->accept(this);

        // Get the result as a unique pointer and move it to the block scope
        auto result {get_owned_return_block()};
        e->get_parent()->set_variable(e->get_name(), std::move(result));
    });
}





// EXPRESSION EVALUATION

double last_evaluation_value = 0;  // Used to keep the current total for the expression evaluation

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::TextElement *e) {
    // If the parent is echoing, print the text
    if (e->get_parent()->is_echoing() && !is_echoing_disabled) {
        this->output << e->get_text();
    }

    // If the text is a numeric value, store the value for the expression evaluation
    // TODO: make this check more efficient
    try
    {
        last_evaluation_value = e->convert_to_double();
    }
    catch(const boost::bad_lexical_cast &)
    {
        last_evaluation_value = std::numeric_limits<double>::quiet_NaN();
    }
}



void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::Block *e) {
    for (auto &statement : e->get_statements()) {
        statement->accept(this);
    }

    // Clone the block and make it available as a return value
    replace_return_block(e->clone<ninx::parser::element::Block>());
}

void ninx::evaluator::DefaultEvaluator::visit_binary_expression(ninx::parser::element::BinaryExpression *e,
                                                                std::function<double(double, double)> const &operation) {
    no_echo([&]{
        // Reset the return block pointer
        reset_return_block();

        e->get_first()->accept(this);
        double first_operand {last_evaluation_value};

        e->get_second()->accept(this);
        double second_operand {last_evaluation_value};

        last_evaluation_value = operation(first_operand, second_operand);

        auto result_block {ninx::parser::element::Block::make_text_block(e->get_parent(), boost::lexical_cast<std::string>(last_evaluation_value))};
        replace_return_block(std::move(result_block));
    });
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::AddExpression *e) {
    visit_binary_expression(e, [](double first, double second) {
        return first + second;
    });
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::SubtractExpression *e) {
    visit_binary_expression(e, [](double first, double second) {
        return first - second;
    });
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::MultiplicationExpression *e) {
    visit_binary_expression(e, [](double first, double second) {
        return first * second;
    });
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::DivisionExpression *e) {
    visit_binary_expression(e, [](double first, double second) {
        return first / second;
    });
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::EqualExpression *e) {
    visit_binary_expression(e, [](double first, double second) {
        return first == second;
    });
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::NotEqualExpression *e) {
    visit_binary_expression(e, [](double first, double second) {
        return first != second;
    });
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::IfCondition *e) {
    bool condition_fulfilled {false};  // Becomes true if at least one case of the if is correct, used to trigger the else body.

    for (auto &if_case : e->get_cases()) {
        // Evaluate the condition
        if_case->accept(this);

        if (last_evaluation_value == 1) {
            condition_fulfilled = true;

            auto body { if_case->get_body()->clone<ninx::parser::element::Block>() };
            body->accept(this);

            break;
        }
    }

    if (!condition_fulfilled && e->get_else_body()) {  // No conditions where fulfilled, execute the else block
        auto body { e->get_else_body()->clone<ninx::parser::element::Block>() };
        body->accept(this);
    }
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::IfCase *e) {
    // Evaluate the condition
    e->get_condition()->accept(this);
}
