cmake_minimum_required(VERSION 2.8.2)

# Set variables
set(PROJECT_NAME prototype)
# Cannot have two different targests with the same names.
# After adding lib/exe I will set their output name to #{PROJECT_NAME} without any suffix.
set(LIB_TARGET ${PROJECT_NAME}_lib)
set(EXE_TARGET ${PROJECT_NAME}_exe)

project(${PROJECT_NAME})

# Set compiler options
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
if(NOT MSVC)
    add_definitions( -Wall -Wextra -Wpedantic -g3 -gdwarf-4)
endif()
enable_language(CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_VERBOSE_MAKEFILE TRUE)


#---------------------------------------------------------
# Main
#---------------------------------------------------------

find_package(OpenCV REQUIRED)

set(MAIN_HEADERS
    # NOTE
    include/CMT.h
)

set(MAIN_SOURCES
    # NOTE
    src/CMT.cpp
)

# Put executable files in bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# Put libs in lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)

# Create static library with all files from src except main.cpp
add_library(${LIB_TARGET} ${MAIN_SOURCES} ${MAIN_HEADERS})
target_link_libraries(${LIB_TARGET}
    # NOTE
    ${OpenCV_LIBS}
)


set_target_properties(${LIB_TARGET} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# Create executable and link with library
add_executable(${EXE_TARGET} src/main.cpp)
target_link_libraries(${EXE_TARGET} ${LIB_TARGET})
set_target_properties(${EXE_TARGET} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})


