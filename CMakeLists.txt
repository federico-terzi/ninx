cmake_minimum_required(VERSION 3.12)
project(ninx)

set(CMAKE_GENERATOR_PLATFORM x64)

set(CMAKE_CXX_STANDARD 14)

add_executable(ninx main.cpp lexer/Lexer.cpp lexer/Lexer.h
        lexer/token/Token.h
        lexer/Reader.cpp lexer/Reader.h
        lexer/token/Type.h
        lexer/token/Text.cpp lexer/token/Text.h
        lexer/token/Limiter.cpp lexer/token/Limiter.h
        lexer/token/Variable.cpp lexer/token/Variable.h
        lexer/token/Keyword.cpp lexer/token/Keyword.h
        lexer/exception/LexerException.cpp lexer/exception/LexerException.h
        parser/Parser.cpp parser/Parser.h
        parser/TokenReader.cpp parser/TokenReader.h
        parser/exception/ParserException.cpp parser/exception/ParserException.h
        parser/element/ASTElement.h
        parser/element/Block.cpp parser/element/Block.h
        parser/element/TextElement.cpp parser/element/TextElement.h
        parser/element/Statement.h
        parser/element/FunctionCall.cpp parser/element/FunctionCall.h
        parser/element/VariableRead.cpp parser/element/VariableRead.h
        parser/element/Assignment.cpp parser/element/Assignment.h
        evaluator/Evaluator.h
        evaluator/DefaultEvaluator.cpp evaluator/DefaultEvaluator.h
        evaluator/exception/VariableNotFoundException.cpp evaluator/exception/VariableNotFoundException.h parser/element/FunctionDefinition.cpp parser/element/FunctionDefinition.h parser/element/FunctionArgument.cpp parser/element/FunctionArgument.h)

file(COPY test DESTINATION .)

#set(Boost_DEBUG 1)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()

set (Boost_USE_STATIC_LIBS ON) # enable static linking
set (Boost_USE_MULTITHREAD ON)  # enable multithreading
set (Boost_ARCHITECTURE "-x64")

find_package (Boost COMPONENTS REQUIRED regex)

target_link_libraries (ninx ${Boost_LIBRARIES})