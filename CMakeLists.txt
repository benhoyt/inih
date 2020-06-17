cmake_minimum_required(VERSION 3.15)
project(INIH)


add_library(inih ini.c)

add_library(INIReader ini.c cpp/INIReader.cpp)

enable_testing()
add_subdirectory(examples)

add_test(NAME test_inihc 
COMMAND bash unittest.sh
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

if(UNIX)
    include(GNUInstallDirs)
    set(includedir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})
    set(libdir ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
    set(prefix ${CMAKE_INSTALL_PREFIX})
    configure_file(inih.pc.in inih.pc @ONLY)
    configure_file(INIReader.pc.in INIReader.pc @ONLY)

    install(TARGETS inih INIReader
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    # message(STATUS "lib:${CMAKE_INSTALL_FULL_LIBDIR}:${CMAKE_INSTALL_BINDIR}")
    install(FILES ${PROJECT_BINARY_DIR}/inih.pc ${PROJECT_BINARY_DIR}/INIReader.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
    )

    install(FILES ini.h cpp/INIReader.h
        TYPE INCLUDE
    )
endif()

if(WIN32)
    install(FILES ini.h cpp/INIReader.h
        TYPE INCLUDE
    )
    install(TARGETS inih INIReader
          RUNTIME DESTINATION bin/$<CONFIG>
          LIBRARY DESTINATION lib/$<CONFIG>
          ARCHIVE DESTINATION lib/$<CONFIG>
    )
endif()