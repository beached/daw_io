cmake_minimum_required(VERSION 3.16)
project(daw-io
        VERSION "0.1.0"
        DESCRIPTION "IO Abstractions for C++"
        HOMEPAGE_URL "https://github.com/beached/daw_io"
        LANGUAGES C CXX
        )

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

option(DAW_USE_PACKAGE_MANAGEMENT "Do not use FetchContent and assume dependencies are installed" OFF)
if (DAW_USE_PACKAGE_MANAGEMENT)
    find_package(daw-header-libraries CONFIG REQUIRED)
    find_package(fmt CONFIG REQUIRED)
else ()
    add_subdirectory(extern)
endif ()

include_directories("${CMAKE_SOURCE_DIR}/include")


set(SOURCE_FOLDER src)

add_library(daw-io-base
        ${SOURCE_FOLDER}/daw/io/daw_output_stream.cpp
        ${SOURCE_FOLDER}/daw/io/daw_io_exception.cpp
        ${SOURCE_FOLDER}/daw/io/daw_output_stream_cfile.cpp
        ${SOURCE_FOLDER}/daw/io/daw_output_stream_console.cpp
        # ${SOURCE_FOLDER}/daw/io/input_stream.cpp
        )
target_compile_features(daw-io-base INTERFACE cxx_std_20)
target_link_libraries(daw-io-base
        PUBLIC daw::daw-header-libraries
        fmt::fmt
        )

add_executable(daw-io-base-test tests/src/daw_io_base_test.cpp)
target_link_libraries(daw-io-base-test daw-io-base)
