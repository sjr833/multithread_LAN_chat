//epoll服务端网络封装
#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <unordered_map>
#include "../utils/tools.h"
#include "../model/data.h"
const int PORT=8888;
const int MAX_EVENTS=512;
class ServerSocket {
public:
    int listen_fd,epoll_fd;
    std::unordered_map<int,User> fd_to_user;
    ServerSocket(){
        // 创建监听socket
        listen_fd = socket(AF_INET,SOCK_STREAM,0);
        int opt=1;
        setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
        sockaddr_in addr{};
        addr.sin_family=AF_INET;
        addr.sin_port=htons(PORT);
        addr.sin_addr.s_addr=INADDR_ANY;
        bind(listen_fd,(sockaddr*)&addr,sizeof(addr));
        listen(listen_fd,10);
        epoll_fd=epoll_create1(0);
        epoll_event ev{};
        ev.data.fd=listen_fd;
        ev.events=EPOLLIN;
        epoll_ctl(epoll_fd,EPOLL_CTL_ADD,listen_fd,&ev);
        log_print("服务端启动，监听端口："+std::to_string(PORT));
    }
    // 阻塞等待事件触发
    int wait_event(epoll_event out_events[]) {
        return epoll_wait(epoll_fd,out_events,MAX_EVENTS,-1);
    }
    // 接收新客户端连接
    int accept_client() {
        sockaddr_in client_addr{};
        socklen_t len=sizeof(client_addr);
        int client_fd = accept(listen_fd,(sockaddr*)&client_addr,&len);
        epoll_event ev{};
        ev.data.fd=client_fd;
        ev.events=EPOLLIN;
        epoll_ctl(epoll_fd,EPOLL_CTL_ADD,client_fd,&ev);
        log_print("新客户端接入 fd:"+std::to_string(client_fd));
        return client_fd;
    }
    ~ServerSocket(){
        close(listen_fd);
        close(epoll_fd);
    }
};