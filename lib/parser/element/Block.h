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
#include <map>
#include <unordered_map>
#include <functional>
#include <parser/util/LateCallDescriptor.h>
#include "ASTElement.h"
#include "Statement.h"
#include "FunctionDefinition.h"
#include "FunctionCall.h"
#include "parser/element/expression/Expression.h"

namespace ninx {
    namespace parser {
        namespace element {
            class Block : public Expression {
            public:
                explicit Block(std::vector<std::unique_ptr<Statement>> statements);

                std::string dump(int level) const override;

                void accept(ninx::evaluator::Evaluator *evaluator) override;

                Block *get_variable(const std::string &name) const;

                Block *get_variable(const std::string &name, bool only_local) const;

                void set_variable(const std::string &name, std::unique_ptr<Block> value, bool force_local);

                void clear_variables();

                FunctionDefinition *get_function(const std::string &name) const;

                void set_function(const std::string &name, std::unique_ptr<FunctionDefinition> func);

                int get_children_count() const;

                void add_child(std::unique_ptr<Statement> statement);

                const std::vector<std::unique_ptr<Statement>> &get_children() const;

                bool is_echoing() const;

                virtual void set_echoing(bool echoing);

                static std::unique_ptr<Block> make_text_block(Block *parent, const std::string &text);
                static std::unique_ptr<Block> make_empty(Block *parent);

                // BUILT-IN METHODS
                FunctionDefinition * get_builtin(const std::string &name) const;
                static std::unordered_map<std::string, std::unique_ptr<FunctionDefinition>> builtin_functions;

                // RUNTIME METHODS, THEY SHOULD NOT BE USED DIRECTLY
                std::string __render_output();
                size_t __add_output_segment(const std::string &text);
                void __set_output_segment_position(int position);
                size_t __add_late_call(std::unique_ptr<ninx::parser::util::LateCallDescriptor> descriptor);
                const std::map<size_t, std::unique_ptr<ninx::parser::util::LateCallDescriptor>> &__get_late_calls() const;

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

                // RUNTIME DATA STRUCTURES, THEY SHOULD NOT BE USED DIRECTLY
                std::vector<std::string> __output_segments;
                int __current_output_segment_position = -1;
                std::map<size_t, std::unique_ptr<ninx::parser::util::LateCallDescriptor>> __late_calls;
            };
        }
    }
}


#endif //NINX_BLOCK_H
