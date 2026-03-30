#include<iostream>
using namespace std;

int main()
{
    auto x = 42; // 使用 auto 关键字自动推导变量类型为 int
    auto y = 3.14; // 使用 auto 关键字自动推导变量类型为 double
    auto z = "Hello, World!"; // 使用 auto 关键字自动推导变量类型为 const char*


    cout<<x<<endl;
    cout<<y<<endl;
    cout<<z<<endl;


    cout << "x: " << x << ", type: " << typeid(x).name() << endl;
    cout << "y: " << y << ", type: " << typeid(y).name() << endl;
    cout << "z: " << z << ", type: " << typeid(z).name() << endl;

    return 0;
}