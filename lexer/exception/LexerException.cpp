//
// Created by federico.terzi2 on 07/12/2018.
//

#include <sstream>
#include "LexerException.h"

ninx::lexer::exception::LexerException::LexerException(int line, std::string &origin, const std::string &message)
        : runtime_error(
        make_message(line, origin, message)), line{line}, origin(origin) {

}

std::string
ninx::lexer::exception::LexerException::make_message(int line, const std::string &origin, const std::string &message) {
    std::stringstream s;
    s << "LexerError at line: " << line << ", " << message << std::endl;
    s << "\tfrom origin: " << origin;
    return s.str();
}
