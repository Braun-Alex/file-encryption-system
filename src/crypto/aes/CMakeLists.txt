cmake_minimum_required(VERSION 3.26)
project(aes_wrapper)

set(CMAKE_CXX_STANDARD 23)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
        Poco
        URL https://github.com/pocoproject/poco/archive/refs/tags/poco-1.12.4-release.zip
)
FetchContent_MakeAvailable(Poco)

configure_file(CMakeLists.txt.googletest googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_library(aes_wrapper SHARED src/AES.cpp)

set(LIB_TEST_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/libtest.cpp
)

add_executable(libtest ${LIB_TEST_SOURCES})
target_link_libraries(libtest aes_wrapper gtest_main Poco::Crypto Poco::Util)

install(TARGETS aes_wrapper DESTINATION /usr/lib)
install(FILES include/AES.h DESTINATION include)
