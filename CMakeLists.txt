cmake_minimum_required(VERSION 3.28)
project(VodozemacTests)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(src)

add_executable(PureCTest test.c)
target_include_directories(PureCTest PUBLIC ${VodozemacBindings_static_INCLUDES})
target_link_libraries(PureCTest VodozemacBindings_static)

# WIP
set(VODOZEMAC_PATH ${CMAKE_BINARY_DIR}/rust_bindings)
set(VODOZEMAC_INCLUDES ${VODOZEMAC_PATH}/include)
set(VODOZEMAC_LIBS ${VODOZEMAC_PATH}/lib)

add_executable(CppTests main.cpp)
target_include_directories(CppTests PUBLIC ${VODOZEMAC_INCLUDES})
target_link_libraries(CppTests PUBLIC ${VODOZEMAC_LIBS}/libvodozemac.a)