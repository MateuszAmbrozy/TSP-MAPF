cmake_minimum_required(VERSION 3.16)
project(TSP_MAPF VERSION 0.1.0 LANGUAGES CXX)

option(TSP_MAPF_BUILD_GUI "Build the Qt warehouse simulator" ON)

add_library(tsp_mapf
    src/AStar/Astar.cpp src/AStar/Node.cpp
    src/Agent.cpp src/BaseEnvironment.cpp
    src/CA/CooperativeA.cpp src/CA_Environment.cpp
    src/TSP.cpp src/TaskGroup.cpp src/setup.cpp
    src/WCA/WCA_Agent.cpp src/WCA/WindowCooperativeA.cpp
    src/WCA_Environment.cpp
)
target_compile_features(tsp_mapf PUBLIC cxx_std_17)
set_target_properties(tsp_mapf PROPERTIES CXX_EXTENSIONS OFF)
target_include_directories(tsp_mapf PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/lib")

add_executable(tsp_mapf_demo main.cpp)
target_link_libraries(tsp_mapf_demo PRIVATE tsp_mapf)

if(TSP_MAPF_BUILD_GUI)
    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
    file(GLOB gui_sources CONFIGURE_DEPENDS visualization_qt/*.cpp visualization_qt/graph/*.cpp)
    file(GLOB gui_headers CONFIGURE_DEPENDS visualization_qt/*.h visualization_qt/graph/*.h)
    add_executable(tsp_mapf_gui ${gui_sources} ${gui_headers} visualization_qt/default.qrc)
    set_target_properties(tsp_mapf_gui PROPERTIES AUTOMOC ON AUTORCC ON)
    target_include_directories(tsp_mapf_gui PRIVATE visualization_qt visualization_qt/graph)
    target_link_libraries(tsp_mapf_gui PRIVATE tsp_mapf Qt${QT_VERSION_MAJOR}::Widgets)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(tsp_mapf_tests tests/core_tests.cpp)
    target_link_libraries(tsp_mapf_tests PRIVATE tsp_mapf)
    add_test(NAME core_algorithms COMMAND tsp_mapf_tests)
    add_test(NAME console_demo COMMAND tsp_mapf_demo)
endif()
