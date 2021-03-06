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

#ifndef NINX_IFCONDITIONAL_H
#define NINX_IFCONDITIONAL_H

#include <vector>
#include "Statement.h"
#include "IfCase.h"

namespace ninx {
    namespace parser {
        namespace element {
            class IfStatement : public Statement {
            public:
                explicit IfStatement(std::vector<std::unique_ptr<IfCase>> cases, std::unique_ptr<Block> else_body);

                std::string dump(int level) const override;

                void accept(ninx::evaluator::Evaluator *evaluator) override;

                void set_parent(Block *parent) override;

                const std::vector<std::unique_ptr<IfCase>> &get_cases() const;
                const std::unique_ptr<Block> &get_else_body() const;

            protected:
                IfStatement *clone_impl() override;

            private:
                std::vector<std::unique_ptr<IfCase>> cases;
                std::unique_ptr<Block> else_body;  // CAN BE NULL
            };
        }
    }
}


#endif //NINX_IFCONDITIONAL_H
