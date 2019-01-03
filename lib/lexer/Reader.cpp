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
#include "exception/LexerException.h"
#include <vector>

using namespace ninx::lexer::exception;

ninx::lexer::Reader::Reader(std::istream &stream, std::string &origin) : stream{stream}, origin{origin} {
    // Allocate the initial buffer size
    this->buffer.reserve(READER_BUFFER_INITIAL_SIZE);
}

void ninx::lexer::Reader::ignore_spaces() {
    int current;
    // Skip all the spaces except the newline
    while (isspace(current = this->stream.peek()) && current != '\n') {
        this->stream.get();
    }
}

bool is_limiter(char c) {
    bool found = false;
    for (char i : ninx::lexer::LIMITER_CHARS) {
        if (i == c) {
            found = true;
            break;
        }
    }
    return found;
}

int ninx::lexer::Reader::get_next_limiter() {
    int next_char = this->stream.peek();
    if (next_char >= 0) {
        if (next_char == '\\') {  // Escape char check
            return -2;
        }else{
            // Check if the char is a limiter
            if (is_limiter(static_cast<char>(next_char))) {
                // If the char is a limiter, consume it
                this->stream.get();

                // Also ignore the trailing spaces
                this->ignore_spaces();

                return next_char;
            }else{
                return -2;
            }
        }
    }else{  // EOF
        return -1;
    }
}

std::string ninx::lexer::Reader::read_until_limiter() {
    this->buffer.clear();

    // Used to prevent more than two consecutive newlines
    int newline_count = 0;

    bool beginning {true}; // Used to trim initial spaces

    while (stream) {
        // Check if the next char is one of the limiting ones
        char next_char = static_cast<char>(stream.peek());
        if (next_char == '\\') {   // Escaping control
            stream.get();  // Discard the escaping char, and avoid limiter check so char is considered as text
        }else{
            if (is_limiter(next_char)) {
                break;
            }
        }

        // Read the current char
        char current = static_cast<char>(stream.get());
        if (current == -1) {
            break;
        }

        if (current == '\n') { // Increment the line count if a newline was found
            this->increment_line();
            newline_count++;
        }else{
            if (!isspace(current)) {
                newline_count = 0;
                beginning = false;
            }
        }

        // If the new character is a newline that exceed the maximum number, remove it
        if (newline_count < MAX_CONSECUTIVE_NEWLINES && !beginning) {
            this->buffer.push_back(current);
        }
    }

    this->buffer.push_back(0);

    return std::move(std::string{buffer.data()});
}

std::string ninx::lexer::Reader::read_identifier() {
    this->buffer.clear();

    while (stream) {
        // An identifier is valid until a space or a limiter is found
        char next_char = static_cast<char>(stream.peek());
        if (isspace(next_char) || is_limiter(next_char)) {
            break;
        }

        // Read the current char
        char current = static_cast<char>(stream.get());
        if (current == -1) {
            break;
        }

        // If the current char is not alphanumeric, raise an exception
        if (isalpha(current) == 0 && isdigit(current) == 0) {
            throw LexerException(this->line_number, this->origin,
                                 "Identifier can only contain alphanumeric characters.");
        }

        this->buffer.push_back(current);
    }

    this->buffer.push_back(0);

    // Eat all the following spaces
    this->ignore_spaces();

    return std::move(std::string{buffer.data()});
}

int ninx::lexer::Reader::get_line_number() const {
    return line_number;
}

void ninx::lexer::Reader::increment_line() {
    this->line_number++;
}

