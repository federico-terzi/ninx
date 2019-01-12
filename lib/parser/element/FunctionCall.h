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

#ifndef NINX_FUNCTIONCALL_H
#define NINX_FUNCTIONCALL_H

#include <memory>
#include <vector>
#include "Statement.h"

namespace ninx {
    namespace parser {
        namespace element {
            class FunctionCall : public Statement {
            public:
                explicit FunctionCall(const std::string &name, std::vector<std::unique_ptr<FunctionCallArgument>> arguments,
                                                      std::unique_ptr<FunctionCallArgument> outer_argument);

                std::string dump(int level) const override;

                void accept(ninx::evaluator::Evaluator *evaluator) override;

                void set_parent(Block *parent) override;

                const std::string &get_name() const;
                const std::vector<std::unique_ptr<FunctionCallArgument>> &get_arguments() const;
                const std::unique_ptr<FunctionCallArgument> &get_outer_argument() const;

                /**
                 * @return the number of the arguments of the call, adding 1 for the outer_argument if present.
                 */
                int get_argument_count() const;
            private:
                std::string name; // The name of the function to be called
                std::vector<std::unique_ptr<FunctionCallArgument>> arguments;  // Function call arguments
                std::unique_ptr<FunctionCallArgument> outer_argument;  // Function call argument, specified by the block syntax.
            };
        }
    }
}


#endif //NINX_FUNCTIONCALL_H
