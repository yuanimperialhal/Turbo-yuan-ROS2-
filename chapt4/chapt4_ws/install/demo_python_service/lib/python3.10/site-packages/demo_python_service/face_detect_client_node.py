import rclpy
from rclpy.node import Node
from chapt4_interfaces.srv import FaceDetector
from cv_bridge import CvBridge
from sensor_msgs.msg import Image  #sensor_msgs是ROS中用于处理图像数据的消息类型库,其中Image是其中的一种消息类型,用于表示图像数据
from ament_index_python.packages import get_package_share_directory#获取资源文件路径
from rcl_interfaces.srv import SetParameters
#用于动态参数设置的服务类型
from rcl_interfaces.msg import Parameter,ParameterValue,ParameterType 
#用于动态参数设置的消息类型
import os
import cv2

class FaceDetectorClientNode(Node):
    def __init__(self):
        super().__init__('face_detector_client_node')#创建一个名为face_detector_client_node的ROS节点
        self.client=self.create_client(FaceDetector,'/face_detect')#创建一个名为/face_detect的服务客户端,该客户端将用于向服务器发送请求并接收响应
        self.bridge=CvBridge()#创建一个CvBridge对象,该对象用于在ROS图像消息和OpenCV图像之间进行转换
        self.get_logger().info('FaceDetectorClientNode has been started.')#日志记录器
        self.test1_image_path=get_package_share_directory('demo_python_service')+'/resource/ceshi2.jpg'
        #获取测试图像的路径
        self.image=cv2.imread(self.test1_image_path)#使用OpenCV加载测试图像


    def send_request(self):
        #发送服务请求，并处理响应
        while self.client.wait_for_service(timeout_sec=1.0) is False:
            self.get_logger().info(f'等待服务上线...')#等待服务上线,每隔1秒检查一次
        
        #2.构造request对象,并填充请求数据
        request=FaceDetector.Request()
        #创建一个FaceDetector服务的请求对象
        request.image=self.bridge.cv2_to_imgmsg(self.image)
        #将OpenCV图像转换为ROS图像消息,并将其赋值给请求对象的image字段

        #3.发送spin等待服务处理完成
        future=self.client.call_async(request)
        #异步调用服务,将请求对象作为参数传递给call_async方法,该方法会返回一个Future对象,用于跟踪服务调用的状态和结果
        rclpy.spin_until_future_complete(self,future)
        #使用rclpy.spin_until_future_complete函数等待服务调用完成,该函数会阻塞当前线程,
        #直到Future对象的状态变为完成(无论是成功还是失败

        #4.根据服务调用结果进行处理
        response=future.result()
        #获取服务调用的结果,如果服务调用成功,则response将包含服务器返回的数据;如果服务调用失败,则response将为None
        self.get_logger().info(f'服务调用完成,结果: 图像多少张脸：{response.number}，耗时{response.use_time}ms')#日志记录服务调用的结果

        #注释show_face_locations函数,防止显示堵塞无法多次请求
        #self.show_face_locations(response)

    def call_set_parameters(self,parameters):
        #1.创建一个客户端，并等待服务上线
        client=self.create_client(SetParameters,'/face_detect_node/set_parameters')
        
        while not client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info('等待参数服务上线...')
        #2.构造请求对象
        request=SetParameters.Request()
        request.parameters=parameters
        #3.发送请求并等待响应
        future=client.call_async(request)
        rclpy.spin_until_future_complete(self,future)
        #4.处理响应结果
        response=future.result()
        
        return response

    def update_detect_model(self,model):
        #1.创建一个Parameter对象

        param=Parameter()
        param.name='face_locations_model'
        #2.设置参数值,并赋值给Parameter对象
        new_model_value=ParameterValue()
        new_model_value.type=ParameterType.PARAMETER_STRING
        new_model_value.string_value=model
        param.value=new_model_value

        #3.请求更新参数，并处理响应结果
        response=self.call_set_parameters([param])
        for result in response.results:
            if result.successful:
                self.get_logger().info(f'参数更新成功: {result.reason}')
            else:
                self.get_logger().error(f'参数更新失败: {result.reason}')



    def show_face_locations(self,response):
        #在图像上绘制人脸位置

        for i in range(response.number):
            top=response.top[i]
            right=response.right[i]
            bottom=response.bottom[i]
            left=response.left[i]
            cv2.rectangle(self.image,(left,top),(right,bottom),(0,255,0),4)
            #在图像上绘制一个矩形框,框住检测到的人脸,矩形框的颜色为绿色(0,255,0),线宽为4
        cv2.imshow("Face Detection Result",self.image)
        #显示图像窗口,窗口标题为"Face Detection Result",显示的图像为self.image
        #该图像已经在上一步中绘制了检测到的人脸位置
        cv2.waitKey(0)


def main(args=None):
    rclpy.init(args=args)
    node=FaceDetectorClientNode()
    node.update_detect_model('hog')#调用update_detect_model方法,将检测模型更新为'hog'
    node.send_request()#调用send_request方法,发送服务请求并处理响应
    node.update_detect_model('cnn')#调用update_detect_model方法,将检测模型更新为'cnn'
    node.send_request()#调用send_request方法,发送服务请求并处理响应


    rclpy.spin(node)#进入ROS2事件循环,等待服务请求
    
    rclpy.shutdown()