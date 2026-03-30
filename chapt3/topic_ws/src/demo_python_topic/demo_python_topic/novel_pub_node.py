import rclpy
from rclpy.node import Node
import requests
from example_interfaces.msg import String
#从example_interfaces包中导入String消息类型，用于发布文本消息，  
from queue import Queue

class NovelPubNode(Node):
    def __init__(self,node_name):
        super().__init__(node_name)#继承父类的初始化方法
        self.novels_queue_ = Queue()
        #为什么要创建队列？因为下载的小说是多行的，发布消息时每次发布一行

        self.publisher_ = self.create_publisher(String, 'novel', 10)
        #创建发布者对象，发布String类型的消息，主题名为novel，队列长度10
        
        self.timer_=self.create_timer(1,self.timer_callback)
        #创建定时器对象，每1秒调用一次timer_callback函数

    def download_novel(self,url):
        response = requests.get(url)
        #requests库发送HTTP GET请求，获取小说内容

        response.encoding='utf-8'
        #设置响应内容的编码格式为utf-8，防止中文乱码

        self.get_logger().info(f'下载完成:{url}')
        #打印日志，提示下载完成

        for line in response.text.splitlines():
            self.novels_queue_.put(line)
            #将小说的每一行放入队列中

    def timer_callback(self):
        if self.novels_queue_.qsize()>0:
            msg=String()#创建消息对象的作用是为了发布消息

            msg.data=self.novels_queue_.get()
             #从队列中获取一行小说内容将获取的小说内容赋值给消息对象的data属性
             #data属性是String消息类型中用于存储文本内容的字段
            
            self.publisher_.publish(msg)
            #发布消息，将消息对象发布到主题上
            self.get_logger().info(f'发布消息:{msg.data}')


def main():
    rclpy.init()
    node=NovelPubNode('novel_pub')
    node.download_novel('http://localhost:8000/novel1.txt')  # 下载小说
    rclpy.spin(node)
    rclpy.shutdown()

    

    
    