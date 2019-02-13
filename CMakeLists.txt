cmake_minimum_required(VERSION 3.8)

# project name
project(GothicMultiplayerLauncher)
# executable name
set(TARGET_NAME gml)

find_package(Git REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Threads REQUIRED)

execute_process(COMMAND
    "${GIT_EXECUTABLE}" tag
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_TAG
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(COMMAND
    "${GIT_EXECUTABLE}" log -1 --pretty=format:%h --no-show-signature
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_COMMIT
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(COMMAND
    "${GIT_EXECUTABLE}" log -1 --date=format:%Y-%m-%d\ %H:%M:%S --pretty=format:%ad --no-show-signature
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE GIT_DATE
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE
)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/resource.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/src/resource.h" @ONLY)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(lib/slikenet)

# list include files to variables
file (GLOB_RECURSE INCLUDE_FILES
    LIST_DIRECTORIES false
    include/*.h
)

# list source files to variables
file (GLOB_RECURSE SRC_FILES
    src/*.cpp
    src/*.h
)

# list ui files to variables
file(GLOB_RECURSE UI_FILES
    src/*.ui
)

# list resource files to variables
file(GLOB_RECURSE RES_FILES
    resource/*.qrc
    resource/*.rc
)

# readme for IDE recognition
set(MISC
    README.md
)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)

if (MINGW OR MSVC)
	set(SOCKET_LIB wsock32 ws2_32)
endif()

add_executable(${TARGET_NAME} ${INCLUDE_FILES} ${SRC_FILES} ${UI_HEADERS} ${RES_FILES} ${MISC})

if (MSVC)
	target_compile_options(${TARGET_NAME} PRIVATE /W3 /WX /EHsc /utf-8 /D_CRT_SECURE_NO_WARNINGS)
    set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS " /MANIFESTUAC:\"level='requireAdministrator'\" /SUBSYSTEM:windows /ENTRY:mainCRTStartup")
else()
	target_compile_options(${TARGET_NAME} PRIVATE -std=c++14 -fpermissive -static-libstdc++ -static-libgcc -Wall -Wextra -O3 -s -Wclobbered -Wignored-qualifiers -Wuninitialized -Wold-style-cast -Wreorder -Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-function -fno-rtti -fno-jump-tables -ffunction-sections -fdata-sections -fstack-protector-strong -Wl,--gc-sections -Wl,--strip-all)
endif()

target_link_libraries(${TARGET_NAME} Qt5::Widgets ${CMAKE_THREAD_LIBS_INIT} RakNetLibStatic ${SOCKET_LIB})
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/slikenet/Source/include ${CMAKE_CURRENT_SOURCE_DIR} ${Qt5Widgets_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${Qt5Network_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS})
