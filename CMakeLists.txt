cmake_minimum_required(VERSION 3.12)
project(ninx)

set(CMAKE_CXX_STANDARD 14)

add_executable(ninx main.cpp lexer/Lexer.cpp lexer/Lexer.h lexer/token/Token.h lexer/Reader.cpp lexer/Reader.h lexer/token/Type.h lexer/token/Text.cpp lexer/token/Text.h)

file(COPY test DESTINATION .)