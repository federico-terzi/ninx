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

#include <boost/test/unit_test.hpp>
#include <sstream>
#include "lexer/Lexer.h"
#include "parser/Parser.h"

BOOST_AUTO_TEST_SUITE(ninx_parser)

    BOOST_AUTO_TEST_CASE(test_parse_block_not_closing)
    {
        std::istringstream ss(
                "$a = {"
                "    this block will not close"
                );
        std::string origin {"test"};
        ninx::lexer::Lexer lexer {ss, "tests"};
        auto tokens = lexer.generate();
        ninx::parser::Parser parser{tokens, "tests"};

        BOOST_CHECK_THROW(parser.parse(), ninx::parser::exception::ParserException);
    }

BOOST_AUTO_TEST_SUITE_END()