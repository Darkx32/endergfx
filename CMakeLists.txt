cmake_minimum_required(VERSION 3.21)
project(endergfx VERSION 0.1.0 LANGUAGES CXX)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
  set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

option(ENDERGFX_INSTALL "Install endergfx" ON)
option(ENDERGFX_FETCH_SDL3 "Fetch SDL3 automatically" OFF)
option(ENDERGFX_FETCH_BGFX "Fetch bgfx automatically" ON)
option(ENDERGFX_FETCH_TINYOBJLOADER "Fetch tinyobjloader automatically" ON)
option(ENDERGFX_BUILD_SHARED "Build endergfx as a shared library" OFF)
option(ENDERGFX_BUILD_EXAMPLES "Build endergfx examples" ON)

include(GNUInstallDirs)
include(FetchContent)

if(NOT ENDERGFX_FETCH_SDL3)
  find_package(SDL3 QUIET)
endif()

if(NOT SDL3_FOUND)
  if(NOT ENDERGFX_FETCH_SDL3)
    message(FATAL_ERROR
      "endergfx: SDL3 was not found on this system.\n"
      "Install it and make it discoverable via find_package(SDL3),\n"
      "or configure with -DENDERGFX_FETCH_SDL3=ON to let endergfx\n"
      "download and build it automatically via FetchContent."
    )
  endif()

  fetchcontent_declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.8
    GIT_SHALLOW ON
  )
  fetchcontent_makeavailable(SDL3)
endif()

if(NOT ENDERGFX_FETCH_BGFX)
  find_package(bgfx QUIET)
endif()

if(NOT bgfx_FOUND)
  if(NOT ENDERGFX_FETCH_BGFX)
    message(FATAL_ERROR
      "endergfx: bgfx was not found on this system.\n"
      "Install it and make it discoverable via find_package(bgfx),\n"
      "or configure with -DENDERGFX_FETCH_BGFX=ON to let endergfx\n"
      "download and build it automatically via FetchContent."
    )
  endif()

  fetchcontent_declare(
    bgfx.cmake
    GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake.git
    GIT_TAG v1.157.9447-569
    GIT_SHALLOW ON
  )
  fetchcontent_makeavailable(bgfx.cmake)
endif()

if(ENDERGFX_FETCH_TINYOBJLOADER)
  fetchcontent_declare(
    tinyobjloader
    GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
    GIT_TAG release
    GIT_SHALLOW ON
  )
  fetchcontent_makeavailable(tinyobjloader)
endif()

file(
  GLOB_RECURSE
  ENDERGFX_SOURCES
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

if(ENDERGFX_BUILD_SHARED)
  add_library(endergfx SHARED ${ENDERGFX_SOURCES})
else()
  add_library(endergfx STATIC ${ENDERGFX_SOURCES})
endif()

add_library(endergfx::endergfx ALIAS endergfx)

target_include_directories(endergfx PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(endergfx PUBLIC
  SDL3::SDL3
  bgfx
  bx
  bimg
  tinyobjloader
)

target_compile_features(endergfx PUBLIC cxx_std_20)

target_include_directories(endergfx PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

file(GLOB ENDERGFX_VS_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*_vs.sc"
)

file(GLOB ENDERGFX_FS_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*_fs.sc"
)

set(ENDERGFX_SHADER_GEN_DIR
  "${CMAKE_BINARY_DIR}/generated/shaders"
)

set(ENDERGFX_SHADER_INCLUDE_ARGS)

fetchcontent_getproperties(bgfx.cmake)

if(DEFINED bgfx.cmake_SOURCE_DIR AND
  NOT "${bgfx.cmake_SOURCE_DIR}" STREQUAL "" AND
  EXISTS "${bgfx.cmake_SOURCE_DIR}/bgfx/src")
  list(APPEND ENDERGFX_SHADER_INCLUDE_ARGS
    INCLUDE_DIRS "${bgfx.cmake_SOURCE_DIR}/bgfx/src"
  )
endif()

set(ENDERGFX_SHADERC_ARGS
  -Wno-error
)

if(ENDERGFX_VS_SOURCES)
  bgfx_compile_shaders(
    TYPE VERTEX
    SHADERS ${ENDERGFX_VS_SOURCES}
    VARYING_DEF "${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying.def.sc"
    OUTPUT_DIR "${ENDERGFX_SHADER_GEN_DIR}"
    ${ENDERGFX_SHADER_INCLUDE_ARGS}
    ${ENDERGFX_SHADERC_ARGS}
    AS_HEADERS
  )
endif()

if(ENDERGFX_FS_SOURCES)
  bgfx_compile_shaders(
    TYPE FRAGMENT
    SHADERS ${ENDERGFX_FS_SOURCES}
    VARYING_DEF "${CMAKE_CURRENT_SOURCE_DIR}/shaders/varying.def.sc"
    OUTPUT_DIR "${ENDERGFX_SHADER_GEN_DIR}"
    ${ENDERGFX_SHADER_INCLUDE_ARGS}
    ${ENDERGFX_SHADERC_ARGS}
    AS_HEADERS
  )
endif()

target_sources(endergfx PRIVATE
  ${ENDERGFX_VS_SOURCES}
  ${ENDERGFX_FS_SOURCES}
)

target_compile_definitions(endergfx PUBLIC
  $<$<CONFIG:Debug>:BX_CONFIG_DEBUG=1>
  $<$<NOT:$<CONFIG:Debug>>:BX_CONFIG_DEBUG=0>
)

target_include_directories(endergfx PRIVATE
  "${ENDERGFX_SHADER_GEN_DIR}"
)

if(ENDERGFX_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
