cmake_minimum_required(VERSION 2.8.4)
SET(CMAKE_AUTOMOC ON)

project("Clarus")
INCLUDE(../clews/Clews.cmake)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

clews_add_qt_resources()
clews_add_executable()

clews_target_link_libraries(Qt5::Core)
clews_target_link_libraries(Qt5::Widgets)
clews_target_link_libraries(Qt5::Network)

