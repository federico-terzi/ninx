//
// Created by federico.terzi2 on 28/12/2018.
//

#include <iostream>
#include "DefaultEvaluator.h"
#include "../parser/element/TextElement.h"
#include "../parser/element/Block.h"

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::TextElement *e) {
    std::cout << e->get_text();
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::Assignment *e) {

}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::Block *e) {
    for (auto &statement : e->get_statements()) {
        statement->accept(this);
    }
}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::FunctionCall *e) {

}

void ninx::evaluator::DefaultEvaluator::visit(ninx::parser::element::VariableRead *e) {

}
