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

#ifndef NINX_FUNCTIONDEFINITION_H
#define NINX_FUNCTIONDEFINITION_H

#include <vector>
#include <memory>
#include <unordered_set>
#include <functional>
#include "FunctionArgument.h"
#include "Statement.h"

namespace ninx {
    namespace parser {
        namespace element {
            class FunctionDefinition : public Statement {
            public:
                explicit FunctionDefinition(const std::string &name,
                                            std::vector<std::unique_ptr<FunctionArgument>> arguments,
                                            std::unique_ptr<Block> body);

                std::string dump(int level) const override;

                void accept(ninx::evaluator::Evaluator *evaluator) override;

                void set_parent(Block *parent) override;

                const std::string &get_name() const;

                const std::vector<std::unique_ptr<FunctionArgument>> &get_arguments() const;

                const std::unique_ptr<Block> &get_body() const;

                const std::unordered_set<std::string> &get_mandatory_arguments() const;

                void set_evaluator(std::function<std::unique_ptr<Block>(Block *target, std::unique_ptr<Block> body)> evaluator);
                std::unique_ptr<Block> evaluate(Block *target, std::unique_ptr<Block> args);

                /**
                 * Efficiently check if the given argument is present in the function definition
                 * @param name of the argument
                 * @return true if the argument is valid, false otherwise.
                 */
                bool check_argument(const std::string &name);

                /**
                 * Efficiently check if the given argument is one of the mandatory of the function definition
                 * @param name of the argument
                 * @return true if the argument is mandatory, false otherwise.
                 */
                bool check_mandatory(const std::string &name);

            protected:
                FunctionDefinition *clone_impl() override;

            private:
                std::string name;  // Function name
                std::vector<std::unique_ptr<FunctionArgument>> arguments;  // Function arguments
                std::unique_ptr<Block> body;  // Function body block

                std::function<std::unique_ptr<Block>(Block *target, std::unique_ptr<Block> body)> evaluator = [](
                        Block *target, std::unique_ptr<Block> args) {
                    // Default NOOP
                    return std::move(args);
                };

                std::unordered_set<std::string> mandatory_arguments;  // Used to efficiently check if an argument is mandatory
                std::unordered_set<std::string> _argument_name_cache;  // Used to efficiently check if an argument is valid
            };
        }
    }
}


#endif //NINX_FUNCTIONDEFINITION_H
