cmake_minimum_required(VERSION 3.1.0)

project(StarMon3d VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories( src )

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets 3DExtras 3DCore 3DRender REQUIRED)

add_executable(StarMon3d
    # src/StarView.cpp
    # src/StarItem.cpp
    # src/StarMap.cpp
    src/Main.cpp
)

#add_executable()

target_link_libraries(StarMon3d Qt5::Widgets Qt5::3DExtras Qt5::3DCore
	Qt5::3DRender)
