PROJECT(Searcher)
CMAKE_MINIMUM_REQUIRED (VERSION 3.5)

INCLUDE_DIRECTORIES(..Tokenizer ../Tokenizer/include ..Tokenizer/dict)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "/usr/local/cppjieba" CACHE PATH "default install path" FORCE )
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES test.cpp)
add_executable(gs_searcher ${SOURCE_FILES})
