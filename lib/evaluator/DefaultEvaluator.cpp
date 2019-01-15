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
#include <set>
#include <boost/algorithm/string/join.hpp>
#include <boost/lexical_cast/bad_lexical_cast.hpp>
#include "DefaultEvaluator.h"
#include "../parser/element/TextElement.h"
#include "../parser/element/Block.h"
#include "../parser/element/Assignment.h"
#include "../parser/element/VariableRead.h"
#include "../parser/element/FunctionDefinition.h"
#include "../parser/element/FunctionCall.h"
#include "../parser/element/FunctionCallArgument.h"
#include "../parser/element/AddExpression.h"
#include "exception/RuntimeException.h"


void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionCall *call) {
    auto function {call->get_parent()->get_function(call->get_name())};

    if (!function) {
        // TODO: add information of line number and origin
        throw ninx::evaluator::exception::RuntimeException(0, "TODO", "Function \""+call->get_name()+"\" has not been declared!");
    }

    // Clear all the function body local variables
    function->get_body()->clear_variables();

    // Load all the function argument default as local variables in the function body block
    for (auto &argument : function->get_arguments()) {
        function->get_body()->set_variable(argument->get_name(), argument->get_default_value().get());
    }

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

            // Setup the argument as a local variable
            function->get_body()->set_variable(argument_name, argument->get_value().get());
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

            // Setup the argument as a local variable
            function->get_body()->set_variable(definition_argument->get_name(), argument->get_value().get());
        }
        index++;
    }

    // Add also the outer_argument
    if (call->get_outer_argument()) {
        auto last_definition_argument {function->get_arguments().at(function->get_arguments().size()-1).get()};

        if (function->check_mandatory(last_definition_argument->get_name())) {
            call_mandatory_arguments.insert(last_definition_argument->get_name());
        }

        function->get_body()->set_variable(last_definition_argument->get_name(), call->get_outer_argument()->get_value().get());
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

    function->get_body()->accept(this);
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::VariableRead *e) {
    auto variable {e->get_parent()->get_variable(e->get_name())};

    if (!variable) {
        // TODO: add information of line number and origin
        throw ninx::evaluator::exception::RuntimeException(0, "TODO", "Variable \""+e->get_name()+"\" has not been declared!");
    }

    variable->accept(this);
}

ninx::evaluator::DefaultEvaluator::DefaultEvaluator(std::ostream &output) : output(output) {}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionDefinition *e) {
    e->get_parent()->set_function(e->get_name(), e);
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionArgument *e) {

}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionCallArgument *e) {

}


// EXPRESSION EVALUATION

double last_evaluation_value = 0;  // Used to keep the current total for the expression evaluation

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::TextElement *e) {
    this->output << e->get_text();

    // If the text is a numeric value, store the value for the expression evaluation
    // TODO: make this check more efficient
    try
    {
        last_evaluation_value = e->convert_to_double();
    }
    catch(const boost::bad_lexical_cast &)
    {}
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::Assignment *e) {
    e->get_parent()->set_variable(e->get_name(), e->get_value());
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::Block *e) {
    for (auto &statement : e->get_statements()) {
        statement->accept(this);
    }
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::AddExpression *e) {
    e->get_first()->accept(this);
    double first_operand {last_evaluation_value};

    e->get_second()->accept(this);
    double second_operand {last_evaluation_value};

    last_evaluation_value = first_operand + second_operand;

    std::cout<<last_evaluation_value<<std::endl;
}
