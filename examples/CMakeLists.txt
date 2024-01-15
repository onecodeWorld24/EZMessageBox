project(example)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

include_directories(
    ${CMAKE_SOURCE_DIR}
)

set(EXAMPLE
    main.cpp
    widget.cpp
    widget.h
    widget.ui
)

set(SRC_FILE_LIST
    ../src/EZMessagebox.h
    ../src/EZMessagebox.cpp
)

add_executable(example ${EXAMPLE} ${SRC_FILE_LIST})
target_link_libraries(example Qt${QT_VERSION_MAJOR}::Widgets dwmapi user32)
