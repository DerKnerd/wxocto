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

add_executable(wxocto main.cpp mainwindow/MainWindowBase.cpp mainwindow/MainWindowBase.h mainwindow/MainWindow.cpp mainwindow/MainWindow.h MainApp.cpp MainApp.h settings/wxOctoSettings.h settings/OctoPrintServerSettingsPage.cpp settings/OctoPrintServerSettingsPage.h settings/OctoPrintServerSettingsPagePanel.cpp settings/OctoPrintServerSettingsPagePanel.h helper.h octoprint/OctoprintFile.cpp octoprint/OctoprintFile.h octoprint/FetchFilesThread.cpp octoprint/FetchFilesThread.h octoprint/OctoprintJob.cpp octoprint/OctoprintJob.h octoprint/FetchPrintStatusThread.cpp octoprint/FetchPrintStatusThread.h octoprint/OctoApiEventIds.h octoprint/SelectAndPrintFileThread.cpp octoprint/SelectAndPrintFileThread.h octoprint/CancelPrintThread.cpp octoprint/CancelPrintThread.h octoprint/PausePrintThread.cpp octoprint/PausePrintThread.h octoprint/ResumePrintThread.cpp octoprint/ResumePrintThread.h)
target_link_libraries(wxocto ${wxWidgets_LIBRARIES} websocketpp::websocketpp easyhttpcppeasyhttp::easyhttpcppeasyhttp nlohmann_json::nlohmann_json)
