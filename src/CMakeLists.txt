set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_library(
    ${PROJECT_NAME}Lib STATIC
    NoiseFilter.cpp
    RandomGen.cpp
)

target_include_directories(${PROJECT_NAME}Lib PRIVATE ../include)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ../include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)