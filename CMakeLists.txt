cmake_minimum_required(VERSION 3.21)
project(file-encryption-system LANGUAGES C CXX)

find_package(Poco REQUIRED COMPONENTS Crypto)

if (NOT Poco_FOUND)
  message("Poco could not be located in the CMake module search path. Downloading it from Git and building it locally")
  include(FetchContent)

  FetchContent_Declare(
          Poco
          GIT_REPOSITORY https://github.com/pocoproject/poco.git
  )

  FetchContent_MakeAvailable(Poco)
endif (NOT Poco_FOUND)

find_package(Slint QUIET)

if (NOT Slint_FOUND)
  message("Slint could not be located in the CMake module search path. Downloading it from Git and building it locally")
  include(FetchContent)

  FetchContent_Declare(
    Slint
    GIT_REPOSITORY https://github.com/slint-ui/slint.git
    GIT_TAG release/1
    SOURCE_SUBDIR api/cpp
  )

  FetchContent_MakeAvailable(Slint)
endif (NOT Slint_FOUND)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

configure_file(CMakeLists.txt.googletest googletest-download/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" . RESULT_VARIABLE result WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} --build . RESULT_VARIABLE result WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src ${CMAKE_CURRENT_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)

if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

set(PROJECT_HEADERS
        src/utils/pickers/pickers.h
        src/crypto/symmetric-encryption.h
        src/services/encryption/symmetric-encryption-service.h
        src/utils/salt-generator/salt-generator.h
        src/utils/passphrase-checker/passphrase-checker.h
        src/utils/base64/base64.h
        src/crypto/kdf/kdf.h
        src/crypto/aes/aes-gcm/aes-gcm.h
)

set(PROJECT_SOURCES
        src/utils/pickers/pickers.cpp
        src/services/encryption/symmetric-encryption-service.cpp
        src/utils/salt-generator/salt-generator.cpp
        src/utils/passphrase-checker/passphrase-checker.cpp
        src/utils/base64/base64.cpp
        src/crypto/kdf/kdf.cpp
        src/crypto/aes/aes-gcm/aes-gcm.cpp
        src/main.cpp
)

set(UNIT_TESTS
        tests/unit/salt-generator/salt-generator-tests.cpp
        tests/unit/kdf/kdf-tests.cpp
        tests/unit/base64/base64-tests.cpp
        tests/unit/passphrase-checker/passphrase-checker-tests.cpp
        tests/unit/aes/aes-tests.cpp
)

set(UNIT_TEST_HEADERS
        src/utils/salt-generator/salt-generator.h
        src/crypto/kdf/kdf.h
        src/utils/base64/base64.h
        src/utils/passphrase-checker/passphrase-checker.h
        src/crypto/symmetric-encryption.h
        src/services/encryption/symmetric-encryption-service.h

)

set(UNIT_TEST_SOURCES
        src/utils/salt-generator/salt-generator.cpp
        src/crypto/kdf/kdf.cpp
        src/utils/base64/base64.cpp
        src/utils/passphrase-checker/passphrase-checker.cpp
        src/services/encryption/symmetric-encryption-service.cpp
        src/crypto/aes/aes-gcm/aes-gcm.cpp
)

enable_testing()

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})
add_executable(unit_tests ${UNIT_TESTS} ${UNIT_TEST_HEADERS} ${UNIT_TEST_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Poco::Crypto Slint::Slint)
target_link_libraries(unit_tests PRIVATE gtest_main gtest gmock Poco::Crypto)

slint_target_sources(${PROJECT_NAME} ui/app.slint)

target_include_directories(unit_tests PRIVATE
        src/utils/pickers
        src/crypto
        src/crypto/kdf
        src/crypto/aes/aes-gcm
        src/services/encryption
        src/utils
        src/utils/base64
        src/utils/passphrase-checker
        src/utils/salt-generator
)
target_include_directories(${PROJECT_NAME} PRIVATE
        src/utils/pickers
        src/crypto
        src/crypto/kdf
        src/crypto/aes/aes-gcm
        src/services/encryption
        src/utils
        src/utils/base64
        src/utils/passphrase-checker
        src/utils/salt-generator
)

add_test(NAME UnitTests COMMAND unit_tests)