cmake_minimum_required(VERSION 3.20)
project(wxocto CXX)

set(CMAKE_CXX_STANDARD 23)

# Request the required wxWidgets libs
find_package(wxWidgets 3.1 COMPONENTS all REQUIRED)

# Include the wxWidgets use file to initialize various settings
include(${wxWidgets_USE_FILE})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.16.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            EXPECTED_HASH SHA256=396e16d0f5eabdc6a14afddbcfff62a54a7ee75c6da23f32f7a31bc85db23484
            TLS_VERIFY OFF)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(
    REQUIRES
    openssl/1.1.1k
    websocketpp/0.8.2
    easyhttpcpp/2.1.0
    nlohmann_json/3.10.2
    GENERATORS cmake_find_package
)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings})

find_package(websocketpp)
find_package(easyhttpcppeasyhttp)
find_package(nlohmann_json)

add_executable(wxocto main.cpp MainWindowBase.cpp MainWindowBase.h MainWindow.cpp MainWindow.h)
target_link_libraries(wxocto ${wxWidgets_LIBRARIES} websocketpp::websocketpp easyhttpcppeasyhttp::easyhttpcppeasyhttp nlohmann_json::nlohmann_json)