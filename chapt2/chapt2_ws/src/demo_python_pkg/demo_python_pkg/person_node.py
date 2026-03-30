import rclpy
from rclpy.node import Node

class PersonNode(Node):

    #定义PersonNode类，继承自Node类

    def __init__(self,node_name:str,name:str,age:int) -> None:
        
        #构造方法里面有三个参数，分别是节点名称，姓名，年龄

        super().__init__(node_name)#调用父类的构造方法

        print("PersonNode的init方法被调用")
        self.age= age #属性
        self.name= name #属性
    
    def eat(self,food:str)->None:
        self.get_logger().info(f"我叫{self.name}，今年{self.age}正在吃{food}")#方法


def main():
    rclpy.init()#初始化rclpy

    node= PersonNode('person_node','张三',20)#实例化对象,传入节点名称，姓名，年龄
    node.eat('苹果')#调用方法

    rclpy.spin(node)#保持节点运行
    rclpy.shutdown()#关闭rclpy

    