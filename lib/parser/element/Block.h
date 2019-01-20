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

#ifndef NINX_BLOCK_H
#define NINX_BLOCK_H

#include <vector>
#include <memory>
#include <unordered_map>
#include "ASTElement.h"
#include "Statement.h"
#include "FunctionDefinition.h"
#include "parser/element/expression/Expression.h"

namespace ninx {
    namespace parser {
        namespace element {
            class Block : public Expression {
            public:
                explicit Block(std::vector<std::unique_ptr<Statement>> statements);

                std::string dump(int level) const override;

                void accept(ninx::evaluator::Evaluator *evaluator) override;

                const std::vector<std::unique_ptr<Statement>> &get_statements() const;

                Block * get_variable(const std::string& name) const;
                void set_variable(const std::string &name, std::unique_ptr<Block> value);
                void clear_variables();

                FunctionDefinition * get_function(const std::string &name) const;
                void set_function(const std::string &name, std::unique_ptr<FunctionDefinition> func);

                static std::unique_ptr<Block> make_text_block(Block * parent, const std::string& text);

                bool is_echoing() const;
                virtual void set_echoing(bool echoing);
            protected:
                Block *clone_impl() override;

            private:
                std::vector<std::unique_ptr<Statement>> statements;

                /*
                 * If this is true, the statements will produce an output, otherwise they will not.
                 * This property WILL NOT BE COPIED
                 */
                bool echoing = true;

                std::unordered_map<std::string, std::unique_ptr<Block>> variables;
                std::unordered_map<std::string, std::unique_ptr<FunctionDefinition>> functions;
            };
        }
    }
}


#endif //NINX_BLOCK_H
