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

#include "NotEqualExpression.h"


ninx::parser::element::NotEqualExpression::NotEqualExpression(std::unique_ptr<ninx::parser::element::Expression> first,
                                                    std::unique_ptr<ninx::parser::element::Expression> second)
        : BinaryExpression(std::move(first), std::move(second)) {


}

void ninx::parser::element::NotEqualExpression::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

ninx::parser::element::NotEqualExpression *ninx::parser::element::NotEqualExpression::clone_impl() {
    return new NotEqualExpression(this->first->clone<Expression>(), this->second->clone<Expression>());
}

std::string ninx::parser::element::NotEqualExpression::get_dump_name() const {
    return "NotEqualExpression";
}
