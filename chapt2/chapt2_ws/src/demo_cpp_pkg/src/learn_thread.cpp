#include <iostream> 
#include <thread>
#include <chrono>
#include <functional>
#include <algorithm>
#include <cpp-httplib/httplib.h>
using namespace std;

class Download
{
    public:
        void download(const string &host,const string &path,const function<void(const string &,const string &)> &callback)
        {
            cout<<"线程ID："<<this_thread::get_id()<<" 开始下载文件"<<endl;

            httplib::Client client(host);
            /*创建HTTP客户端对象,本代码的详细说明为：

                httplib是一个轻量级的C++ HTTP库，可以用于发送HTTP请求和处理HTTP响应。

                Client是httplib库中的一个类，用于创建HTTP客户端对象。

                client是Client类的一个实例，通过传入host参数来初始化。

                host是一个字符串，表示服务器的主机名或IP地址。
             */

            auto response = client.Get(path);
             /*发送GET请求,本代码的详细说明为：

                auto是C++11引入的关键字，用于自动推导变量的类型。

                response是一个变量，用于存储HTTP响应对象。

                client.Get(path)调用了Client类的Get方法，发送一个GET请求到指定的路径path。

                path是一个字符串，表示请求的资源路径。
             */

            if(response && response->status == 200)
            {
                callback(path,response->body);

            }
              /*处理响应,本代码的详细说明为：

                if语句用于检查HTTP响应是否有效且状态码为200（表示请求成功）。

                response是一个指向HTTP响应对象的智能指针。

                response->status访问响应对象的状态码。

                callback(path,response->body)调用传入的回调函数，将路径和响应体作为参数传递给回调函数。

                response->body访问响应对象的正文内容。
             */           

        }
        void start_download(const string &host,const string &path,const function<void(const string &,const string &)> &callback)
        {
            auto download_fun = bind(&Download::download,this,placeholders::_1,placeholders::_2,placeholders::_3);
             /*绑定成员函数,本代码的详细说明为：

                auto是C++11引入的关键字，用于自动推导变量的类型。

                download_fun是一个变量，用于存储绑定后的函数对象。

                bind是C++标准库中的一个函数模板，用于将成员函数与对象实例绑定在一起。

                &Download::download是成员函数的指针，表示Download类中的download方法。

                this是当前对象的指针，表示要调用download方法的对象实例。

                placeholders::_1、placeholders::_2和placeholders::_3是占位符，表示传递给绑定函数的参数位置。
             */

            thread download_thread(download_fun,host,path,callback);
             /*创建线程,本代码的详细说明为：

                thread是C++标准库中的一个类，用于创建和管理线程。

                download_thread是一个thread类的实例，表示一个新的线程对象。

                download_fun是之前绑定的函数对象，表示要在线程中执行的函数。

                host、path和callback是传递给download_fun函数的参数。
             */

            download_thread.detach();
            /*分离线程,本代码的详细说明为：
    
                    download_thread.detach()调用了thread类的detach方法，将线程与主线程分离。
                    分离线程后，线程将独立运行，主线程不会等待其完成。这对于后台任务或不需要等待结果的任务非常有用。
            */
        }

};

int main()
{
    Download download;

    auto dowload_finish_callback=[](const string &path,const string &result)->void
    {
        cout<<"下载文件："<<path<<" 完成，文件大小为："<<result.length()<<result.substr(0,16)<<endl;
    };
    //定义回调函数,本代码的详细说明为：

        //auto是C++11引入的关键字，用于自动推导变量的类型。
        //dowload_finish_callback是一个变量，用于存储定义的回调函数。
        //[]表示这是一个lambda表达式的开始，表示捕获列表为空。
        //(const string &path,const string &result)是lambda表达式的参数列表，表示接受两个字符串参数path和result。
        //->void表示lambda表达式的返回类型为void。
        //是lambda表达式的函数体，包含打印下载完成信息的代码。
        //result.substr(0,16)表示获取下载结果的前16个字符作为预览。

    download.start_download("http://localhost:8000","/novel1.txt",dowload_finish_callback);
    download.start_download("http://localhost:8000","/novel2.txt",dowload_finish_callback);
    download.start_download("http://localhost:8000","/novel3.txt",dowload_finish_callback);

    this_thread::sleep_for(chrono::seconds(10));
    /*等待下载完成,本代码的详细说明为：

        this_thread是C++标准库中的一个命名空间，表示当前线程。
        sleep_for是this_thread命名空间中的一个函数，用于使当前线程休眠指定的时间。
        chrono::seconds(10)表示休眠10秒钟，chrono是C++标准库中的一个命名空间，seconds是其中的一个类，用于表示时间段。
     */



return 0;
}