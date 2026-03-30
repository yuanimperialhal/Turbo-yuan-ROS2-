cmake_minimum_required(VERSION 3.8)
project(demo_cpp_pkg)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#本代码段用于包含头文件目录，添加头文件的搜索路径，以便编译器能够找到相应的头文件。



# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

find_package(rclcpp REQUIRED)
#1.查找可执行文件的头文件和库文件





add_executable(cpp_node src/cpp_node.cpp)
add_executable(person_node src/person_node.cpp)
add_executable(learn_auto src/learn_auto.cpp)
add_executable(learn_shared_ptr src/learn_shared_ptr.cpp)
add_executable(learn_lambda src/learn_lambda.cpp)
add_executable(learn_function src/learn_function.cpp)
add_executable(learn_thread src/learn_thread.cpp)



#2.添加可执行文件 cpp_node person_node

ament_target_dependencies(cpp_node rclcpp)
ament_target_dependencies(person_node rclcpp)
ament_target_dependencies(learn_auto rclcpp)
#3.为可执行文件添加依赖项 rclcpp

install(TARGETS
  cpp_node person_node learn_auto learn_shared_ptr learn_lambda learn_function learn_thread
  DESTINATION lib/${PROJECT_NAME})
#4.将编译好的可执行文件复制到安装目录 lib/demo_cpp_pkg 下

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
