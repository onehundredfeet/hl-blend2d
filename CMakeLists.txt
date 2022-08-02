cmake_minimum_required(VERSION 3.10)
project (blend2d C CXX )

set(APP_DIR "${CMAKE_CURRENT_LIST_DIR}"
    CACHE PATH "Application directory")

set(BLEND2D_DIR "${APP_DIR}/ext/blend2d"
    CACHE PATH "Location of 'blend2d'")

if (NOT HDLL_DESTINATION) 
    set(HDLL_DESTINATION "/usr/local/lib")
endif()

if (NOT HL_INCLUDE_DIR) 
    set(HL_INCLUDE_DIR "/usr/local/include")
endif()

if (NOT COMMON_LIB_DIR) 
    set(COMMON_LIB_DIR "/usr/local/lib")
endif()

# Enable Blend2D static build.
set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

#### Project definitions

add_library(blend2d.hdll SHARED
#src/hl-idl-helpers.cpp
src/blend2d.cpp
)


set_target_properties(blend2d.hdll
PROPERTIES
PREFIX ""
OUTPUT_NAME blend2d
SUFFIX .hdll
)

set_property(TARGET blend2d.hdll PROPERTY CXX_STANDARD 17)

cmake_policy(SET CMP0015 NEW)

target_include_directories(blend2d.hdll
PRIVATE
${HL_INCLUDE_DIR}
)


if (UNIX)
    # Some special flags are needed for GNU GCC compiler
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fPIC -fpermissive")
    #not sure why the ${HL_LIB_DIR} is necessary given the above.
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -shared  ")

    find_library(LIBHL NAMES hl HINTS ${HL_LIB_DIR} )
else()
    find_library(LIBHL NAMES libhl HINTS ${HL_LIB_DIR} )
endif (UNIX)

set_property (TARGET blend2d.hdll APPEND_STRING PROPERTY COMPILE_FLAGS )
 
target_link_libraries(blend2d.hdll ${LIBHL} Blend2D::Blend2D)

install(TARGETS blend2d.hdll DESTINATION ${HDLL_DESTINATION})
