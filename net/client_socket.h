//客户端 socket 封装
#pragma once
#include <sys/socket.h>   //socket,connect,send,recv套接子系统调用
#include <netinet/in.h>   //sockaddr_inj结构体，端口字节序转换htons
#include <unistd.h>       //close关闭文件描述符
#include <arpa/inet.h>    //inet_pton：IP字符串转网络字节序整数
#include <string>
#include "../utils/tools.h" //引入线程安全日志log_print
class ClientSocket {
public:
    int sock_fd;        //客户端socket文件描述符
    ClientSocket(const std::string& ip,int port) {
        //创建TCP客户端socket，AF_INET表示IPv4，SOCK_STREAM表示TCP协议，创建失败返回-1
        sock_fd = socket(AF_INET,SOCK_STREAM,0);
        //IPv4专用地址结构体，存放目标服务端IP，端口，{}做零初始化清空脏数据
        sockaddr_in addr{};
        addr.sin_family=AF_INET;       //标记地址族为IPv4
        addr.sin_port=htons(port);      //主机字节序转换为网络大端字节序
        //IP字符串转网络字节序整数，写入地址结构体，成功返回1，失败返回0
        inet_pton(AF_INET,ip.c_str(),&addr.sin_addr);
        if(connect(sock_fd,(sockaddr*)&addr,sizeof(addr))<0){
            log_print("服务端连接失败");
            exit(1);
        }
    }
    void send_data(const std::string& msg) {
        send(sock_fd,msg.c_str(),msg.size(),0);
    }
    std::string recv_data(int buf_len=1024) {
        char buf[1024]{};
        int len = recv(sock_fd,buf,buf_len,0);
        if(len<=0) return "";
        return std::string(buf,len);
    }
    ~ClientSocket(){
        close(sock_fd);
    }
};