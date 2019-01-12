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

#ifndef NINX_FUNCTIONCALLARGUMENT_H
#define NINX_FUNCTIONCALLARGUMENT_H

#include <memory>
#include "ASTElement.h"

namespace ninx {
    namespace parser {
        namespace element {
            class FunctionCallArgument : public ASTElement {
            public:
                explicit FunctionCallArgument(std::unique_ptr<std::string> name, std::unique_ptr<Block> value);

                void accept(ninx::evaluator::Evaluator *evaluator) override;

                void set_parent(Block *parent) override;

                std::string dump(int level) const override;

                const std::unique_ptr<std::string> &get_name() const;
                const std::unique_ptr<Block> &get_value() const;

            private:
                std::unique_ptr<std::string> name;  // Argument name ( could be NULL )
                std::unique_ptr<Block> value;  // Argument's value
            };
        }
    }
}


#endif //NINX_FUNCTIONCALLARGUMENT_H
