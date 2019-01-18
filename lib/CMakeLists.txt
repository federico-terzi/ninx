add_library(ninxlib STATIC lexer/Lexer.cpp lexer/Lexer.h
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
        parser/element/FunctionDefinition.cpp parser/element/FunctionDefinition.h
        parser/element/FunctionArgument.cpp parser/element/FunctionArgument.h
        parser/element/FunctionCallArgument.cpp parser/element/FunctionCallArgument.h
        evaluator/Evaluator.h
        evaluator/DefaultEvaluator.cpp evaluator/DefaultEvaluator.h
        evaluator/exception/RuntimeException.cpp evaluator/exception/RuntimeException.h parser/element/Expression.cpp parser/element/Expression.h parser/element/AddExpression.cpp parser/element/AddExpression.h parser/element/BinaryExpression.cpp parser/element/BinaryExpression.h parser/element/SubtractExpression.cpp parser/element/SubtractExpression.h parser/element/MultiplicationExpression.cpp parser/element/MultiplicationExpression.h parser/element/DivisionExpression.cpp parser/element/DivisionExpression.h)

target_include_directories (ninxlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ninxlib ${Boost_LIBRARIES})