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

#ifndef NINX_DEFAULTEVALUATOR_H
#define NINX_DEFAULTEVALUATOR_H

#include <functional>
#include "Evaluator.h"


namespace ninx {
    namespace evaluator {
        class DefaultEvaluator : public Evaluator {
        public:
            void visit(ninx::parser::element::TextElement *e) override;

            void visit(ninx::parser::element::Assignment *e) override;

            void visit(ninx::parser::element::Block *e) override;

            void visit(ninx::parser::element::FunctionCall *call) override;

            void visit(ninx::parser::element::VariableRead *e) override;

            void visit(ninx::parser::element::FunctionDefinition *e) override;

            void visit(ninx::parser::element::FunctionArgument *e) override;

            void visit(ninx::parser::element::FunctionCallArgument *e) override;

            void visit(ninx::parser::element::IfStatement *e) override;

            void visit(ninx::parser::element::IfCase *e) override;

            void visit(ninx::parser::element::ForStatement *e) override;

            void visit(ninx::parser::element::AddExpression *e) override;

            void visit(ninx::parser::element::SubtractExpression *e) override;

            void visit(ninx::parser::element::MultiplicationExpression *e) override;

            void visit(ninx::parser::element::DivisionExpression *e) override;

            void visit(ninx::parser::element::EqualExpression *e) override;

            void visit(ninx::parser::element::NotEqualExpression *e) override;

            void visit(ninx::parser::element::NoopStatement *e) override;

            void evaluate_function(ninx::parser::element::Block *target_object,
                                   ninx::parser::element::FunctionDefinition *function,
                                   std::unique_ptr<ninx::parser::element::Block> body);

            void evaluate(ninx::parser::element::Block *e) override;

            explicit DefaultEvaluator(std::ostream &output);

        private:
            std::ostream &output;

            void visit_binary_expression(ninx::parser::element::BinaryExpression *e,
                                         std::function<double(double, double)> const &operation);
        };

    }
}


#endif //NINX_DEFAULTEVALUATOR_H
