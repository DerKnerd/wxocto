cmake_minimum_required(VERSION 3.20)
project(wxocto CXX)

set(CMAKE_CXX_STANDARD 23)

include(./ucm.cmake)

ucm_set_runtime(STATIC)
if (WIN32 OR MINGW)
    set(target WIN32)
    set(options Poco:enable_netssl_win=True Poco:enable_netssl=False)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(BUILD_SHARED_LIBS false)
    set(CONAN_SETTINGS compiler.runtime=MT)
endif ()

# Request the required wxWidgets libs
find_package(wxWidgets 3.0 REQUIRED)

# Include the wxWidgets use file to initialize various settings
include(${wxWidgets_USE_FILE})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(./conan.cmake)

conan_cmake_configure(
        REQUIRES
        nlohmann_json/3.10.2
        expat/2.4.1
        cpp-httplib/0.9.7
        GENERATORS cmake_find_package
)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings}
        SETTINGS ${CONAN_SETTINGS}
        OPTIONS ${options})

find_package(expat)
find_package(nlohmann_json)
find_package(httplib)

file(GLOB SOURCES mainwindow/*.cpp octoprint/*.cpp octoprint/spoolmanager/*.cpp spoolmanager/*.cpp settings/*.cpp)

add_executable(wxocto ${target} main.rc main.cpp MainApp.h MainApp.cpp ${SOURCES})
target_include_directories(wxocto PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(wxocto PRIVATE httplib::httplib expat::expat nlohmann_json::nlohmann_json ${wxWidgets_LIBRARIES})