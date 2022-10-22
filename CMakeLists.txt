project(HoleckyAlgorithm CXX)

cmake_minimum_required(VERSION 3.2)

set(HOLEC_INCLUDE_DIRS "include")

if (NOT MSVC)
  set(GEOMETRY_LIBS "pthread")
endif ()

include_directories(${HOLEC_INCLUDE_DIRS})

add_compile_options(
      -mfpmath=sse
      -fstack-protector-all
      -g
      -W
      -Wall
      -Wextra
      -Wunused
      -Wcast-align
      -Werror -pedantic
      -pedantic-errors
      -Wfloat-equal
      -Wpointer-arith
      -Wformat-security
      -Wmissing-format-attribute
      -Wformat=1
      -Wwrite-strings
      -Wcast-align
      -Wno-long-long
      -Woverloaded-virtual
      -Wnon-virtual-dtor
      -Wcast-qual
      -Wno-suggest-attribute=format
)

add_executable(
  holec_alg
  include/header.hpp
  source/main.cpp
  source/matrx_funcs.cpp
  source/holecki_alg.cpp
)

target_link_libraries(holec_alg ${GEOMETRY_LIBS})
