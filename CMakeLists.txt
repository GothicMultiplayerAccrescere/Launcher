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
find_package(Qt5Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR} ${Qt5Network_INCLUDE_DIRS} ${Qt5Multimedia_INCLUDE_DIRS})

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
    )

# list ui files to variables
file(GLOB_RECURSE UI_FILES
    src/*.ui
    )

set(RC_FILE
    resource/resource.qrc
    )

set(MISC
    README.md
    )

# set up filter groups for the file lists
if (MSVC)
    foreach(_source IN ITEMS ${INCLUDE_FILES})
        get_filename_component(_source_path "${_source}" PATH)
        file(RELATIVE_PATH _source_path_rel "${CMAKE_SOURCE_DIR}" "${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
else()
    source_group("includes" FILES ${INCLUDE_FILES})
endif()

source_group("source" FILES ${SRC_FILES})

# create RES_FILES variable
set(RES_FILES "")
# compile the resource file for windows
# if this is the specified platform (by compiler)
if (MSVC OR MINGW)
    set(RES_FILES "resource/resource.rc")
    set(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
endif()

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# set up build pathes and add the executable definition
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)
add_executable(${TARGET_NAME} ${INCLUDE_FILES} ${SRC_FILES} ${UI_HEADERS} ${RC_FILE} ${RES_FILES} ${MISC})

target_link_libraries(${TARGET_NAME} Qt5::Widgets Qt5::Network)

# FIXME: nasty workaround here because MSVC didn't recognize the subsystem else
if (WIN32)
    if (MSVC)
        set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
        set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
    elseif(MINGW)
    endif()
endif()

# link additional libraries to the target
if (MINGW)
    # FIXME: merge this with MSVC linking if available in the future
    #target_link_libraries(${TARGET_NAME} ${QT})
elseif()
    # FIXME: use proper library check if available later on (currently crashing MSVC)
    #target_link_libraries(${TARGET_NAME} QT)
endif()
