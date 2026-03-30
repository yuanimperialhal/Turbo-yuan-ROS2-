import rclpy
from rclpy.node import Node
from example_interfaces.msg import String #从example_interfaces包中导入String消息类型
import threading
from queue import Queue
import time
import espeakng


class NovelSubNode(Node):
    def __init__(self, node_name):
        super().__init__(node_name)
        self.get_logger().info('小说朗读节点启动')

        self.novels_queue_= Queue()
        self.get_logger().info('正在创建小说订阅者...')

        self.novel_subscriber_ = self.create_subscription(
            String,'novel',self.novel_callback,10
        )
        self.get_logger().info('小说订阅者创建成功，正在等待小说内容...')

        self.speech_thread_=threading.Thread(target=self.speak_thread)
        self.get_logger().info('启动小说朗读线程...')

        self.speech_thread_.start()
        self.get_logger().info('小说朗读线程启动成功')




    def novel_callback(self, msg):
        # if msg.data=='':
        #     self.get_logger().info('收到空小说内容，忽略该消息')
        # else:
            self.novels_queue_.put(msg.data)
            self.get_logger().info('收到小说内容，已加入朗读队列')

    def speak_thread(self):
        speaker = espeakng.Speaker()
        self.get_logger().info('初始化语音引擎...')

        speaker.voice = 'zh'
        self.get_logger().info('语音引擎初始化完成，开始朗读小说内容...')

        
        
        
    
        while rclpy.ok() :
            if self.novels_queue_.qsize() > 0:
                self.get_logger().info('检测到朗读队列中有小说内容，开始朗读...')
                text=self.novels_queue_.get()
                self.get_logger().info(f'正在朗读小说内容：{text}')
                speaker.say(text)
                
                speaker.wait()
                self.get_logger().info('小说内容朗读完成')
            else:
                time.sleep(1)
                self.get_logger().info('朗读队列为空，等待新的小说内容...')
                # 等待一段时间再检查队列，避免忙等待




def main(args=None):
    rclpy.init(args=args)

    node = NovelSubNode("novel_read")
    rclpy.spin(node)
    rclpy.shutdown()
