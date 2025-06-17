cmake_minimum_required(VERSION 3.28)
project(VodozemacPureCBindings)

set(CMAKE_CXX_STANDARD 23)

set(VODOZEMAC_PATH ${CMAKE_BINARY_DIR}/rust_bindings)
set(VODOZEMAC_INCLUDES ${VODOZEMAC_PATH}/include)
set(VODOZEMAC_LIBS ${VODOZEMAC_PATH}/lib)


find_program(MAKE_EXE NAMES gmake nmake make)
add_custom_target(
    VodozemacRustBindings_build
    COMMAND ${MAKE_EXE} -C ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp
    BYPRODUCTS ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp/target
)

add_custom_target(
    VodozemacRustBindings
    COMMAND ${MAKE_EXE} -C ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp PREFIX=${VODOZEMAC_PATH} install
    BYPRODUCTS ${VODOZEMAC_PATH}
)
add_dependencies(VodozemacRustBindings VodozemacRustBindings_build)

add_library(VodozemacBindings_shared SHARED bridge.cpp)
add_dependencies(VodozemacBindings_shared VodozemacRustBindings)

target_include_directories(VodozemacBindings_shared PUBLIC .)
target_include_directories(VodozemacBindings_shared PUBLIC ${VODOZEMAC_INCLUDES})
target_link_libraries(VodozemacBindings_shared ${VODOZEMAC_LIBS}/libvodozemac.a)

add_library(VodozemacBindings_static STATIC bridge.cpp)
add_dependencies(VodozemacBindings_static VodozemacRustBindings)

target_include_directories(VodozemacBindings_static PUBLIC .)
target_include_directories(VodozemacBindings_static PUBLIC ${VODOZEMAC_INCLUDES})
target_link_libraries(VodozemacBindings_static ${VODOZEMAC_LIBS}/libvodozemac.a)




set(LUA_PATH ${CMAKE_BINARY_DIR}/lua_bindings)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LUA REQUIRED lua-5.3)

add_library(VodozemacLua SHARED lua_bindings.c)
target_include_directories(VodozemacLua PUBLIC ${LUA_INCLUDE_DIRS})
target_link_libraries(VodozemacLua VodozemacBindings_static)
set_target_properties(VodozemacLua PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(VodozemacLua PROPERTIES PREFIX "")
set_target_properties(VodozemacLua PROPERTIES OUTPUT_NAME "megolm")
set_target_properties(VodozemacLua PROPERTIES  LIBRARY_OUTPUT_DIRECTORY ${LUA_PATH})
