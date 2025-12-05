cmake_minimum_required(VERSION 3.0)

project(circ_log)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(absl CONFIG REQUIRED)

list(APPEND SRCS main.cpp)
list(APPEND PROJ_DEP fmt)
list(APPEND PROJ_DEP absl::time)

list(APPEND INC_PATH "$ENV{HOME}/.local/fmt/include")
list(APPEND INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third-party/")
list(APPEND INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/inc/")
list(APPEND LIB_PATH "$ENV{HOME}/.local/fmt/lib/")

message(AUTHOR_WARNING "inc path: ${INC_PATH}")
message(AUTHOR_WARNING "lib path: ${LIB_PATH}")

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJ_DEP})
target_link_directories(${PROJECT_NAME} PRIVATE ${LIB_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_PATH})

add_custom_target(run
    DEPENDS ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
