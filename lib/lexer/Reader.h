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


#ifndef NINX_CONTEXT_H
#define NINX_CONTEXT_H

#include <string>
#include <iostream>
#include <vector>

namespace ninx {
    namespace lexer {
        const int READER_BUFFER_INITIAL_SIZE = 500;

        // Chars that have a special meaning, such ascan be used in operator definition and the @ keyword prefix.
        // NOTE: place them based on the priority and on probability of occurrence.
        //       The first ones are the most used.
        const char LIMITER_CHARS[] = {'@', '{', '}', '*', '_', '$', '=', '(', ')', ',', '<', '>', '\\', '/', '+', '-'};

        const int MAX_CONSECUTIVE_NEWLINES = 2;

        class Reader {
        private:
            std::istream &stream;
            std::string origin;

            std::vector<char> buffer;

            int line_number = 1;

            void increment_line();
        public:
            explicit Reader(std::istream &stream, std::string &origin);

            int get_line_number() const;

            void ignore_spaces();
            bool ignore_comment();


            static const int FOUND_EOF = -1;
            static const int FOUND_TEXT = -2;
            int get_next_limiter();


            std::string read_until_limiter();
            std::string read_identifier();
        };
    }
}

#endif //NINX_CONTEXT_H
