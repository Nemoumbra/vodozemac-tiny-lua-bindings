cmake_minimum_required(VERSION 3.28)
project(VodozemacPureCBindings)

set(CMAKE_CXX_STANDARD 23)

set(VODOZEMAC_PATH ${CMAKE_BINARY_DIR}/rust_bindings)
set(VODOZEMAC_INCLUDES ${VODOZEMAC_PATH}/include)
set(VODOZEMAC_LIBS ${VODOZEMAC_PATH}/lib)


find_program(MAKE_EXE NAMES gmake nmake make)

# Executes 'make' to create the vodozemac artifacts
# TODO: make sure the build command is configurable from CMake (debug vs release, etc).
add_custom_target(
    VodozemacRustBindings_build
    COMMAND ${MAKE_EXE} -C ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp
    BYPRODUCTS ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp/target
)

# "Installs" the vodozemac artifacts into the easy to work with dir
add_custom_target(
    VodozemacRustBindings
    COMMAND ${MAKE_EXE} -C ${CMAKE_SOURCE_DIR}/ext/vodozemac_bindings/cpp PREFIX=${VODOZEMAC_PATH} install
    BYPRODUCTS ${VODOZEMAC_PATH}
)
add_dependencies(VodozemacRustBindings VodozemacRustBindings_build)


# Bindings (shared)
add_library(VodozemacBindings_shared SHARED bridge.cpp)
add_dependencies(VodozemacBindings_shared VodozemacRustBindings)

target_include_directories(VodozemacBindings_shared PUBLIC .)
target_include_directories(VodozemacBindings_shared PUBLIC ${VODOZEMAC_INCLUDES})
target_link_libraries(VodozemacBindings_shared ${VODOZEMAC_LIBS}/libvodozemac.a)


# Bindings (static)
add_library(VodozemacBindings_static STATIC bridge.cpp)
add_dependencies(VodozemacBindings_static VodozemacRustBindings)

target_include_directories(VodozemacBindings_static PUBLIC .)
target_include_directories(VodozemacBindings_static PUBLIC ${VODOZEMAC_INCLUDES})
target_link_libraries(VodozemacBindings_static ${VODOZEMAC_LIBS}/libvodozemac.a)



# Lua stuff
set(DESIRED_LUA_VERSION "5.3" CACHE STRING "The version of Lua to build against (e.g., 5.1, 5.2, 5.3, 5.4)")

# Optional: Provide a dropdown list for GUI environments like cmake-gui (thank you, pseudo gemini)
set_property(CACHE DESIRED_LUA_VERSION PROPERTY STRINGS "5.1" "5.2" "5.3" "5.4")
find_package(Lua ${DESIRED_LUA_VERSION} EXACT REQUIRED)

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(LUA REQUIRED lua-5.3)



set(LUA_PATH ${CMAKE_BINARY_DIR}/lua_bindings)

# The Lua bindings
add_library(VodozemacLua SHARED lua_bindings.c)
target_include_directories(VodozemacLua PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(VodozemacLua VodozemacBindings_static)

set_target_properties(VodozemacLua PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(VodozemacLua PROPERTIES PREFIX "")
set_target_properties(VodozemacLua PROPERTIES OUTPUT_NAME "megolm")
set_target_properties(VodozemacLua PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LUA_PATH})
