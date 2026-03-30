import launch
import launch_ros

def generate_launch_description():

    #创建参数声明 action,用于解析launch命令中的参数

    action_declare_arg_max_speed = launch.actions.DeclareLaunchArgument(
        "launch_max_speed",
        default_value="3.0"
    )



    action_node_turtle_control = launch_ros.actions.Node(
        package="demo_cpp_service",
        executable="turtle_control",
        #使用launch命令行参数 max_speed 的值作为节点的参数 max_speed 的值
        parameters=[{"max_speed": launch.substitutions.LaunchConfiguration('launch_max_speed',default="3.0")}],
        output="screen",
    )

    action_node_patrol_client = launch_ros.actions.Node(
        package="demo_cpp_service",
        executable="patrol_client",
        output="screen",
    )

    action_node_turtlesim_node = launch_ros.actions.Node(
        package="turtlesim",
        executable="turtlesim_node",
        output="screen",
    )   

    #合成启动并且返回启动描述对象

    launch_description= launch.LaunchDescription([
        action_declare_arg_max_speed,
        action_node_turtle_control,
        action_node_patrol_client,
        action_node_turtlesim_node])
    
    return launch_description