#include <cstdlib>
#include <ctime>
#include "rclcpp/rclcpp.hpp"
#include "chapt4_interfaces/srv/patrol.hpp"
#include <chrono>
#include "rcl_interfaces/msg/parameter.hpp"
#include "rcl_interfaces/msg/parameter_value.hpp"
#include "rcl_interfaces/msg/parameter_type.hpp"
#include "rcl_interfaces/srv/set_parameters.hpp"

using Setp= rcl_interfaces::srv::SetParameters;
//创建SetParameters服务的别名，方便使用相关的内容



using namespace std::chrono_literals;//使用时间间隔的字面量，方便使用时间单位
using Patrol = chapt4_interfaces::srv::Patrol;//创建Patrol服务的别名，方便使用相关的内容


class PatrolClient : public rclcpp::Node
//创建PatrolClient类，继承自rclcpp::Node
{
    public:
    PatrolClient():Node("patrol_client")//构造PatrolClient类，节点名称为patrol_client
    {
        patrol_client_= this->create_client<Patrol>("patrol");
        //创建服务客户端，服务类型为Patrol，服务名称为patrol
        timer_ = this->create_wall_timer(10s, bind(&PatrolClient::time_callback, this));
        //创建定时器，每隔10秒调用一次time_callback函数
        srand(time(NULL));
        //设置随机数种子，使用当前时间作为种子，保证每次运行程序时生成的随机数不同
   
    }

    std::shared_ptr<Setp::Response> call_set_parameters(rcl_interfaces::msg::Parameter& parameter)
    //定义参数设置回调函数set_parameters_callback，接收一个参数列表，返回一个SetParametersResult对象的智能指针
    {
        //1.创建客户端等待服务上线
        auto param_client = this->create_client<Setp>("/turtle_control/set_parameters");
        while(!param_client->wait_for_service(1s))
        {
            if(!rclcpp::ok())
            {
                RCLCPP_ERROR(this->get_logger(),"参数服务客户端被中断");
                return nullptr;
            }
            RCLCPP_INFO(this->get_logger(),"等待参数服务可用...");
        }
        //2.创建请求对象并发送请求
        auto request = std::make_shared<Setp::Request>();
        request->parameters.push_back(parameter);
        //3.异步发送请求并等待响应
        auto future = param_client->async_send_request(request);
        
        rclcpp::spin_until_future_complete(this->get_node_base_interface(), future);
        //等待请求完成，获取响应
        auto response = future.get();
        return response;


    }   

    void update_server_param_k(double k)//更新参数服务
    {
        //1.创建参数对象
        auto parameter = rcl_interfaces::msg::Parameter(); 
        parameter.name = "k";
        //2.设置参数值对象并赋值
        auto parameter_value = rcl_interfaces::msg::ParameterValue();//创建参数值对象
        parameter_value.type = rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE; 
        //设置参数值对象的类型为double   
        parameter_value.double_value = k;//将参数值对象的类型设置为double，并赋值为k
        parameter.value = parameter_value;//把参数值对象赋值给参数对象的value成员
        //3.更新参数服务器的参数值，并处理响应结果
        auto response = call_set_parameters(parameter);
        if(response == nullptr)
        {
            RCLCPP_ERROR(this->get_logger(),"调用参数服务失败");
            return;     
        }
        else 
        {
            for(const auto& result : response->results)
            {
                if(result.successful)
                {
                    RCLCPP_INFO(this->get_logger(),"参数更新成功,修改参数k的值为： %.2f", k);
                }
                else
                {
                    RCLCPP_WARN(this->get_logger(),"参数更新失败，原因：%s", result.reason.c_str());
                }
            }
        }    
    }
    

    void time_callback()
    //定义回调函数time_callback，定时发送巡逻请求
    {
        //1.等待服务可用

        while(!patrol_client_->wait_for_service(1s))
        {
            if(!rclcpp::ok())
            {
                RCLCPP_ERROR(this->get_logger(),"客户端被中断");
                return;
            }
            RCLCPP_INFO(this->get_logger(),"等待服务可用...");
        }
        //2.创建请求对象
        auto request = std::make_shared<Patrol::Request>();
        request->target_x = static_cast<float>(rand() % 1200) / 100.0f;//生成随机的目标x坐标，范围为0.0到12.0
        request->target_y = static_cast<float>(rand() % 1200) / 100.0f;//生成随机的目标y坐标，范围为0.0到12.0
        RCLCPP_INFO(this->get_logger(),"发送巡逻请求，目标位置为，x: %.2f, y: %.2f", request->target_x, request->target_y);
        //打印发送的请求信息

        //3.异步发送请求并等待响应
        patrol_client_->async_send_request(
            request,
            [this](rclcpp::Client<Patrol>::SharedFuture result_future)
            {
                auto response = result_future.get();
                if(response->result == Patrol::Response::SUCESS)
                {
                    RCLCPP_INFO(this->get_logger(),"巡逻请求成功，目标位置有效");
                }
                else
                {
                    RCLCPP_WARN(this->get_logger(),"巡逻请求失败，目标位置无效");
                }
            }
        );//异步发送请求，回调函数接收一个SharedFuture对象，获取响应并判断结果，根据结果打印相应的信息
        
    }
    private:
        rclcpp::Client<Patrol>::SharedPtr patrol_client_;//定义服务客户端智能指针
        rclcpp::TimerBase::SharedPtr timer_;//定义定时器智能指针

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);//初始化rclcpp
    auto node = std::make_shared<PatrolClient>();//创建节点对象，节点名称为patrol_client
    node->update_server_param_k(2.0);//调用参数设置函数，更新参数k的值为2.0
    rclcpp::spin(node);//进入循环，等待回调函数被调用
    rclcpp::shutdown();//关闭rclcpp
    return 0;
}