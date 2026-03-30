//添加头文件，并且创建类别名
#include<stdio.h>
#include<iostream>
#include<math.h>//包含数学库，使用数学函数
#include<functional>//包含函数对象库，使用bind函数
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"//这个头文件定义了速度消息类型
#include "turtlesim/msg/pose.hpp"//这个头文件定义了乌龟位姿消息类型
using namespace std;//使用命名空间，方便使用相关的内容
#include "chapt4_interfaces/srv/patrol.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"
using SetParametersResult = rcl_interfaces::msg::SetParametersResult;
//创建SetParametersResult消息的别名，方便使用相关的内容
using Patrol = chapt4_interfaces::srv::Patrol;
using namespace std;//使用命名空间，方便使用相关的内容

class TurtleControl : public rclcpp::Node
//创建TurtleControl类，继承自rclcpp::Node
{
    public:
        TurtleControl():Node("turtle_control")//构造TurtleControl类，节点名称为turtle_control
        {
            this->declare_parameter("k",1.0);//声明参数k，默认值为1.0
            this->declare_parameter("max_speed",1.0);//声明参数max_speed，默认值为1.0
            this->get_parameter("k",k_);//获取参数k的值，存储在成员变量k_中
            this->get_parameter("max_speed",max_speed_);//获取参数max_speed的值，存储在成员变量max_speed_中 


            velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
            //创建发布器，发布到/turtle1/cmd_vel话题，队列长度为10

            pose_subscription_ = this->create_subscription<turtlesim::msg::Pose>(
                "/turtle1/pose",10,
                bind(&TurtleControl::one_pose_received_,this,placeholders::_1)   
            );
            //创建订阅器，订阅/turtle1/pose话题，队列长度为10，回调函数为one_pose_received_

            patrol_service_ = this->create_service<Patrol>
            (
                "patrol",
                [&](
                    const shared_ptr<Patrol::Request> request,
                    const shared_ptr<Patrol::Response> response
                   )->void
                    {
                        //判断巡逻点是否在模拟器边界内
                        if(0.0 < request->target_x && request->target_x < 12.0f &&
                           0.0 < request->target_y && request->target_y < 12.0f)
                        {
                            target_x_ = request->target_x;
                            target_y_ = request->target_y;
                            response->result = Patrol::Response::SUCESS;
                        }
                        else
                        {
                            response->result = Patrol::Response::FAIL;
                        }
                    }
            );
            //创建服务，服务类型为Patrol，服务名称为patrol
            //回调函数为一个lambda函数，接收请求和响应指针
            //判断请求中的目标位置是否在模拟器边界内，如果在则更新目标位置并返回成功，否则返回失败

        //添加参数设置回调函数，动态调整控制参数

        parameters_callback_handle_ = this->add_on_set_parameters_callback(
            [&](const std::vector<rclcpp::Parameter> &params)->SetParametersResult
            {
                //遍历参数列表，更新参数值
                for(const auto &param : params)
                {
                RCLCPP_INFO(this->get_logger(),"更新参数: %s值为： %f", 
                param.get_name().c_str(), param.as_double());
                    if(param.get_name() == "k")//如果参数名称为k，则更新比例系数k_
                    {
                        k_ = param.as_double();
                        RCLCPP_INFO(this->get_logger(),"Updated k: %.2f", k_);
                    }
                    else if(param.get_name() == "max_speed")
                    //如果参数名称为max_speed，则更新最大速度max_speed_
                    {
                        max_speed_ = param.as_double();
                        RCLCPP_INFO(this->get_logger(),
                        "Updated max_speed: %.2f", max_speed_);                    
                    }

                }

                auto result = SetParametersResult();
                result.successful = true;
                return result;//返回参数设置结果，表示成功
        });
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
        rclcpp::Service<Patrol>::SharedPtr patrol_service_;//定义巡逻服务智能指针
        OnSetParametersCallbackHandle::SharedPtr parameters_callback_handle_;
        //定义参数设置回调句柄智能指针,
        //OnSetParametersCallbackHandle是一个结构体，
        //包含一个回调函数成员变量callback，
        //类型为std::function<SetParametersResult(const std::vector<rclcpp::Parameter> &)>，
        //用于处理参数设置请求 

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
