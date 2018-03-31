cmake_minimum_required(VERSION 3.3)

file(GLOB_RECURSE SRC
	*.cpp
	*.h
)

add_library(RakNet STATIC ${SRC})
