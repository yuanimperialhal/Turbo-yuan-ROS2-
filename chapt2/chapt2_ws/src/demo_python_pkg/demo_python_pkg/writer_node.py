from demo_python_pkg.person_node import PersonNode

class WriterNode(PersonNode):
    def __init__(self,book:str,name:str,age:int) -> None:#构造方法
        super().__init__(name, age)#调用父类的构造方法
        print('WriterNode的init方法被调用')
        self.book= book#属性


def main():
    writer= WriterNode('三国演义','李四',18)#实例化对象
    writer.eat('香蕉')#调用继承自PersonNode的方法