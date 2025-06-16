cmake_minimum_required(VERSION 3.28)
project(VodozemacTests)

set(CMAKE_CXX_STANDARD 23)

find_program(MAKE_EXE NAMES gmake nmake make)

add_custom_target(
    VodozemacRustBindings
    COMMAND ${MAKE_EXE} -C ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp PREFIX=output install
)


add_library(VodozemacBindings_shared SHARED bridge.cpp)
add_dependencies(VodozemacBindings_shared VodozemacRustBindings)


target_include_directories(VodozemacBindings_shared PUBLIC .)
target_include_directories(VodozemacBindings_shared PUBLIC ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp/output/include)
target_link_libraries(VodozemacBindings_shared ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp/output/lib/libvodozemac.a)

add_library(VodozemacBindings_static STATIC bridge.cpp)
add_dependencies(VodozemacBindings_static VodozemacRustBindings)

target_include_directories(VodozemacBindings_static PUBLIC .)
target_include_directories(VodozemacBindings_static PUBLIC ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp/output/include)
target_link_libraries(VodozemacBindings_static ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp/output/lib/libvodozemac.a)


add_executable(PureCTest test.c)
target_link_libraries(PureCTest VodozemacBindings_static)
