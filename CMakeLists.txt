cmake_minimum_required(VERSION 3.17)
project(course_work)

set(CMAKE_CXX_STANDARD 14)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5Widgets)

add_executable(course_work
    main.cpp
    MainWindow.cpp MainWindow.h
    RepairPlan.cpp RepairPlan.h)
target_link_libraries(course_work Qt5::Core Qt5::Widgets)
