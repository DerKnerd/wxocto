cmake_minimum_required(VERSION 3.16)
project(wxocto CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(building/cmake/ucm.cmake)
include(./conan.cmake)

ucm_set_runtime(STATIC)
if (WIN32 OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    add_compile_definitions(__WXMSW__ _UNICODE)
    set(BUILD_SHARED_LIBS false)
    set(CMAKE_SYSTEM_NAME Windows)
    set(CMAKE_SYSTEM_PROCESSOR x86_64)
    set(CMAKE_WIN32_EXECUTABLE 1)
    set(CMAKE_CXX_COMPILER_WORKS TRUE)
    set(CMAKE_C_COMPILER_WORKS TRUE)
endif ()

set(wxUSE_STL ON)
set(wxUSE_OPENGL OFF)
set(wxBUILD_SHARED OFF)
set(wxUSE_ZLIB builtin)
set(wxUSE_EXPAT builtin)
set(wxUSE_LIBJPEG builtin)
set(wxUSE_LIBPNG builtin)
set(wxUSE_LIBTIFF builtin)
add_subdirectory(libs/wxWidgets)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(./conan.cmake)

conan_add_remote(NAME conan.imanuel.dev
        URL https://conan.imanuel.dev/artifactory/api/conan/conan-virtual)

conan_cmake_configure(
        REQUIRES
        nlohmann_json/3.10.2
        cpp-httplib/0.9.7
        GENERATORS cmake_find_package
)

if (MINGW)
    conan_cmake_install(PATH_OR_REFERENCE .
            BUILD missing
            REMOTE conan.imanuel.dev
            PROFILE ${CMAKE_SOURCE_DIR}/building/conan/linux_to_win64_conan
            SETTINGS ${CONAN_SETTINGS}
            OPTIONS ${options})
else ()
    conan_cmake_autodetect(settings)
    conan_cmake_install(PATH_OR_REFERENCE .
            BUILD missing
            REMOTE conan.imanuel.dev
            SETTINGS ${settings}
            SETTINGS ${CONAN_SETTINGS}
            OPTIONS ${options})
endif ()
conan_cmake_autodetect(settings)

find_package(nlohmann_json)
find_package(httplib)

file(GLOB SOURCES mainwindow/*.cpp octoprint/*.cpp octoprint/spoolmanager/*.cpp spoolmanager/*.cpp settings/*.cpp)

add_executable(wxocto ${target} main.rc main.cpp MainApp.h MainApp.cpp ${SOURCES})
target_link_libraries(wxocto PRIVATE wx::base wx::core wx::net httplib::httplib nlohmann_json::nlohmann_json)