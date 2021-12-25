cmake_minimum_required(VERSION 3.0.0)
project(compiler_construction VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(compiler_construction 
	src/main.cpp
	src/lexer/lexer.cpp
	src/translator/translator.cpp
	src/gen/gen.cpp
	src/vm/vm.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
