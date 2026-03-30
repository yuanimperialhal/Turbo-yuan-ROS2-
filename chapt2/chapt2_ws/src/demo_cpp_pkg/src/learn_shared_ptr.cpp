#include<iostream>
#include<memory> // 包含智能指针的头文件
using namespace std;

int main()
{
    auto p1=make_shared<string>("This is a str");
    cout<<"p1引用的内存地址计数为："<<p1.use_count()<<"---指向的内存地址为---"<<p1.get()<<endl;

    auto p2=p1; // p2 共享 p1 指向的内存

    cout<<"p1引用的内存地址计数为："<<p1.use_count()<<"---指向的内存地址为---"<<p1.get()<<endl;
    cout<<"p2引用的内存地址计数为："<<p2.use_count()<<"---指向的内存地址为---"<<p2.get()<<endl;

    p1.reset();// 释放 p1 对内存的引用

    cout<<"p1引用的内存地址计数为："<<p1.use_count()<<"---指向的内存地址为---"<<p1.get()<<endl;
    cout<<"p2引用的内存地址计数为："<<p2.use_count()<<"---指向的内存地址为---"<<p2.get()<<endl;

    cout<<"p2指向的内容为："<<*p2<<endl;
    cout<<"p2指向的内容为："<<p2->c_str()<<endl;

    return 0;

}