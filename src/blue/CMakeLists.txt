cmake_minimum_required(VERSION 3.8)
project(blue)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# -----------------
# Find dependencies
# -----------------

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)

# -------------------
# Include directories
# -------------------

include_directories(include)

# -------------------
# Declare executables
# -------------------

add_executable(color_filter_node src/color_filter_node.cpp
                                 src/color_filter.cpp)

# -----------------------------
# Link Target with dependencies
# -----------------------------

ament_target_dependencies(color_filter_node
                          rclcpp sensor_msgs
                          OpenCV
                          cv_bridge)

target_include_directories(color_filter_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  ${OpenCV_INCLUDE_DIRS}
)

# -------------
# Install Rules
# -------------

# Executable
install(TARGETS color_filter_node DESTINATION lib/${PROJECT_NAME})
# Launch and RViz files
install(DIRECTORY launch
        DESTINATION share/${PROJECT_NAME})
# Header files
install(DIRECTORY include/
        DESTINATION include/
)

# ------------
# Auto-Linting
# ------------

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
