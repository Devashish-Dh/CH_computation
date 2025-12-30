cmake_minimum_required(VERSION 3.10)

project(dnc_ch
    VERSION 1.0
    DESCRIPTION "NA"
    LANGUAGES CXX
)

add_executable(dnc_ch
    src/main.cpp
    src/files_read_write.cpp
    src/2d.cpp
    src/3d.cpp
    src/2d_io.cpp
)

target_include_directories(dnc_ch
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(dnc_ch PUBLIC cxx_std_17)
