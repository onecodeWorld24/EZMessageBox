project(EZMessagebox)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCE_FILES
    EZMessagebox.h
    EZMessagebox.cpp
)

add_library(EZMessagebox ${SOURCE_FILES})
target_link_libraries(EZMessagebox Qt${QT_VERSION_MAJOR}::Widgets)
