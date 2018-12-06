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


#include "Reader.h"
#include <vector>

int ninx::lexer::Reader::get_line_number() const {
    return line_number;
}

void ninx::lexer::Reader::increment_line() {
    this->line_number++;
}

ninx::lexer::Reader::Reader(std::istream &stream, std::string &origin) : stream{stream}, origin{origin}{}

void ninx::lexer::Reader::ignore_spaces() {
    while(isspace(this->stream.peek())) {
        int current = this->stream.get();
        if (current == '\n') { // Increment the line count if a newline was found
            this->increment_line();
        }
    }
}

std::string ninx::lexer::Reader::read_until(char c) {
    std::vector<char> buffer;  // TODO levare
    buffer.reserve(100);

    while(stream && stream.peek() != c) {
        char current = static_cast<char>(stream.get());
        if (current == '\n') { // Increment the line count if a newline was found
            this->increment_line();
        }

        buffer.push_back(current);
    }

    buffer.push_back(0);

    return std::move(std::string {buffer.data()});
}
