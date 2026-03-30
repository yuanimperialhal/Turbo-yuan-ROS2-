#include<stdio.h>
#include<iostream>
#include<math.h>//包含数学库，使用数学函数
#include<functional>//包含函数对象库，使用bind函数
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"//这个头文件定义了速度消息类型
#include "turtlesim/msg/pose.hpp"//这个头文件定义了乌龟位姿消息类型
using namespace std;//使用命名空间，方便使用相关的内容

class TurtleControl : public rclcpp::Node
//创建TurtleControl类，继承自rclcpp::Node
{
    public:
        TurtleControl():Node("turtle_control")//构造TurtleControl类，节点名称为turtle_control
        {
            velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
            //创建发布器，发布到/turtle1/cmd_vel话题，队列长度为10

            pose_subscription_ = this->create_subscription<turtlesim::msg::Pose>(
                "/turtle1/pose",10,
                bind(&TurtleControl::one_pose_received_,this,placeholders::_1)   
            );
            //创建订阅器，订阅/turtle1/pose话题，队列长度为10，回调函数为one_pose_received_
        }

    private:

        void one_pose_received_(const turtlesim::msg::Pose::SharedPtr pose)
        //定义回调函数one_pose_received_，接收乌龟位姿消息
        //TODO: 收到位置计算误差，发布速度命令
        {
            auto message = geometry_msgs::msg::Twist();//定义一个速度消息变量
            //1.记录当前位置
            double current_x = pose->x;
            double current_y = pose->y;
            RCLCPP_INFO(this->get_logger(),"当前位置为，x: %.2f, y: %.2f", current_x, current_y);

            //2.计算目标之间的距离，以及与当前海龟朝向的角度差
            double distance =sqrt(pow((target_x_ - current_x),2) + pow((target_y_ - current_y),2));
            //计算当前位置与目标位置之间的欧几里得距离
            double angle=atan2((target_y_ - current_y),(target_x_ - current_x))-pose->theta;
            //计算当前位置与目标位置之间的角度差,theta为乌龟当前朝向角

            //3.控制策略：距离大于0.1时，继续运动，角度大于0.2则原地旋转，否则直行
            if(distance>0.1)
            {
                if(fabs(angle)>0.2)
                {
                    message.angular.z=fabs(angle);

                }
                else
                {
                    message.linear.x=k_*distance;

                }
            }
            //4.限制最大速度,并发布消息
            if(message.linear.x>max_speed_)
            {
                message.linear.x=max_speed_;
            }
            velocity_publisher_->publish(message);



        }

    private:
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;//定义速度发布器智能指针
        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscription_;//定义位姿订阅器智能指针

        double target_x_{1.0}, target_y_{1.0};
        //目标位置，默认为(1.0, 1.0)

        double k_{1.0};
        //比例系数，用于计算速度命令，
        //控制输出=误差*比例系数

        double max_speed_{2.0};
        //最大速度，默认为2.0
};


int main(int argc,char* argv[])
{
    rclcpp::init(argc,argv);//初始化rclcpp
    auto node=make_shared<TurtleControl>();//创建节点对象，节点名称为turtle_control
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}