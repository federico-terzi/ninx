//
// Created by federico.terzi2 on 25/12/2018.
//

#include "FunctionCall.h"

ninx::parser::element::FunctionCall::FunctionCall(const std::string &name) : name(name) {}

std::string ninx::parser::element::FunctionCall::dump(int level) const {
    return std::string(level, '\t')+ "FunctionCall: "+this->name;
}

void ninx::parser::element::FunctionCall::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}
