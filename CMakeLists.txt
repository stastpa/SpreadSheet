cmake_minimum_required(VERSION 3.28)
project(BIG)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(BIG
        expression.h
        ExpressionBuilder.cpp
        ExpressionBuilder.h
        test.cpp
        CSpreadsheet.h
        CSpreadsheet.cpp
        CPos.h
        CPos.cpp
        Node.h
        Node.cpp)

target_link_libraries(BIG ${CMAKE_SOURCE_DIR}/libexpression_parser.a)