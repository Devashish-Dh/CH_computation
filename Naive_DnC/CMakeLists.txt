cmake_minimum_required(VERSION 3.10)

project(seq_dnc_ch
    VERSION 1.0
    DESCRIPTION "NA"
    LANGUAGES CXX
)

add_executable(seq_dnc_ch
    src/main.cpp
    src/files_read_write.cpp
)

target_include_directories(seq_dnc_ch
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_features(seq_dnc_ch PUBLIC cxx_std_17)
