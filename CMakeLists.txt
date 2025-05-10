cmake_minimum_required(VERSION 3.10)
project(KFileSystemTest)

# C++14指定
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# 実行ファイル main
add_executable(main src/main.cpp)
add_executable(gui src/gui.cpp)

# ヘッダファイルのパスを指定
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(gui ${wxWidgets_LIBRARIES})
