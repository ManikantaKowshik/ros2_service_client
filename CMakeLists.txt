cmake_minimum_required(VERSION 3.8)
project(ros2_service_client)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)

# Add the executable
add_executable(add_two_ints_client src/add_two_ints_client.cpp)

# Link dependencies to the executable
ament_target_dependencies(add_two_ints_client 
  rclcpp 
  example_interfaces
)

# Install the executable to the lib folder
install(TARGETS
  add_two_ints_client
  DESTINATION lib/${PROJECT_NAME})

# (Optional) Install the package.xml
install(DIRECTORY include/
  DESTINATION include/
)

ament_package()
