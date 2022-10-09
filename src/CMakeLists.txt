set(${PROJECT_ID}_SOURCES
        functionA.cpp
        main.cpp
)

add_executable(${PROJECT_ID})

target_sources(${PROJECT_ID}
        PRIVATE
            ${${PROJECT_ID}_SOURCES}
)   

target_include_directories(${PROJECT_ID}
        PRIVATE
            ../include
)

# clear variable
unset(${PROJECT_ID}_SOURCES)