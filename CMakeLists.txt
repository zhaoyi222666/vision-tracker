cmake_minimum_required(VERSION 3.16)

project(vision_tracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

## 给 clangd / VSCode IntelliSense 生成 compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tracker
  src/main.cpp
  src/ArmorTarget.cpp
  src/SpinningTarget.cpp
  src/TargetManager.cpp
  src/TrackingBase.cpp
  include/data_types.hpp
  include/TargetManager.hpp
  include/ArmorTarget.hpp
  include/SpinningTarget.hpp
  include/TrackingBase.hpp
  include/test_frames.hpp
)

target_include_directories(tracker PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
