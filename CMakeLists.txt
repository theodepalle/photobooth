cmake_minimum_required(VERSION 3.16)
project(photobooth)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_compile_options(-Wall -Wextra -Werror=return-type)

find_library(GPHOTO2_LIB gphoto2 REQUIRED)
find_library(GPHOTO2_PORT_LIB gphoto2_port REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

# BUILD
include_directories(include)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
file(GLOB_RECURSE INCLUDE_FILES "include/*.h")

add_executable(photobooth ${SRC_FILES} ${INCLUDE_FILES})

target_link_libraries(photobooth
    ${GPHOTO2_LIB} ${GPHOTO2_PORT_LIB}
    Qt::Core Qt::Gui Qt::Qml Qt::Quick
)

set(qml_resource_files
    "ui/assets/camera-capture-icon.png"
    "ui/assets/touch-icon.png"
    "ui/assets/smile.png"
    "ui/assets/countdown/1.png"
    "ui/assets/countdown/2.png"
    "ui/assets/countdown/3.png"
    "ui/assets/countdown/4.png"
    "ui/assets/countdown/5.png"
    "ui/assets/right.png"
    "ui/assets/wrong.png"
    "ui/assets/fonts/Dancing_script/DancingScript-VariableFont_wght.ttf"
    "ui/main.qml"
    "ui/StandbyPage.qml"
    "ui/CapturePage.qml"
    "ui/DisplayPage.qml"
    "ui/Countdown.qml"
)

qt6_add_resources(photobooth "qml"
    PREFIX
        "/"
    FILES
        ${qml_resource_files}
)

# INSTALL
install(TARGETS photobooth DESTINATION bin)

# TESTING
