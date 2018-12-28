//
// Created by federico.terzi2 on 28/12/2018.
//

#ifndef NINX_DEFAULTEVALUATOR_H
#define NINX_DEFAULTEVALUATOR_H

#include "Evaluator.h"

namespace ninx {
    namespace evaluator {
        class DefaultEvaluator : public Evaluator{
        public:
            void visit(ninx::parser::element::TextElement *e) override;

            void visit(ninx::parser::element::Assignment *e) override;

            void visit(ninx::parser::element::Block *e) override;

            void visit(ninx::parser::element::FunctionCall *e) override;

            void visit(ninx::parser::element::VariableRead *e) override;
        };
    }
}


#endif //NINX_DEFAULTEVALUATOR_H
