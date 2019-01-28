add_executable(ninx_test Tests.cpp lexer/ReaderTest.cpp parser/ParserTest.cpp evaluation/EvaluationTest.cpp)

target_link_libraries (ninx_test ninxlib)