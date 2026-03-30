import threading
import requests

class Dowdload:
    def dowload(self,url,callback):#作为线程执行的函数
        print(f"线程:{threading.get_ident()}开始下载{url}...")#获取线程ID
        response=requests.get(url)#发送请求
        response.encoding='utf-8'#设置编码格式
        callback(url,response.text)#调用回调函数

    def start_dowload(self,url,callback):#启动线程
        thread=threading.Thread(target=self.dowload,args=(url,callback))#创建线程对象
        thread.start()#启动线程



def download_finish_callback(url,result):#回调函数
    print(f"线程:{threading.get_ident()}下载完成{url},内容长度:{len(result)},内容为：{result[:5]}...")
    #获取线程ID,输出内容分别为URL,内容长度,内容前5个字符


def main():

    d=Dowdload()#创建下载对象

    d.start_dowload('http://localhost:8000/novel1.txt',download_finish_callback)#启动下载线程1
    d.start_dowload('http://localhost:8000/novel2.txt',download_finish_callback)#启动下载线程2
    d.start_dowload('http://localhost:8000/novel3.txt',download_finish_callback)#启动下载线程3


