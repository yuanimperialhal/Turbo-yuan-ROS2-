#include<iostream>

#include "rclcpp/rclcpp.hpp"

using namespace std;


class PersonNode : public rclcpp::Node
//定义 PersonNode 类，继承自 rclcpp::Node
//在类体内可以添加成员变量和成员函数
{
    private:
        string name_;// 成员变量，存储姓名
            int age_;// 成员变量，存储年龄
        
        public:
        /*
            PersonNode(const string &node_name, const string &name, const int &age) 
            : rclcpp::Node(node_name), name_(name), age_(age) // 初始化列表,调用基类构造函数,初始化成员变量
            {
                // 构造函数，初始化节点名称、姓名和年龄
            } ;
        */

            PersonNode(const string &node_name, const string &name, const int &age)
            : rclcpp::Node(node_name)// 初始化列表,调用基类构造函数
            {
                //初始化成员变量
                name_ = name;
                age_ = age;
            };
            

            void eat(const string &food_name)// 成员函数，表示吃东西的行为
            {
                RCLCPP_INFO(this->get_logger(), "Hello, my name is %s and I am %d years old. I am eating %s", name_.c_str(), age_, food_name.c_str());
                // 成员函数，打印吃东西的信息
            };


};


int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);// 初始化 ROS 2 客户端库
    auto node = make_shared<PersonNode>("cpp_node","Alice",20);  // 创建 PersonNode 节点实例，节点名称为 "cpp_node"，姓名为 "Alice"，年龄为 20

    node->eat("apple"); // 调用 eat 函数，表示吃苹果

    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
    


}
