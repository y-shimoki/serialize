cmake_minimum_required(VERSION 4.3)

project(Serialize CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(serialize)

target_sources(serialize
	PUBLIC
		FILE_SET CXX_MODULES
		FILES
			"src/Serialize.cpp"
)