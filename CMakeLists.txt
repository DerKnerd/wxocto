cmake_minimum_required(VERSION 3.20)
project(wxocto CXX)

set(CMAKE_CXX_STANDARD 23)

# Request the required wxWidgets libs
find_package(wxWidgets 3.1 REQUIRED)

# Include the wxWidgets use file to initialize various settings
include(${wxWidgets_USE_FILE})

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(./conan.cmake)

conan_cmake_configure(
        REQUIRES
        easyhttpcpp/2.1.0
        nlohmann_json/3.10.2
        expat/2.4.1
        GENERATORS cmake_find_package
)

conan_cmake_autodetect(settings)

if (WIN32 OR MINGW)
    set(target WIN32 octoprint/spoolmanager/SaveSpoolThread.cpp octoprint/spoolmanager/SaveSpoolThread.h)
    set(options Poco:enable_netssl_win=True Poco:enable_netssl=False)
endif ()

conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings}
        OPTIONS ${options})

find_package(expat)
find_package(easyhttpcppeasyhttp)
find_package(nlohmann_json)

file(GLOB SOURCES mainwindow/*.cpp octoprint/*.cpp octoprint/spoolmanager/*.cpp spoolmanager/*.cpp settings/*.cpp)

add_executable(wxocto ${target} main.cpp MainApp.h MainApp.cpp ${SOURCES})
target_include_directories(wxocto PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(wxocto expat::expat ${wxWidgets_LIBRARIES} easyhttpcppeasyhttp::easyhttpcppeasyhttp nlohmann_json::nlohmann_json)
