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

set(PROJECT_HEADERS
        src/utils/file-picker/file-picker.h
        src/utils/codec-interface.h
        src/crypto/symmetric-encryption.h
        src/services/encryption/symmetric-encryption-service.h
        src/utils/salt-generator/salt-generator.h
        src/utils/base64/base64.h
        src/crypto/aes/aes-gcm/aes-gcm.h
)

set(PROJECT_SOURCES
        src/utils/file-picker/file-picker.cpp
        src/services/encryption/symmetric-encryption-service.cpp
        src/utils/salt-generator/salt-generator.cpp
        src/utils/base64/base64.cpp
        src/crypto/aes/aes-gcm/aes-gcm.cpp
        src/main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE Poco::Crypto Slint::Slint)
slint_target_sources(${PROJECT_NAME} ui/app.slint)
target_include_directories(${PROJECT_NAME} PRIVATE
        src/utils/file-picker
        src/crypto
        src/crypto/aes/aes-gcm
        src/services/encryption
        src/utils
        src/utils/base64
        src/utils/salt-generator
)