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

#ifndef NINX_EVALUATOR_H
#define NINX_EVALUATOR_H

// Forward declaration of classes
namespace ninx {
    namespace parser {
        namespace element{
            class TextElement;
            class Assignment;
            class Block;
            class FunctionCall;
            class VariableRead;
        }
    }
}

namespace ninx {
    namespace evaluator {
        class Evaluator {
        public:
            virtual ~Evaluator() = default;

            virtual void visit(ninx::parser::element::TextElement * e) = 0;
            virtual void visit(ninx::parser::element::Assignment * e) = 0;
            virtual void visit(ninx::parser::element::Block * e) = 0;
            virtual void visit(ninx::parser::element::FunctionCall * e) = 0;
            virtual void visit(ninx::parser::element::VariableRead * e) = 0;
        };
    }
}

#endif
