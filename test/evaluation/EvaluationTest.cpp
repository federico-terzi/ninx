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
#include <boost/algorithm/string.hpp>
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
    lexer.set_verbose(true);
    auto tokens = lexer.generate();
    ninx::parser::Parser parser{tokens, "tests"};
    auto ast{parser.parse()};
    std::stringstream output;
    ninx::evaluator::DefaultEvaluator eval{output};
    eval.evaluate(ast.get());
    return output.str();
}

std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
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

    BOOST_AUTO_TEST_CASE(test_variable_formatting) {
        auto output{eval(
                R"NINX(
$x={2}
$y=$x
$x = {3}
$x $y
)NINX"
        )};

        BOOST_CHECK_EQUAL(output, "3 2");  // Notice the space between
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

    BOOST_AUTO_TEST_CASE(test_function_indent_removing_spaces) {
        auto output{eval(
                R"NINX(
@func test() {
    Indented with spaces!
}
@test
)NINX"
        )};

        BOOST_CHECK_EQUAL(output, "Indented with spaces!\n");
    }

    BOOST_AUTO_TEST_CASE(test_function_reference_read) {
        auto output{eval(
                R"NINX(
$x={2}
@func test() {
$x
}
@test
)NINX"
        )};

        BOOST_CHECK_EQUAL(output, "2");
    }

    BOOST_AUTO_TEST_CASE(test_function_reference_write) {
        auto output{eval(
                R"NINX(
$i={0}
@func test() {
$i={2}
}
@test
$i
)NINX"
        )};

        BOOST_CHECK_EQUAL(output, "2");
    }

    BOOST_AUTO_TEST_CASE(test_function_reference_multiple_write) {
        auto output{eval(
                R"NINX(
$i={0}
@func test() {
$i=$i+{1}
}
@test
@test
$i
)NINX"
        )};

        BOOST_CHECK_EQUAL(output, "2");
    }

    BOOST_AUTO_TEST_CASE(test_function_late_call) {
        auto output{eval(
                R"NINX(
$i={0}
@func test() {
    $i
}
First @test
Second @test?
$i = {3}
)NINX"
        )};

        boost::trim(output);

        BOOST_CHECK_EQUAL(output, "First 0\nSecond 3");
    }

    BOOST_AUTO_TEST_CASE(test_function_late_call_nested_function) {
        auto output{eval(
                R"NINX(
$i={0}
$test = {
    @func inner() {
         $i
    }
}
First @test.inner
Second @test.inner?
$i = {3}
)NINX"
        )};

        boost::trim(output);

        BOOST_CHECK_EQUAL(output, "First 0\nSecond 3");
    }

    BOOST_AUTO_TEST_CASE(test_function_late_call_nested_late_calls) {
        auto output{eval(
                R"NINX(
$x = {1}
@func name() {
	Third $x
}
@func test() {
	First
	$x

	@name?

	$x = {2}
	Second
}
Test
@test?
$x = {3}
End
)NINX"
        )};

        boost::trim(output);

        BOOST_CHECK_EQUAL(output, R"(Test
First
3
Third 2
Second
End)");
    }

    BOOST_AUTO_TEST_CASE(test_function_late_call_builtin) {
        auto output{eval(
                R"NINX(
$i={}
@i.add({3})
First @i.size
Second @i.size?
@i.add({2})
)NINX"
        )};

        boost::trim(output);

        BOOST_CHECK_EQUAL(output, "First 1\nSecond 2");
    }

    // Operators

    BOOST_AUTO_TEST_CASE(test_simple_operator) {
        auto output{eval(
                R"NINX(
@func bold($body) {\<b\>$body\<\/b\>}
@operator bold *

This text can be *bold*
)NINX"
        )};

        boost::trim(output);

        BOOST_CHECK_EQUAL(output, "This text can be <b>bold</b>");
    }

    BOOST_AUTO_TEST_CASE(test_multiple_operators) {
        auto output{eval(
                R"NINX(
@func bold($body) {\<b\>$body\<\/b\>}
@func italic($body) {\<i\>$body\<\/i\>}
@operator bold *
@operator italic _
why _not *together*_
)NINX"
        )};

        boost::trim(output);

        BOOST_CHECK_EQUAL(output, "why <i>not <b>together</b></i>");
    }

    BOOST_AUTO_TEST_CASE(test_operator_spacing) {
        auto output{eval(
                R"NINX(
@func bold($body) {\<b\>$body\<\/b\> }
@operator bold *

First *test* second
)NINX"
        )};

        boost::trim(output);



        BOOST_CHECK_EQUAL(output, "First <b>test</b> second");
    }

    // Comments

    BOOST_AUTO_TEST_CASE(test_comment) {
        auto output{eval(
                R"NINX(
First
/* Commented line */
Second
)NINX"
        )};

        boost::trim(output);

        BOOST_CHECK_EQUAL(output, "First\nSecond");  // Notice the space between
    }

    BOOST_AUTO_TEST_CASE(test_multiline_comment) {
        auto output{eval(
                R"NINX(
First
/*
Multiple comment
lines
*/
Second
)NINX"
        )};

        boost::trim(output);

        BOOST_CHECK_EQUAL(output, "First\nSecond");  // Notice the space between
    }

BOOST_AUTO_TEST_SUITE_END()