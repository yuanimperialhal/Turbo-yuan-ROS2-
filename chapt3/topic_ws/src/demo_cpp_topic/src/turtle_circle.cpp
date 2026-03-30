#include<stdio.h>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"//这个头文件定义了速度消息类型
#include<chrono>//这个头文件定义了时间相关的内容
//使用时间单位字面量，可以在代码中直接使用s和ircle.cpp:2:10: fatams表示秒和毫秒
using namespace std;//使用命名空间，方便使用相关的内容

class TurtleCircle : public rclcpp::Node
{
    private:
        rclcpp::TimerBase::SharedPtr timer_;//定义一个定时器智能指针
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;//定义一个发布器智能指针



    public:

        explicit TurtleCircle(const string &node_name) : Node(node_name)
        //构造TurtleCircle类，传入节点名称，并调用基类构造函数，其中explicit表示禁止隐式转换
        {
            publisher_=this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
            //创建发布器，发布到/turtle1/cmd_vel话题，队列长度为10

            timer_=this->create_wall_timer(1000ms , bind(&TurtleCircle::timer_callback,this));
            //创建定时器，每隔1000毫秒调用一次timer_callback函数
        }
    
    private:
        void timer_callback()
        {
            auto msg=geometry_msgs::msg::Twist();//定义一个速度消息变量
            msg.linear.x=1.0;//设置线速度为1.0
            msg.angular.z=0.5;//设置角速度为0.5
            publisher_->publish(msg);//发布速度消息
        }

};

int main(int argc, char *argv[])
{
    rclcpp::init(argc,argv);//初始化rclcpp
    auto node=make_shared<TurtleCircle>("turtle_circle");//创建节点对象，节点名称为turtle_circle
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}