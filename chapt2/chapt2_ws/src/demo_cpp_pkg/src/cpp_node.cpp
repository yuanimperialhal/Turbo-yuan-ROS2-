#include "rclcpp/rclcpp.hpp"
#include <iostream>
using namespace std;

int main(int argc,char **argv)
{
    rclcpp::init(argc, argv);
    auto node = make_shared<rclcpp::Node>("cpp_node");
    RCLCPP_INFO(rclcpp::get_logger("cpp_node"), "Hello from C++ node!");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}