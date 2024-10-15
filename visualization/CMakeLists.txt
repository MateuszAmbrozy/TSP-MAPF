cmake_minimum_required(VERSION 3.5)

project(visualization VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/../src")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/../lib")

set(PROJECT_SOURCES
        main.cpp
        mainwindow.ui
)

file(GLOB SRC_FILES "${SRC_DIR}/*.cpp" "${SRC_DIR}/Space-Time-AStar/*.cpp" "${SRC_DIR}/AStar/*.cpp")
file(GLOB HEADER_FILES "${LIB_DIR}/*.h" "${LIB_DIR}/Space-Time-AStar/*.h" "${LIB_DIR}/AStar/*.h")

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(visualization
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${SRC_FILES}
        ${HEADER_FILES}
        mapfscene.h mapfscene.cpp
        graphrectitem.h graphrectitem.cpp
        view.h view.cpp
        agentrectitem.h agentrectitem.cpp
    )
else()
    if(ANDROID)
        add_library(visualization SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(visualization
            ${PROJECT_SOURCES}
            ${SRC_FILES}
            ${HEADER_FILES}
        )
    endif()
endif()

target_link_libraries(visualization PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.visualization)
endif()
set_target_properties(visualization PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS visualization
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(visualization)
endif()
