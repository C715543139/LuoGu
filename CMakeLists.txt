cmake_minimum_required(VERSION 3.22.1)

project(LuoGu LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# PB_DS 等 GNU 扩展需要使用 gnu++17。
set(CMAKE_CXX_EXTENSIONS ON)

# 支持目标文件夹的 IDE 可以按题目目录展示目标。
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 为一个目录中的每道题创建独立的可执行目标。
# 目标直接使用题号命名，使 IDE 分组内只显示 P3378 等简短名称。
function(add_luogu_targets)
    foreach(source IN LISTS ARGN)
        get_filename_component(problem "${source}" NAME_WE)

        # CMake 要求整个工程中的逻辑目标名唯一，发现重复题号时立即给出明确错误。
        if(TARGET "${problem}")
            message(FATAL_ERROR "Duplicate problem target: ${problem}")
        endif()

        # 默认构建时不一次编译全部题目，仍可在 CLion 中单独选择并运行该目标。
        add_executable("${problem}" EXCLUDE_FROM_ALL "${source}")
    endforeach()
endfunction()

add_subdirectory("Algorithm 2-1")
add_subdirectory("Algorithm 2-2")
add_subdirectory("Algorithm 2-3")
add_subdirectory("Algorithm 2-4")
add_subdirectory("DataStructure 2-1")
