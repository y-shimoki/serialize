cmake_minimum_required(VERSION 4.3)

project(Serialize CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(serialize INTERFACE)

target_sources(serialize
	INTERFACE 
		src/core.hpp
)

target_compile_features(serialize
	INTERFACE
		cxx_std_23
)

target_include_directories(serialize
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}/src
)

set(FIX_INTELLISENSE_FILE_PATH "${CMAKE_CURRENT_BINARY_DIR}/fix_intellisense.cpp")
if(NOT EXISTS "${FIX_INTELLISENSE_FILE_PATH}")
	file(WRITE "${FIX_INTELLISENSE_FILE_PATH}" [[int main() { return 0; }]])
endif()

add_executable(fix_intellisense "${FIX_INTELLISENSE_FILE_PATH}")

target_link_libraries(fix_intellisense PRIVATE serialize)