#include <iostream> //读写io c++标准库
#include <fstream>  //读写文件 c++标准库
#include <string>   //字符串类 c++标准库
#include <sstream>  //字符串流 c++标准库

#include "httplib.h"

#include "json.hpp"
using json = nlohmann::json;

#include "dijkstra.cpp"

int main()
{
    // 创建一个服务器实例
    httplib::Server svr;
    // 定义一个GET接口
    svr.Get("/", [](const httplib::Request &req, httplib::Response &res)
            {
                // code
                // 从请求中获取参数
                string start_pos = req.get_param_value("start_pos");
                string end_pos = req.get_param_value("end_pos");
                double percent = stod(req.get_param_value("percent"));
                int car_cost = stoi(req.get_param_value("car_cost"));
                int train_cost = stoi(req.get_param_value("train_cost"));
                int plane_cost = stoi(req.get_param_value("plane_cost"));
                int min_transition = stoi(req.get_param_value("min_transition"));
                // 调用dijkstra算法
                json result = dijkstra_run(start_pos, end_pos, percent, car_cost, plane_cost, train_cost, min_transition);
                // 将结果以json格式写入响应
                res.set_content(result.dump(), "application/json");
                //
            });
    // 启动服务器，监听端口5000
    svr.listen("0.0.0.0", 5000);
    return 0;
}