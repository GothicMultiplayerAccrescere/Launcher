cmake_minimum_required(VERSION 3.3)

# project name
project(GothicMultiplayerLauncher)
# executable name
set(TARGET_NAME gml)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${Qt5Network_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(lib/RakNet)

# the following line provides the
# different build types
# BEWARE that if you change something
# it also has to be changed further down...
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

# search for dependency libraries
# find_library(QT Qt)

# this block may add some buildflags
# for the different defined compilers
# extend this for more build systems later on
if (MSVC)
    # VISUAL STUDIO
    # fill the build definitions for the MSVC build subsystem in here
    set_property (GLOBAL PROPERTY USE_FOLDERS ON)
    set(CMAKE_CXX_FLAGS " /MD /MP /EHsc /DNOMINMAX /D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS_RELEASE " /DNDEBUG /Ox /Ob2 /Oi /Ot /Oy /fp:fast /GF /FD /MT /GS- /GR- /utf-8")
    set(CMAKE_CXX_FLAGS_DEBUG "/GR- /Zi /Od /DCOMPILE_WITH_LOGS")
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    set(SOCKET_LIB "wsock32 ws2_32")
else()
    # MINGW
    # fill the build definitions for the MinGW build subsystem in here
    set(CMAKE_CXX_FLAGS " -std=c++14 -fpermissive -static-libstdc++ -static-libgcc -Wall")
    set(CMAKE_CXX_FLAGS_RELEASE " -O3 -s -Wclobbered -Wignored-qualifiers -Wuninitialized -Wold-style-cast -Wreorder -Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-function -fno-rtti -fno-jump-tables -ffunction-sections -fdata-sections -fstack-protector-strong -Wl,--gc-sections -Wl,--strip-all")
    set(CMAKE_CXX_FLAGS_DEBUG " -g -Wold-style-cast -DCOMPILE_WITH_LOGS")
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
endif()


# define the include directories
include_directories(
    "${CMAKE_SOURCE_DIR}/include"
)

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
    resource/*.*
)

# readme for IDE recognition
set(MISC
    README.md
)

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# set up build pathes and add the executable definition
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

add_executable(${TARGET_NAME} ${INCLUDE_FILES} ${SRC_FILES} ${UI_HEADERS} ${RES_FILES} ${MISC})

add_dependencies(${TARGET_NAME} RakNet)

target_link_libraries(${TARGET_NAME} Qt5::Widgets ${CMAKE_THREAD_LIBS_INIT} RakNet ${SOCKET_LIB})
