cmake_minimum_required(VERSION 2.8)
include(FindPkgConfig)

PROJECT(fontpreview)

SET(fontpreview_SOURCES main.cpp FontPreview.cpp)
SET(fontpreview_HEADERS FontPreview.h)


set(CMAKE_CXX_FLAGS "-g -Wall")

pkg_check_modules(FT2 REQUIRED freetype2)
INCLUDE_DIRECTORIES(${FT2_INCLUDE_DIRS})
LINK_DIRECTORIES(${FT2_LIBRARY_DIRS})
FIND_PACKAGE(Qt5Widgets)
ADD_EXECUTABLE(fontpreview ${fontpreview_SOURCES})
TARGET_LINK_LIBRARIES(fontpreview Qt5::Widgets ${FT2_LIBRARIES})
