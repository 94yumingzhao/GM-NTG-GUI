# CMakeLists.txt - GM-NTG-GUI
# 多工厂NTG博弈求解器的图形界面
cmake_minimum_required(VERSION 3.24)
project(GM-NTG-GUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt 路径配置
set(CMAKE_PREFIX_PATH "D:/Tools-DV/Qt/6.10.1/msvc2022_64")

# 查找 Qt 组件
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# 源文件
set(SOURCES
    src/main.cpp
    src/main_window.cpp
    src/parameter_widget.cpp
    src/log_widget.cpp
    src/results_widget.cpp
    src/solver_worker.cpp
    src/analysis_widget.cpp
    src/generator_widget.cpp
    src/panels/overview_panel.cpp
    src/panels/plant_panel.cpp
    src/panels/coalition_panel.cpp
    src/panels/iteration_panel.cpp
)

# 头文件
set(HEADERS
    src/main_window.h
    src/parameter_widget.h
    src/log_widget.h
    src/results_widget.h
    src/solver_worker.h
    src/analysis_widget.h
    src/generator_widget.h
    src/panels/overview_panel.h
    src/panels/plant_panel.h
    src/panels/coalition_panel.h
    src/panels/iteration_panel.h
)

# 可执行文件
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Widgets
)

# 输出目录
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin/Debug"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin/Release"
)

# 构建信息
message(STATUS "")
message(STATUS "=== GM-NTG-GUI Build Configuration ===")
message(STATUS "Qt Version: ${Qt6_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output: ${CMAKE_BINARY_DIR}/bin")
message(STATUS "=======================================")
message(STATUS "")
