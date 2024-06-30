cmake_minimum_required(VERSION 3.27)
project(GaussElimination C)

set(CMAKE_C_STANDARD 17)

# -O0 (i.e. no optimization) is enabled by default
# So, in order to use -march=native we should enable optimizations
if (ARCH_NATIVE)
    set(EXTRA_FLAGS "-march=native -O2")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mavx ${EXTRA_FLAGS}")

set(COMMON_SOURCES src/benchmark.c src/benchmark.h)

macro(add_ge_executable name file)
    add_executable(${name} ${COMMON_SOURCES} ${file})
    target_link_libraries(${name} PRIVATE m papi) # math
    target_compile_definitions(${name} PRIVATE MAT_SIZE=1500)
endmacro()

add_ge_executable(ge1 src/ge1.c)
add_ge_executable(ge2 src/ge2.c)
add_ge_executable(ge3 src/ge3.c)
add_ge_executable(ge4 src/ge4.c)
add_ge_executable(ge5 src/ge5.c)
add_ge_executable(ge6 src/ge6.c)
add_ge_executable(ge7 src/ge7.c)
add_ge_executable(ge8 src/ge8.c)
