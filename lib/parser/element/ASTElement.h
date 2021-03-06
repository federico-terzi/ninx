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

#ifndef NINX_ASTELEMENT_H
#define NINX_ASTELEMENT_H

#include <iostream>
#include <memory>
#include "../../evaluator/Evaluator.h"

namespace ninx {
    namespace parser {
        namespace element {
            class ASTElement {
            public:
                virtual ~ASTElement() = default;

                virtual void accept(ninx::evaluator::Evaluator * evaluator) = 0;

                virtual std::string dump(int level) const = 0;

                friend std::ostream& operator<<(std::ostream &strm, const ASTElement &a) {
                    return strm << a.dump(0);
                }

                template<class T>
                std::unique_ptr<T> clone() {
                    std::unique_ptr<T> new_obj(dynamic_cast<T*>(clone_impl()));
                    new_obj->set_parent(this->get_parent());
                    new_obj->__set_output_block(this->__get_output_block());
                    return new_obj;
                };

                Block *get_parent() const {
                    return parent;
                }

                virtual void set_parent(Block *parent) {
                    this->parent = parent;
                }

                Block *__get_output_block() const {
                    return __output_block;
                }

                virtual void __set_output_block(Block *__output_block) {
                    ASTElement::__output_block = __output_block;
                }

            protected:
                Block * parent = nullptr;  // Reference to the parent block

                virtual ASTElement * clone_impl() = 0;

                // RUNTIME METHODS, SHOULD NOT BE USED DIRECTLY
                Block * __output_block = nullptr;
            };
        }
    }
}

#endif //NINX_ASTELEMENT_H
