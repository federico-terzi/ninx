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

int ninx::lexer::Reader::ignore_spaces() {
    int count = 0;
    int current;
    // Skip all the spaces except the newline
    while (isspace(current = this->stream.peek()) && current != '\n') {
        this->stream.get();
        count++;
    }
    return count;
}

void ninx::lexer::Reader::ignore_spaces_and_newline() {
    while (isspace(this->stream.peek())) {
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

bool ninx::lexer::Reader::ignore_comment() {
    // Verify that this is a comment
    if (this->stream.peek() != '/') {
        return false;  // Not a comment
    }

    this->stream.get();

    if (this->stream.peek() != '*') {
        this->stream.seekg(-1, std::ios_base::cur);  // Rewind the / character
        return false;
    }

    this->stream.get();

    bool is_closing {false};

    while (stream) {
        int current {stream.get()};

        if (is_closing) {
            if (current == '/') {
                break;
            }else{
                is_closing = false;
            }
        }else{
            if (current == '*') {
                is_closing = true;
            }
        }
    }

    // Eat all the following spaces
    this->ignore_spaces_and_newline();

    return true;
}

int ninx::lexer::Reader::get_next_limiter() {
    int next_char = this->stream.peek();
    if (next_char >= 0) {
        if (next_char == '\\') {  // Escape char check
            return FOUND_TEXT;
        }else{
            if(next_char == '-') {  // Negative number handling
                this->stream.get();

                // Check if the next entry is a digit
                if (isdigit(this->stream.peek())) {
                    this->stream.seekg(-1, std::ios_base::cur);
                    return FOUND_TEXT;
                }

                // Also ignore the trailing spaces
                this->ignore_spaces();

                return next_char;
            }

            if (next_char == '/') {  // Comments handling
                bool comments_ignored { this->ignore_comment() };
                if (comments_ignored) {
                    return this->get_next_limiter();
                }
            }

            // Check if the char is a limiter
            if (is_limiter(static_cast<char>(next_char))) {
                // If the char is a limiter, consume it
                this->stream.get();

                // Also ignore the trailing spaces
                this->ignore_spaces();

                return next_char;
            }else{
                return FOUND_TEXT;
            }
        }
    }else{  // EOF
        return FOUND_EOF;
    }
}

std::string ninx::lexer::Reader::read_until_limiter() {
    this->buffer.clear();

    // Used to prevent more than two consecutive newlines
    int newline_count = 0;

    while (stream) {
        // Check if the next char is one of the limiting ones
        char next_char = static_cast<char>(stream.peek());
        if (next_char == '\\') {   // Escaping control
            stream.get();  // Discard the escaping char, and avoid limiter check so char is considered as text
        }else{
            if (is_limiter(next_char) && next_char != '-') {  // Negative number handling
                break;
            }
        }

        // Read the current char
        char current = static_cast<char>(stream.get());
        if (current == -1) {
            break;
        }

        if (current == '-' && !isdigit(stream.peek())) {  // Not a negative number, rollback
            this->stream.seekg(-1, std::ios_base::cur);
            break;
        }

        if (current == '\n') { // Increment the line count if a newline was found
            this->increment_line();
            newline_count++;
        }else{
            if (!isspace(current)) {
                newline_count = 0;
            }
        }

        // If the new character is a newline that exceed the maximum number, remove it
        if (newline_count < MAX_CONSECUTIVE_NEWLINES) {
            this->buffer.push_back(current);
        }
    }

    this->buffer.push_back(0);

    return std::move(std::string{buffer.data()});
}

std::string ninx::lexer::Reader::read_identifier(int &trailing_spaces) {
    bool found;
    return this->read_identifier(trailing_spaces, -1, found);
}

std::string ninx::lexer::Reader::read_identifier(int &trailing_spaces, int target_suffix, bool &found) {
    this->buffer.clear();

    while (stream) {
        // An identifier is valid until a space or a limiter is found
        char next_char = static_cast<char>(stream.peek());
        if (isspace(next_char) || (is_limiter(next_char) && next_char != '_')) {
            break;
        }

        // Read the current char
        char current = static_cast<char>(stream.get());
        if (current == -1) {
            break;
        }

        // If the current char is not alphanumeric, underscore or dot ( used to call nested methods and variables ),
        // raise an exception
        if (isalpha(current) == 0 && isdigit(current) == 0 && current != '.' && current != '_') {
            throw LexerException(this->line_number, this->origin,
                                 "Identifier can only contain alphanumeric characters.");
        }

        this->buffer.push_back(current);
    }

    this->buffer.push_back(0);

    found = false;
    if (target_suffix != -1 && stream.peek() == target_suffix) {
        stream.get();
        found = true;
    }

    trailing_spaces = ignore_spaces();

    return std::move(std::string{buffer.data()});
}

int ninx::lexer::Reader::get_line_number() const {
    return line_number;
}

void ninx::lexer::Reader::increment_line() {
    this->line_number++;
}


