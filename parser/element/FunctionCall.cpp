//
// Created by federico.terzi2 on 25/12/2018.
//

#include "FunctionCall.h"

ninx::parser::element::FunctionCall::FunctionCall(const std::string &name) : name(name) {}

std::string ninx::parser::element::FunctionCall::dump() const {
    return "FunctionCall: "+this->name;
}
