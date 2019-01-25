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
#include "ForStatement.h"

ninx::parser::element::ForStatement::ForStatement(const std::string &iterator_name, std::unique_ptr<ninx::parser::element::Expression> range_expr,
                                                  std::unique_ptr<ninx::parser::element::Block> body)
        : iterator_name(iterator_name), range_expr(std::move(range_expr)), body(std::move(body)) {}

std::string ninx::parser::element::ForStatement::dump(int level) const {
    std::stringstream s;

    s << std::string(level, '\t') + "ForStatement {" << std::endl;
    s << std::string(level+1, '\t') + "Iterator: " << iterator_name << std::endl;
    s << this->body->dump(level+1) << std::endl;
    s << std::string(level, '\t') + "}" << std::endl;

    return s.str();
}

const std::unique_ptr<ninx::parser::element::Expression> &ninx::parser::element::ForStatement::get_range_expr() const {
    return range_expr;
}

const std::unique_ptr<ninx::parser::element::Block> &ninx::parser::element::ForStatement::get_body() const {
    return body;
}

const std::string &ninx::parser::element::ForStatement::get_iterator_name() const {
    return iterator_name;
}

void ninx::parser::element::ForStatement::set_parent(ninx::parser::element::Block *parent) {
    ASTElement::set_parent(parent);

    this->range_expr->set_parent(parent);
    this->body->set_parent(parent);
}

void ninx::parser::element::ForStatement::accept(ninx::evaluator::Evaluator *evaluator) {
    evaluator->visit(this);
}

ninx::parser::element::ForStatement *ninx::parser::element::ForStatement::clone_impl() {
    return new ForStatement(this->iterator_name, this->range_expr->clone<Expression>(), this->body->clone<Block>());
}


