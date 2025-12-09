cmake_minimum_required(VERSION 3.5)

project(circ_log)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(ABSL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/absl)
set(ABSL_BUILD_TESTING OFF CACHE BOOL "Disable Abseil tests")
set(ABSL_USE_GOOGLETEST_HEAD OFF CACHE BOOL "Disable Googletest")
message(WARNING "ABSL: ${ABSL_ROOT_DIR}")
add_subdirectory(${ABSL_ROOT_DIR})

set(FMT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/fmt)
set(FMT_TEST OFF CACHE BOOL "Disable fmt tests")
set(FMT_DOC OFF CACHE BOOL "Disable fmt documentation")
set(FMT_INSTALL ON CACHE BOOL "Enable fmt installation")
message(WARNING "FMT: ${FMT_ROOT_DIR}")
add_subdirectory(${FMT_ROOT_DIR})

list(APPEND SRCS main.cpp)
list(APPEND PROJ_DEP fmt)
list(APPEND PROJ_DEP absl::time)

list(APPEND INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third-party/")
list(APPEND INC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/inc/")

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJ_DEP})
target_link_directories(${PROJECT_NAME} PRIVATE ${LIB_PATH})
target_include_directories(${PROJECT_NAME} PRIVATE ${INC_PATH})

add_custom_target(run
    DEPENDS ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
