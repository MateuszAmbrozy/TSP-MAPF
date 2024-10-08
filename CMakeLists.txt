cmake_minimum_required(VERSION 3.8)

set(This Example)

project(${This} C CXX)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Ustawienie jednolitego runtime dla całego projektu
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")

enable_testing()

add_subdirectory(googletest)

file(GLOB_RECURSE Headers lib/*.hpp)
#file(GLOB_RECURSE Headers lib/Space-Time-AStar/*.hpp)
file(GLOB_RECURSE Sources src/*.cpp)
#file(GLOB_RECURSE Sources lib/Space-Time-AStar/*.cpp)

# Przypisanie znalezionych plików do zmiennych
set(Headers ${Headers})
set(Sources ${Sources})

# Dodanie biblioteki
add_library(${This} STATIC ${Sources} ${Headers})

# Dodanie pliku wykonywalnego
add_executable(${This}_app main.cpp)

# Linkowanie pliku wykonywalnego z biblioteką
target_link_libraries(${This}_app PRIVATE ${This})

add_subdirectory(gtest)
