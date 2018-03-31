cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s -flto -fPIC")

file(GLOB_RECURSE SRC
	*.cpp
	*.h
)

add_library(RakNet STATIC ${SRC})
