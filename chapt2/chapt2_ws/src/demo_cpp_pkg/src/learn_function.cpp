#include<iostream>
#include<functional>

using namespace std;

void save_with_free_fun(const string &file_name)
{
    cout << "调用了自由函数，保存" << file_name  << endl;
    // 模拟保存数据的操作
}

class FileSave
{
public:
    void save_with_member_fun(const string &file_name)
    {
        cout << "调用了成员函数，保存" << file_name << endl;
        // 模拟保存数据的操作
    };
};


int main()
{

    // 使用成员函数
    FileSave file_save;
    auto save_with_lambda =[](const string &file_name)->void
    {
        cout<<"调用了 Lambda 表达式，保存"<< file_name <<endl;
    };

    // 使用自由函数放入function对象中
    function<void(const string &)> func1 = save_with_free_fun;

    // 调用 Lambda 表达式放入function对象中
    function<void(const string &)> func2 = save_with_lambda;


    // 使用成员函数放入function对象中

    function<void(const string &)> func3  = bind(&FileSave::save_with_member_fun, &file_save, placeholders::_1);


    func1("data1.txt");
    func2("data2.txt");
    func3("data3.txt");


    return 0;
}