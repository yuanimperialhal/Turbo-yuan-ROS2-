#include<iostream>
#include<algorithm>

using namespace std;

int main()
{
    auto add =[] (int a, int b) -> int { return a+b; };
    // 定义一个 lambda 表达式，接受两个整数参数并返回它们的和

    int sum=add(3,3);
    // 调用 lambda 表达式，计算 3 和 3 的和

    auto print_sum = [sum]()->void{cout<<"The sum is: "<<sum<<endl; };
    // 定义一个 lambda 表达式，捕获外部变量 sum 并打印其值

    print_sum();

    return 0;


}