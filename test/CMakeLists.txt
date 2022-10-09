set(${PROJECT_ID}_TEST_SOURCES
        ../src/functionA.cpp
        main.cpp
)

add_executable(${PROJECT_ID}-test
)

target_sources(${PROJECT_ID}-test
        PRIVATE
            ${${PROJECT_ID}_TEST_SOURCES}
)

target_include_directories(${PROJECT_ID}-test
        PRIVATE
            ../include
)