cmake_minimum_required(VERSION 2.8.9)
project(inih)

include(GNUInstallDirs)

file(GLOB SOURCES "*.c")
file(GLOB HEADERS "*.h")

add_library(inih SHARED ${SOURCES})
set_property(TARGET inih PROPERTY VERSION "1.0.0")
set_property(TARGET inih PROPERTY SOVERSION 1 )

install(TARGETS inih DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(FILES ${HEADERS} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
