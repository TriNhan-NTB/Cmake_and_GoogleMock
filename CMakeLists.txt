cmake_minimum_required(VERSION 3.22)

#set the project name
set(PROJECT_NAME "PROJECT_TEST")

# set the PROJECT_ID which will be the target name by changing every
# non-alphanumeric character in PROJECT_NAME to an underscore
string(MAKE_C_IDENTIFIER "${PROJECT_NAME}" PROJECT_ID)
# i.e. CPP Tutorial -> CPP_Tutorial

# change the PROJECT_ID to lower case
string(TOLOWER ${PROJECT_ID} PROJECT_ID)

#set the project for cmake
project(${PROJECT_NAME})

add_subdirectory(src)

option(BUILD_TESTS "build tests also for ${PROJECT_NAME}" OFF)

if(BUILD_TESTS)
    add_subdirectory(test)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            gtest
            gmock
    )
endif()