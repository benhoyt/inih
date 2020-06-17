

file(GLOB SRCS "*.c")

foreach(SRC ${SRCS}) 
message(STATUS "SRC ${SRC}")
    string(REGEX REPLACE ".c$" "" TAG ${SRC})
    string(REGEX REPLACE ".*/" "" TAG ${TAG})
    message(STATUS "TAG ${TAG}")
    add_executable(${TAG} ${SRC})
    target_link_libraries(${TAG} inih)
endforeach(SRC ${SRCS}) 

add_executable(INIReaderExample INIReaderExample.cpp)
target_link_libraries(INIReaderExample INIReader)

add_test(NAME test_inih COMMAND ./examples/INIReaderExample
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../
)
