cmake_minimum_required(VERSION 3.12)
project(ninx)

set(CMAKE_CXX_STANDARD 14)

add_executable(ninx main.cpp lexer/Lexer.cpp lexer/Lexer.h lexer/token/BaseToken.h lexer/token/TextToken.cpp lexer/token/TextToken.h lexer/Context.cpp lexer/Context.h)

file(COPY test DESTINATION .)