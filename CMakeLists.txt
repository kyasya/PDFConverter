cmake_minimum_required(VERSION 3.10)
project(KFileSystemTest)

# C++14指定
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# wxWidgets 検出と設定
set(wxWidgets_USE_STATIC_LIBS ON)
find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

# 実行ファイル main（リンク忘れに注意）
add_executable(pdfconv-cui src/pdfconv-cui.cpp)
target_link_libraries(pdfconv-cui ${wxWidgets_LIBRARIES})

add_executable(gui src/gui.cpp)
target_link_libraries(gui ${wxWidgets_LIBRARIES})

# ヘッダファイルのパスを指定
target_include_directories(pdfconv-cui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
