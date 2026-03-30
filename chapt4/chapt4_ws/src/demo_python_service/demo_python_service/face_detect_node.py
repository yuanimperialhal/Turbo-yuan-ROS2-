import rclpy
from rclpy.node import Node
from chapt4_interfaces.srv import FaceDetector
from ament_index_python.packages import get_package_share_directory#获取资源文件路径
from cv_bridge import CvBridge      #opencv和ROS图像消息之间的转换
from rcl_interfaces.msg import SetParametersResult #用于动态参数设置的结果消息类型
import cv2
import face_recognition
import time
import os

class FaceDetectorinNode(Node):
    def __init__(self):
        super().__init__('face_detect_node')#创建一个名为face_detect_node的节点,继承自Node类
        self.bridge=CvBridge()#实例化CvBridge对象
        self.service=self.create_service(FaceDetector,'/face_detect',self.face_detect_callback)
        #创建一个名为/face_detect的服务,使用FaceDetector服务类型,并指定回调函数为face_detect_callback

        self.get_logger().info('人脸检测服务已启动')#日志输出,表示服务已启动

        self.default_image_path=os.path.join(get_package_share_directory('demo_python_service'),'resource','ceshi2.jpg')#获取默认图像路径

        #声明和获取参数
        self.declare_parameter('face_locations_upsample_times', 1)
        #声明参数face_locations_upsample_times,默认值为1
        self.declare_parameter('face_locations_model', 'hog')
        #声明参数face_locations_model,默认值为'hog'

        self.upsample_times=self.get_parameter('face_locations_upsample_times').value
        #获取参数face_locations_upsample_times的值,并赋值给实例变量
        self.model=self.get_parameter('face_locations_model').value
        #获取参数face_locations_model的值,并赋值给实例变量

        self.add_on_set_parameters_callback(self.parameter_callback)


    def parameter_callback(self,parameters):
        #动态参数设置回调函数,当参数被修改时调用
        for parameter in parameters:
            self.get_logger().info(f'参数{parameter.name}被设置为{parameter.value}')
            #日志输出,表示哪个参数被修改以及修改后的值
            if parameter.name=='face_locations_upsample_times':
                self.upsample_times=parameter.value
                #如果修改的参数是face_locations_upsample_times,则更新实例变量的值
            elif parameter.name=='face_locations_model':
                self.model=parameter.value
                #如果修改的参数是face_locations_model,则更新实例变量的值
        return SetParametersResult(successful=True)
        #返回参数设置结果,表示参数设置成功




    def face_detect_callback(self,request,response):
        #TODO:实现人脸检测逻辑
        
        if request.image.data:
            #如果请求中包含图像数据,则进行人脸检测
            cv_image=self.bridge.imgmsg_to_cv2(request.image)#将ROS图像消息转换为OpenCV图像格式

        else:
            #如果请求中不包含图像数据,则使用默认图像进行人脸检测
            cv_image=cv2.imread(self.default_image_path)

        start_time=time.time()#记录开始时间
        self.get_logger().info('正在进行人脸检测...')#日志输出,表示正在进行人脸检测

        face_locations=face_recognition.face_locations(cv_image,number_of_times_to_upsample=self.upsample_times,model=self.model)
        #使用face_recognition库进行人脸检测,返回人脸位置列表
        #cv_image为输入图像,number_of_times_to_upsample为上采样次数,model为检测模型

        end_time=time.time()#记录结束时间
        self.get_logger().info(f'人脸检测完成,共检测到{len(face_locations)}张人脸,耗时{end_time-start_time:.2f}秒')
        #日志输出,表示人脸检测完成,并显示检测到的人脸数量和耗时

        response.number=len(face_locations)
        #将检测到的人脸数量赋值给响应对象的number字段


        response.use_time=end_time-start_time
        #将识别耗时赋值给响应对象的use_time字段

        for (top,right,bottom,left) in face_locations:
            response.top.append(top)#append方法将人脸位置的坐标添加到响应对象的相应字段列表中
            response.right.append(right)#append方法将人脸位置的坐标添加到响应对象的相应字段列表中
            response.bottom.append(bottom)#append方法将人脸位置的坐标添加到响应对象
            response.left.append(left)#append方法将人脸位置的坐标添加到响应对象的相应字段列表中 
        #将人脸在图像中的位置赋值给响应对象的相应字段



        return response
    
def main(args=None):
    rclpy.init(args=args)#初始化ROS2客户端库
    node=FaceDetectorinNode()#创建FaceDetectorinNode节点实例
    rclpy.spin(node)#进入ROS2事件循环,等待服务请求
    rclpy.shutdown()#关闭ROS2客户端库