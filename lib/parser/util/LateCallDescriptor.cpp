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

#include "LateCallDescriptor.h"
#include "parser/element/Block.h"
#include "parser/element/FunctionDefinition.h"

ninx::parser::util::LateCallDescriptor::LateCallDescriptor(std::unique_ptr<Block> body,
                                                           FunctionDefinition *function, Block *target_object) :
        body(std::move(body)), function(function), target_object(target_object) {}

const std::unique_ptr<Block> &ninx::parser::util::LateCallDescriptor::get_body() const {
    return body;
}

FunctionDefinition *ninx::parser::util::LateCallDescriptor::get_function() const {
    return function;
}

Block *ninx::parser::util::LateCallDescriptor::get_target_object() const {
    return target_object;
}

std::unique_ptr<ninx::parser::util::LateCallDescriptor> ninx::parser::util::LateCallDescriptor::clone() {
    return std::make_unique<LateCallDescriptor>(body->clone<ninx::parser::element::Block>(), function, target_object);
}
