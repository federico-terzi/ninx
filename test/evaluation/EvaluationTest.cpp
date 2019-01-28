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
#include "evaluator/DefaultEvaluator.h"

std::string eval(const std::string &command) {
    std::istringstream ss(
            command
    );
    std::string origin{"test"};
    ninx::lexer::Lexer lexer{ss, "tests"};
    auto tokens = lexer.generate();
    ninx::parser::Parser parser{tokens, "tests"};
    auto ast{parser.parse()};
    std::stringstream output;
    ninx::evaluator::DefaultEvaluator eval{output};
    eval.evaluate(ast.get());
    return output.str();
}

BOOST_AUTO_TEST_SUITE(ninx_evaluation)

    BOOST_AUTO_TEST_CASE(test_basic_number) {
        auto output{eval("5")};

        BOOST_CHECK_EQUAL(output, "5");
    }

    BOOST_AUTO_TEST_CASE(test_basic_text) {
        auto output{eval("Hello")};

        BOOST_CHECK_EQUAL(output, "Hello");
    }

    // VARIABLES

    BOOST_AUTO_TEST_CASE(test_basic_variable) {
        auto output{eval(
                "$x={2}"
                "$x"
        )};

        BOOST_CHECK_EQUAL(output, "2");
    }

    BOOST_AUTO_TEST_CASE(test_variable_copy) {
        auto output{eval(
                R"NINX(
$x={2}
$y=$x
$x = {3}
$x$y
)NINX"
        )};

        BOOST_CHECK_EQUAL(output, "32");
    }

    BOOST_AUTO_TEST_CASE(test_variable_reference_read) {
        auto output{eval(
                R"NINX(
$x={2}
$y={Hello $x}
$y
)NINX"
        )};

        BOOST_CHECK_EQUAL(output, "Hello 2");
    }

    BOOST_AUTO_TEST_CASE(test_variable_reference_write) {
        auto output{eval(
                R"NINX(
$x={2}
$y={$x={4}}
$x
)NINX"
        )};

        BOOST_CHECK_EQUAL(output, "4");
    }

    // FUNCTIONS

    BOOST_AUTO_TEST_CASE(test_basic_function) {
        auto output{eval(
                "@func test() {"
                "Hello"
                "}"
                "@test"
        )};

        BOOST_CHECK_EQUAL(output, "Hello");
    }

BOOST_AUTO_TEST_SUITE_END()