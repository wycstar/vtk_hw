cmake_minimum_required(VERSION 3.5)

project(vtk_hw LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_PREFIX_PATH "D:/lang/qt/5.15.1/msvc2019_64")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)
find_package(VTK REQUIRED)
find_package(CUDA REQUIRED)
if (CUDA_FOUND)
    message("CUDA found!")
else()
    message("CUDA not found, doing something alternatively")
endif()

link_directories(
        D:/lang/vtk/lib
        D:/lang/vtk/bin
)

include_directories(D:/lang/vtk/include/vtk-9.0)

set(TS_FILES vtk_hw_zh_CN.ts)

if(ANDROID)
  add_library(vtk_hw SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(vtk_hw
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
endif()

target_link_libraries(vtk_hw PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${VTK_LIBRARIES})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
