//客户端控制台交互界面
#pragma once
#include "../net/client_socket.h" 
#include "../utils/tools.h"
#include <thread>    //C++标准线程库，用来创建接受消息的后台子线程
static void recv_loop(ClientSocket& sock) {
    while(true) {
        std::string res = sock.recv_data();
        if(res.empty()) break;
        std::cout<<res<<"\n";
    }
}
inline void run_ui(ClientSocket& sock) {
    std::string nick;
    std::cout<<"输入你的昵称完成登录：";
    std::getline(std::cin,nick);
    sock.send_data(nick);
    // 子线程异步接收服务端推送消息
    std::thread recv_th(recv_loop,std::ref(sock));
    recv_th.detach();
    std::string input;
    std::cout<<"输入聊天内容发送群聊，输入quit退出\n";
    while(true) {
        std::getline(std::cin,input);
        if(input=="quit") break;
        sock.send_data(input);
    }
}